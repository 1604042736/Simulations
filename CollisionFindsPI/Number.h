#ifndef __NUMBER_H__
#define __NUMBER_H__

#include <stdlib.h>
#include <stdio.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))

#define DIGIT_BASE 10000
#define DIGIT_BASE_NUM 4

#define SIGN_POSITIVE 1
#define SIGN_NEGATIVE 0

#define COMPARE_EQ 0
#define COMPARE_GT 1
#define COMPARE_LT -1

#define ASSIGN_AUTO_FREE_2(func, a, b) \
    {                                  \
        Number *t = a;                 \
        a = func(t, b);                \
        Number_Del(t);                 \
    }

#define QUICK_PRINT(c)                    \
    {                                     \
        size_t i;                         \
        if (c->sign == SIGN_NEGATIVE)     \
            printf("-");                  \
        for (i = c->size - 1; i > 0; i--) \
        {                                 \
            printf("%04d", c->digit[i]);  \
            if (i == c->fsize)            \
                printf(".");              \
        }                                 \
        printf("%04d", c->digit[i]);      \
    }

typedef char compare_t;
typedef unsigned short digit_t; // 每位数字的类型
typedef unsigned char sign_t;   // 符号的类型
typedef struct
{
    digit_t *digit; // 数字
    size_t size;    // 总长度
    size_t fsize;   // 小数部分长度(在这里指小数点后面数字的长度)
    sign_t sign;    // 符号
} Number;

extern size_t fkeep_size; // 保留的小数部分大小
extern Number *number_0;
extern Number *number_1;
extern Number *number_2;
extern Number *number_10;
extern Number *number_digit_base;

void Number_Init();
Number *Number_New(size_t, size_t, sign_t);
Number *Number_NewFromInt32(int);
Number *Number_NewFromString(char *);
Number *Number_NewFromNumber(Number *);
void Number_Del(Number *);
Number *Number_Add(Number *, Number *);
Number *Number_Sub(Number *, Number *);
Number *Number_Mul(Number *, Number *);
Number *Number_Div(Number *, Number *);
Number *Number_Pow(Number *, Number *);
compare_t Number_Compare(Number *, Number *);

#endif