#include <stdio.h>
#include <stdlib.h>

#include <string.h>

typedef struct T_Data
{
    long val1;
    long val2;
} T_DATA;

T_DATA *ApplyRules(int rock)
{
    T_DATA outData;
    T_DATA *out;
    outData.val2 = -1;
    if (rock == 0)
    {
        outData.val1 = 0;
    }
    else if (rock > 9)
    {
        char rockStr[10];
        sprintf(rockStr, "%d", rock);
        int rockStrLen = strlen(rockStr);
        if (rockStrLen % 2 == 0)
        {
            outData.val1 = 5;
            outData.val2 = 5;
        }
    }
    else
    {
        outData.val1 = 2024 * rock;
    }
    out = malloc(sizeof(T_DATA));
    *out = outData;
    return out;
}