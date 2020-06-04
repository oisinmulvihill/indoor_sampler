/*
Collection of functions doing the business logic. I'm going to unittest these
as they don't need Arduino specific hardware.

Oisin Mulvihill
2020-05-30

*/
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "number.h"


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
void generateReport(
    float temperature, 
    float humidity, 
    float dew_point,
    char *report, 
    int report_size
) {
  // Size 5: XX.YY
  char temperature_str[5];
  char humidity_str[5];
  char dew_point_str[5];

  // if the number is X.YY I need to reduce the length by one to avoid
  // the space e.g. avoid 't= 8.4' getting 't=8.4'
  int length = 3;

  if(isnan(temperature) || isnan(humidity))
  {
    snprintf(report, report_size, "No temp data :(");
  }
  else {
    // Clear out previous displayed values:
    memset(report, 0, report_size);

    // Convert floats to a string XX.YY 2 decimal fixed format.
    memset(temperature_str, 0, sizeof(temperature_str));
    // dtostrf doesn't pad with 0 so I need to reduce 
    length = (temperature < 10) ? 4 : 3;
    decimalToString(temperature_str, length, temperature);

    memset(humidity_str, 0, sizeof(humidity_str));
    length = (humidity < 10) ? 4 : 3;
    decimalToString(humidity_str, length, humidity);

    memset(dew_point_str, 0, sizeof(dew_point_str));
    length = (dew_point < 10) ? 4 : 3;
    decimalToString(dew_point_str, length, dew_point);

    snprintf(
      report,
      report_size,
      "t=%s&h=%s&d=%s",
      temperature_str,
      humidity_str,
      dew_point_str
    );
  }
}
