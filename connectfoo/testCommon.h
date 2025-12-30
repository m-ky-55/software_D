#ifndef __TEST_COMMON_H
#define __TEST_COMMON_H

#include <assert.h>
#include <math.h>
#include <stdlib.h>

#define message(m) fprintf(stderr, (m))
#define message1(m, a) fprintf(stderr, (m), (a))
#define message2(m, a, b) fprintf(stderr, (m), (a), (b))
#define message3(m, a, b, c) fprintf(stderr, (m), (a), (b), (c))
#define message4(m, a, b, c, d) fprintf(stderr, (m), (a), (b), (c), (d))
#define message6(m, a, b, c, d, e, f) \
    fprintf(stderr, (m), (a), (b), (c), (d), (e), (f))
#define message8(m, a, b, c, d, e, f, g, h) \
    fprintf(stderr, (m), (a), (b), (c), (d), (e), (f), (g), (h))
#define messend1(m, a)      \
    do {                    \
        message1((m), (a)); \
        existErrorCount++;  \
    } while (0)
#define messend2(m, a, b)        \
    do {                         \
        message2((m), (a), (b)); \
        existErrorCount++;       \
    } while (0)
#define messend3(m, a, b, c)          \
    do {                              \
        message3((m), (a), (b), (c)); \
        existErrorCount++;            \
    } while (0)
#define messend4(m, a, b, c, d)            \
    do {                                   \
        message4((m), (a), (b), (c), (d)); \
        existErrorCount++;                 \
    } while (0)
#define messend6(m, a, b, c, d, e, f)                \
    do {                                             \
        message6((m), (a), (b), (c), (d), (e), (f)); \
        existErrorCount++;                           \
    } while (0)
#define messend8(m, a, b, c, d, e, f, g, h)                    \
    do {                                                       \
        message8((m), (a), (b), (c), (d), (e), (f), (g), (h)); \
        existErrorCount++;                                     \
    } while (0)

#define DELTA 1e-6

void testStart(char* mes);
void testErrorCheck(void);
void assertEqualsIntFunc(int a, int b, char* fname, int line);
void assertNotEqualsIntFunc(int a, int b, char* fname, int line);
void assertEqualsFloatFunc(float a, float b, char* fname, int line);
void assertEqualsDoubleFunc(double a, double b, char* fname, int line);
void assertEqualsIntArrayFunc(int* ap, int* bp, int n, char* fname, int line);
void assertEqualsStringFunc(char* ap, char* bp, char* fname, int line);

#define assertEqualsInt(a, b) assertEqualsIntFunc(a, b, __FILE__, __LINE__)
#define assertNotEqualsInt(a, b) \
    assertNotEqualsIntFunc(a, b, __FILE__, __LINE__)
#define assertEqualsFloat(a, b) assertEqualsFloatFunc(a, b, __FILE__, __LINE__)
#define assertEqualsDouble(a, b) \
    assertEqualsDoubleFunc(a, b, __FILE__, __LINE__)
#define assertEqualsIntArray(a, b, n) \
    assertEqualsIntArrayFunc((int*)a, (int*)b, n, __FILE__, __LINE__)
#define assertEqualsString(a, b) \
    assertEqualsStringFunc(a, b, __FILE__, __LINE__)

#ifdef __COMPLEX_H__
void assertEqualsComplexFunc(complex a, complex b, char* fname, int line);
#define assertEqualsComplex(a, b) \
    assertEqualsComplexFunc(a, b, __FILE__, __LINE__)
#endif
#endif
