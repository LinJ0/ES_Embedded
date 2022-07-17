#include <stdio.h>

int int_1w1 = 111;
int int_1w2 = 222;
int int_1w3 = 333;
int int_1w4 = 444;
int int_1w5 = 555;
float float_1w = 1111.2222;
double double_2w = 3333.4444;


int callee_int(int i1, int i2, int i3, int i4, int i5)
{
    return (i1 + i2 + i3 + i4 + i5);
}
float callee_float(float f){return f;}
double callee_double(double d){return d;}


void reset_handler(void)
{
    int temp1;
    float temp2;
    double temp3;

    temp1 = callee_int(int_1w1, int_1w2, int_1w3, int_1w4, int_1w5);
    temp2 = callee_float(float_1w);
    temp3 = callee_double(double_2w);
    while (1);
}
