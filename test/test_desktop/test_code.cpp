/* Tests run on the desktop (without access to arduino libraries).

Oisin Mulvihill
2020-06-04

*/
#include <iostream>
#include <unity.h>
#include "number.h"
#include "logic.h"


void test_decimalToString(void) {
    char output[6] = {0};
    
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
        "4.1", decimalToString(output, 6, 4.1)
    );
    
    TEST_ASSERT_EQUAL_STRING(
        "122.1", decimalToString(output, 6, 122.1)
    );
    
    TEST_ASSERT_EQUAL_STRING(
        "1.1", decimalToString(output, 6, 1.1)
    );
    
    TEST_ASSERT_EQUAL_STRING(
        "12.1", decimalToString(output, 6, 12.1)
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
    // Maximum string size I expect "t=XXX.YY&h=XXX.YY&d=XXX.YY\0" (27)
    char report[27] = {0};

    generateReport(
        report, 
        sizeof(report), 
        21.322, 
        34.871,
        17.12
    );
    TEST_ASSERT_EQUAL_STRING("t=21.32&h=34.87&d=17.12", report);

    generateReport(
        report, 
        sizeof(report), 
        99.99, 
        100.0,
        0.0
    );
    TEST_ASSERT_EQUAL_STRING("t=99.99&h=100.0&d=0.0", report);

    // Note: the trailing 0 will be dropped in conversion
    generateReport(
        report, 
        sizeof(report), 
        1.10, 
        2.20,
        3.30
    );
    TEST_ASSERT_EQUAL_STRING("t=1.1&h=2.3&d=3.3", report);

    generateReport(
        report, 
        sizeof(report), 
        1.1, 
        2.2,
        3.3
    );
    TEST_ASSERT_EQUAL_STRING("t=1.1&h=2.2&d=3.3", report);
}

void test_generateHTTPPOST(void) {
    char request[MAX_REQUEST_LINES][MAX_REQUEST_LINE_SIZE] = {0};

    generateHTTPPost(request, "tarsis", "t=22.87&h=33.34&d=12.4");

    TEST_ASSERT_EQUAL_STRING(
        "POST /log/sample/indoor HTTP/1.0", 
        request[0]
    );
    TEST_ASSERT_EQUAL_STRING(
        "Host: tarsis", 
        request[1]
    );
    TEST_ASSERT_EQUAL_STRING(
        "Content-length: 22", 
        request[2]
    );
    TEST_ASSERT_EQUAL_STRING(
        "Content-Type: application/x-www-form-urlencoded", 
        request[3]
    );
    TEST_ASSERT_EQUAL_STRING(
        "\n", 
        request[4]
    );
    TEST_ASSERT_EQUAL_STRING(
        "t=22.87&h=33.34&d=12.4", 
        request[5]
    );
}

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_decimalToString);
    RUN_TEST(test_generateReport);
    RUN_TEST(test_generateHTTPPOST);
    UNITY_END();

    return 0;
}
