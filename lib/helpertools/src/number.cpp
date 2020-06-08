#include <stdio.h>
#include <math.h>


/* Convert a float into a string with two decimal precision. 

   E.g.::  21.3413 will become "32.34", 1.1 with become "1.10"

   I'm doing this to avoid using dtostrf(), Arduino specific code, so I can 
   test on board and native. My version is better for me. I only want 2 decimal 
   point precision and I don't want padded 0's. 
   
   The end user will have to make sure the output buffer is big enough or the 
   number will be truncated strangely. I don't check for this.

   Interesting read on how on floating point operations:

   https://www.learncpp.com/cpp-tutorial/floating-point-numbers/

   https://www.exploringbinary.com/
        why-0-point-1-does-not-exist-in-floating-point/

    https://www.exploringbinary.com/
        floating-point-questions-are-endless-on-stackoverflow-com/

*/
char * decimalToString(char *output, int output_size, float number) {
    // handle the negative number in the output as I found it easier to do it 
    // this way
    float absolute = abs(number);
    int isNegative = (number < 0.0) ? 1 : 0;
    // e.g. number: 12.1 recover just the integrer part: 12
    int integer = (int)(absolute);

    // recover fractional part without having to worry about floating point 
    // represention issues
    // e.g. number: 12.11 fractional part: 1211 - 1200 = 11
    int fractional = (int)((absolute * 100) - (integer * 100));
    
    // safely output a string represention of the number with no trailing or 
    // leading zeros.
    if (isNegative) {
        snprintf(output, output_size, "-%d.%02d", integer, fractional);
    } else {
        snprintf(output, output_size, "%d.%02d", integer, fractional);
    }

    // printf("\n\nnumber: %f\n", number);
    // printf("absolute: %f\n", absolute);
    // printf("integer: %d\n", integer);
    // printf("isNegative: %s\n", (isNegative) ? "Yes" : "No");
    // printf("fractional: %d\n", fractional);
    // printf("output: %s\n", output);

    return output;
}