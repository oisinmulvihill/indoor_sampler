/* Indoor Sampler

This recovers the current temperature, humidity, pressure and gas values. These
variables are then POSTed to a REST API endpoint which will store the logged 
sample.

Oisin Mulvihill
2020-05-29

*/
#include <SPI.h>
#include <Wire.h>
#include "Ethernet.h"
#include "logic.h"
#include "Zanshin_BME680.h"

// 5min = 1000 * 60 * 5
// #define DELAY 300000
#define DELAY 5000
#define DHT_PIN 2
#define HTTP_DEBUG
#define PORT 8080
#define HOSTNAME "tarsis"

BME680_Class BME680;
EthernetClient client;
int server_port = PORT;
char server_host[] = HOSTNAME;
// MAC Address for the Arduino Ethernet.
byte mac[] = {0x90, 0xA2, 0xDA, 0x00, 0x61, 0x58};


void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // start the Ethernet connection:
  Serial.println("Initialize Ethernet with DHCP.");
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // Check for Ethernet hardware present
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      Serial.println(
        "Ethernet shield was not found.  Sorry, can't run without hardware. :("
      );
      while (true) {
        delay(1); // do nothing, no point running without Ethernet hardware
      }
    }
    if (Ethernet.linkStatus() == LinkOFF) {
      Serial.println("Ethernet cable is not connected.");
    }  
  } else {
    Serial.println("DHCP assigned IP:");
    Serial.println(Ethernet.localIP());
  }

  // Start BME680 using I2C, use first device found
  while (!BME680.begin())
  {
    Serial.print("Unable to find BME680. Trying again in 5 seconds.\n");
    delay(5000);
  }

  Serial.print("BME680: Configuring.\n");
  BME680.setOversampling(TemperatureSensor, Oversample16);
  BME680.setOversampling(HumiditySensor, Oversample16);
  BME680.setOversampling(PressureSensor, Oversample16);
  BME680.setIIRFilter(IIR4);
  BME680.setGas(320,150); // 320°c for 150 milliseconds
}

void indoor_sample() {
  char report[MAX_REPORT_SIZE] = {0};
  char buffer[MAX_REPORT_SIZE] = {0};
  int32_t temperature = 0;
  int32_t humidity = 0;
  int32_t pressure = 0;
  int32_t gas = 0;

  BME680.getSensorData(temperature, humidity, pressure, gas);

  generateReport(report, sizeof(report), temperature, humidity, pressure, gas);
  Serial.print("Sending report '");
  Serial.print(report);
  memset(buffer, 0, sizeof(buffer));
  snprintf(buffer, sizeof(buffer), "' to %s:%d.", server_host, server_port);
  Serial.println(buffer);
  
  if (client.connect(server_host, server_port)) {
    // Success, connected OK, send HTTP POST request lines:
    client.println(HTTP_REQUEST_LINE);
    // Request Headers
    hostHeader(buffer, sizeof(buffer), server_host, server_port);
    client.println(buffer);
    client.println(HTTP_CONTENT_TYPE);
    contentLengthHeader(buffer, sizeof(buffer), report);
    client.println(buffer);
    sourceAddressHeader(buffer, sizeof(buffer), (unsigned char *) mac);
    client.println(buffer);
    // End of headers
    client.println();
    // Request Body
    client.println(report);
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
      Serial.println("Renew failed!");
      break;

    case 2:
      //renewed success
      Serial.println("Renew success");
      break;

    case 3:
      //rebind fail
      Serial.println("Rebind fail!");
      break;

    case 4:
      //rebind success
      Serial.println("Rebind success");
      break;

    default:
      indoor_sample();
      break;
  }
}