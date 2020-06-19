/* Tests run on the desktop (without access to arduino libraries).

Oisin Mulvihill
2020-06-04

*/
#ifdef ARDUINO
#include <Arduino.h>
#endif
#include <unity.h>
#include "number.h"
#include "logic.h"


void test_decimalToString(void) {
    char output[7] = {0};
    
    TEST_ASSERT_EQUAL_STRING(
        "34.16", decimalToString(output, 6, 34.1623)
    );
    
    TEST_ASSERT_EQUAL_STRING(
        "4.12", decimalToString(output, 6, 4.1244)
    );
    
    TEST_ASSERT_EQUAL_STRING(
        "4.11", decimalToString(output, 6, 4.11)
    );
    
    TEST_ASSERT_EQUAL_STRING(
        "4.10", decimalToString(output, 6, 4.1)
    );
    
    TEST_ASSERT_EQUAL_STRING(
        "122.1", decimalToString(output, 6, 122.1)
    );
    
    TEST_ASSERT_EQUAL_STRING(
        "0.00", decimalToString(output, 6, 0)
    );
    
    TEST_ASSERT_EQUAL_STRING(
        "1.10", decimalToString(output, 6, 1.1)
    );

    TEST_ASSERT_EQUAL_STRING(
        "2.20", decimalToString(output, 6, 2.2)
    );
    
    TEST_ASSERT_EQUAL_STRING(
        "12.10", decimalToString(output, 6, 12.1)
    );

    TEST_ASSERT_EQUAL_STRING(
        "-1.22", decimalToString(output, 6, -1.22)
    );

    TEST_ASSERT_EQUAL_STRING(
        "-1.10", decimalToString(output, 6, -1.1)
    );

    TEST_ASSERT_EQUAL_STRING(
        "-99.10", decimalToString(output, 7, -99.1)
    );

    TEST_ASSERT_EQUAL_STRING(
        "-99.11", decimalToString(output, 7, -99.11)
    );

    TEST_ASSERT_EQUAL_STRING(
        "-99.12", decimalToString(output, 7, -99.12)
    );

    // The buffer is not big enough so you will see example of it breaking. You
    // need to make sure output is big enough for maximum number of digits + 
    // '.' + \0 character. The fraction part is always two digits.
    //
    // example of what you would see
    TEST_ASSERT_EQUAL_STRING(
        "122.1", decimalToString(output, 6, 122.19)
    );
    // other bad output examples:
    TEST_ASSERT_EQUAL_STRING(
        "1222.", decimalToString(output, 6, 1222.19)
    );
    TEST_ASSERT_EQUAL_STRING(
        "12222", decimalToString(output, 6, 12222.19)
    );
}

void test_generateReport(void) {
    // Maximum string size I expect 
    char report[] = "type=bme680&t=12345&h=123456&p=123456789&g=123456";

    generateReport(
        report, 
        sizeof(report), 
        1, 
        2,
        3,
        4
    );
    TEST_ASSERT_EQUAL_STRING("type=bme680&t=1&h=2&p=3&g=4", report);

    generateReport(
        report, 
        sizeof(report), 
        2856, 
        41089,
        100915,
        108926
    );
    TEST_ASSERT_EQUAL_STRING(
        "type=bme680&t=2856&h=41089&p=100915&g=108926",
        report
    );
}

void test_hostHeader(void) {
    char request[MAX_REPORT_SIZE] = {0};

    hostHeader(request, MAX_REPORT_SIZE, (char *) "tarsis", 8080);
    TEST_ASSERT_EQUAL_STRING("Host: tarsis:8080", request);

    hostHeader(request, MAX_REPORT_SIZE, (char *) "192.168.0.2", 8080);
    TEST_ASSERT_EQUAL_STRING("Host: 192.168.0.2:8080", request);

    hostHeader(request, MAX_REPORT_SIZE, (char *) "www.google.com", 80);
    TEST_ASSERT_EQUAL_STRING("Host: www.google.com:80", request);
}

void test_contentLengthHeader(void) {
    char request[MAX_REPORT_SIZE] = {0};

    contentLengthHeader(request, MAX_REPORT_SIZE, (char *) "1");
    TEST_ASSERT_EQUAL_STRING("Content-Length: 1", request);

    contentLengthHeader(request, MAX_REPORT_SIZE, (char *) "12");
    TEST_ASSERT_EQUAL_STRING("Content-Length: 2", request);

    contentLengthHeader(request, MAX_REPORT_SIZE, (char *) "123");
    TEST_ASSERT_EQUAL_STRING("Content-Length: 3", request);
}


#ifdef ARDUINO
// Arduino test run
void setup() {
    UNITY_BEGIN();
}

void loop() {
    RUN_TEST(test_decimalToString);
    RUN_TEST(test_generateReport);
    RUN_TEST(test_hostHeader);
    RUN_TEST(test_contentLengthHeader);
    UNITY_END();
}

#else
// Desktop test run
int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_decimalToString);
    RUN_TEST(test_generateReport);
    RUN_TEST(test_hostHeader);
    RUN_TEST(test_contentLengthHeader);
    UNITY_END();

    return 0;
}

#endif


