#ifndef _LOGIC_H  
#define _LOGIC_H 

#define MAX_REQUEST_LINES 6
// Max size of the generated report string.
// char report[] = "type=bme680&t=12345&h=123456&p=123456789&g=123456";
#define MAX_REPORT_SIZE 50
#define HTTP_REQUEST_LINE "POST /log/sample/indoor HTTP/1.0"
#define HTTP_CONTENT_TYPE "Content-Type: application/x-www-form-urlencoded"
  
void hostHeader(
  char *request,
  int request_size,
  char *server_host, 
  int server_port
);

void contentLengthHeader(
  char *request,
  int request_size,
  char *report
);

char * generateReport(
    char *report, 
    int report_size,
    int32_t temperature, 
    int32_t humidity, 
    int32_t pressure,
    int32_t gas
);

#endif