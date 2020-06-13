/* Indoor Sampler

This recovers the current temperature & humidity. It then works out the dew 
point. These three variables are then POSTed to a REST API endpoint which will
store the logged sample.

DHTesp DHT11 library
- https://github.com/beegee-tokyo/DHTesp

Oisin Mulvihill
2020-05-29

*/
#include <SPI.h>
#include <Ethernet.h>
#include "DHTesp.h"
#include "logic.h"

// 5min = 1000 * 60 * 5
// #define DELAY 300000
#define DELAY 5000
#define DHT_PIN 2
#define HTTP_DEBUG
#define PORT 8080
#define HOSTNAME "tarsis"

DHTesp dht;
EthernetClient client;

int server_port = PORT;
char server_host[] = HOSTNAME;

// MAC Address for the Arduino Ethernet.
byte mac[] = {0x90, 0xA2, 0xDA, 0x00, 0x61, 0x58};

void setup() {
  // Set up temp/humidity sensor
  dht.setup(DHT_PIN, DHTesp::DHT11);

  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // start the Ethernet connection:
  Serial.println("Initialize Ethernet with DHCP:");
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    } else if (Ethernet.linkStatus() == LinkOFF) {
      Serial.println("Ethernet cable is not connected.");
    }
    // no point in carrying on, so do nothing forevermore:
    while (true) {
      delay(1);
    }
  }
}

void indoor_sample() {
  char line[MAX_REQUEST_LINE_SIZE] = {0};
  char report[MAX_REQUEST_LINE_SIZE] = {0};
  char request[MAX_REQUEST_LINES][MAX_REQUEST_LINE_SIZE] = {0};
  float temperature = dht.getTemperature();
  float humidity = dht.getHumidity();
  float dewPoint = dht.computeDewPoint(temperature, humidity);

  generateReport(
    report,
    sizeof(report),
    temperature,
    humidity,
    dewPoint
  );

  generateHTTPPost(request, server_host, report);

  Serial.print("Sending report '");
  Serial.print(report);
  memset(line, 0, sizeof(line));
  snprintf(line, sizeof(line), "' to %s:%d... ", server_host, server_port);
  Serial.print(line);
  
  if (client.connect(server_host, server_port)) {
    // Success, connected OK, send HTTP POST request lines:
    for(int index=0; index < MAX_REQUEST_LINES; index++) {
      Serial.println(report[index]);
      client.println(report[index]);
    }
  } else {
    // This will be tried again later, so not a huge deal if we don't connect.
    Serial.println("Connection failed!");
  }  

  delay(DELAY);
}

void loop() {
  switch (Ethernet.maintain()) {
    case 1:
      //renewed fail
      break;

    case 2:
      //renewed success
      break;

    case 3:
      //rebind fail
      break;

    case 4:
      //rebind success
      break;

    default:
      indoor_sample();
      break;
  }
}