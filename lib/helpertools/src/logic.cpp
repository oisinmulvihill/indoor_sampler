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

const char *requestLine = "POST /log/sample/indoor HTTP/1.0";

const char *contentTypeLine = 
  "Content-Type: application/x-www-form-urlencoded";


void hostHeader(
  char *request,
  int request_size,
  char *server_host, 
  int server_port
) {
  memset((void *) request, 0, request_size);
  snprintf(request, request_size, "Host: %s:%d", server_host, server_port);
}

void contentLengthHeader(
  char *request,
  int request_size,
  char *report
) {
  memset((void *) request, 0, request_size);
  snprintf(
    request, request_size, "Content-Length: %d", (int) strlen(report)
  );
}

/*
  Given the temperature, humidity, pressure and gas values generate the 
  report string.

  The readings are given as integers instead of floating point numbers
  
  For example:

  Temperature: 
    -XXYY e.g. -2856 = -28.56C [range -40 <-> +85 C]

  Humidity: 
    XXXYYY e.g. 41089 = 41.089% [range 0 <-> 100%]

  Pressure: 
    XXXXYY e.g. 100915 = 1009.15hPa [range 300hPa <-> 1100hPa]

  Gas: 
    XXXXXX e.g. 108926 [mΩ resistance, I'm not sure of range]

  The resultant report will be:

  'type=bme680&t=28562&h=41089&p=100915&g=108926'

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
  int32_t temperature, 
  int32_t humidity, 
  int32_t pressure,
  int32_t gas
) {
  // Clear out previous displayed values:
  memset(report, 0, report_size);

  snprintf(
    report,
    report_size,
    "type=bme680&t=%05lu&h=%06lu&p=%07lu&g=%08lu",
    (unsigned long) temperature,
    (unsigned long) humidity,
    (unsigned long) pressure,
    (unsigned long) gas
  );

  return report;
}
