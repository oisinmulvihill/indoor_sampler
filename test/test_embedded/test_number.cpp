/* Tests run on the arduino ethernet board.

Oisin Mulvihill
2020-06-04

*/
#include <Arduino.h>
#include <unity.h>
#include "number.h"


void test_decimalToString(void) {
    char output[6] = {0};
    float humidity = 34.169;
    TEST_ASSERT_EQUAL("34.16", decimalToString(output, 6, humidity));
}

void setup() {
    UNITY_BEGIN();
}

void loop() {
    RUN_TEST(test_decimalToString);
    UNITY_END();
}