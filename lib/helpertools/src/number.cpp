#include <stdio.h>


/* Convert a float into a string with two decimal positions. 

   E.g.  21.3413 will become "32.34"

   I'm doing this to avoid use of Arduino specific code so I can test on board 
   and native. My version is better in that I only want 2 decimal point 
   precision and I don't want padded 0's. 
   
   The end user will have to make sure the output buffer is big enough or the 
   number will be truncated strangely. I don't check for this.

   Interesting read on how on floating point operations:
   
   https://www.exploringbinary.com/
        why-0-point-1-does-not-exist-in-floating-point/

    https://www.exploringbinary.com/
        floating-point-questions-are-endless-on-stackoverflow-com/

*/
char * decimalToString(char *output, int output_size, float number) {
    // e.g. number: 12.1 recover just the integrer part: 12
    int integer = (int)(number);

    // recover fractional part without having to worry about floating point 
    // represention issues
    // e.g. number: 12.11 fractional part: 1211 - 1200 = 11
    int fractional = (int)((number * 100) - (integer * 100));
    if (fractional == 10) {
        // e.g. number: 12.1 
        // I don't want the trailing 0 on we would get in the output 12.10
        // So fractional part / 10 = 1
        fractional = fractional / 10;
    }
 
    // safely output a string represention of the number with no trailing or 
    // leading zeros.
    snprintf(output, output_size, "%d.%d", integer, fractional);

    return output;
}