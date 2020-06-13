/*
Collection of functions doing the business logic.

Oisin Mulvihill
2020-05-30

*/
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "number.h"
#include "logic.h"


/* Using the given handler construct and send the HTTP Headers and Body.
*/
void generateHTTPPost(
  char request[MAX_REQUEST_LINES][MAX_REQUEST_LINE_SIZE], 
  const char *server_host, 
  const char *report
) {
    char line[MAX_REQUEST_LINE_SIZE] = {0};

    // Send the data as a HTTP POST request:
    //
    // Request line:
    strncpy(request[0], "POST /log/sample/indoor HTTP/1.0", 33);
    
    // HTTP Header fields
    snprintf(line, sizeof(line), "Host: %s", server_host);
    strncpy(request[1], line, sizeof(line));
    memset(line, 0, sizeof(line));

    snprintf(line, sizeof(line), "Content-length: %d", (int) strlen(report));
    strncpy(request[2], line, sizeof(line));
    strncpy(request[3], "Content-Type: application/x-www-form-urlencoded", 48);

    // Empty line to denote end of headers.
    strncpy(request[4], "\n", 1);

    // HTTP Body
    strncpy(request[5], report, strlen(report));
}

/*
    Given the temperature and humidity values generate the report string which
    includes the dew point.
    
    For example:

    temperature=26.23
    humidity=39.01
    dew_point=11.22

    The resultant report will be:

    't=26.2&h=39.0&d=11.2'

    If temperature or humidity are not a number then report will be:

    'No temp data :('

    The report data is in a format capable with that used to POST to a HTTP
    endpoint. It will be easy converted and used as key-value pairs on the 
    server side. It will be POSTed as content type 
    application/x-www-form-urlencoded.

    I use snprintf to generate the report string so you need to pass in the
    report buffer size, so we can safely create the report string.      

*/
char * generateReport(
  char *report, 
  int report_size,
  float temperature, 
  float humidity, 
  float dew_point
) {
  char temperature_str[FIELD_MAX_LENGTH] = {0};
  char humidity_str[FIELD_MAX_LENGTH] = {0};
  char dew_point_str[FIELD_MAX_LENGTH] = {0};
  
  // Clear out previous displayed values:
  memset(report, 0, report_size);

  // Convert float values into a string ready for the report construction. I
  // am not checking the size of the field values. I'm assuming temperature,
  // humidity and dew point will fit into the space I've allocated. The 
  // convert function will safely truncate to fit, but the values store could 
  // be wrong if they are too big. I'm not worried about this for this 
  // application. Temp & Dew point won't realistically be bigger then 2 
  // digits and humidity is a percentage.
  decimalToString(temperature_str, FIELD_MAX_LENGTH, temperature);
  decimalToString(humidity_str, FIELD_MAX_LENGTH, humidity);
  decimalToString(dew_point_str, FIELD_MAX_LENGTH, dew_point);

  snprintf(
    report,
    report_size,
    "t=%s&h=%s&d=%s",
    temperature_str,
    humidity_str,
    dew_point_str
  );

  return report;
}
