/* Tests run on the arduino ethernet board.

Oisin Mulvihill
2020-06-04

*/
#include <Arduino.h>
#include <unity.h>
#include "number.h"


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

}

void setup() {
    UNITY_BEGIN();
}

void loop() {
    RUN_TEST(test_decimalToString);
    RUN_TEST(test_generateReport);
    UNITY_END();
}