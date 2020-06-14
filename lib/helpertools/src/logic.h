#ifndef _LOGIC_H  
#define _LOGIC_H 

// Max size 7 including null terminator: XXX.YY\0
#define FIELD_MAX_LENGTH 7
#define MAX_REQUEST_LINES 6
#define MAX_REQUEST_LINE_SIZE 64

void generateHTTPPost(
  char request[MAX_REQUEST_LINES][MAX_REQUEST_LINE_SIZE],
  const char *server_host, 
  const char *report
);

char * generateReport(
    char *report, 
    int report_size,
    float temperature, 
    float humidity, 
    float dew_point
);

#endif