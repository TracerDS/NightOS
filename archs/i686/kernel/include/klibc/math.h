#ifndef __KLIBC_MATH_H__
#define __KLIBC_MATH_H__

float fabsf(float num);
double fabs(double num);
long double fabsl(long double num);
float fmodf(float x, float y);
double fmod(double x, double y);
long double fmodl(long double x, long double y);
float powf(float base, float exponent);
double pow(double base, double exponent);
long double powl(long double base, long double exponent);

float exp2f(float n);
double exp2(double n);

float frexp(float num, int* exp);
#endif