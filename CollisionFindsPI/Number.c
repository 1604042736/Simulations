#include <math.h>
#include <string.h>

#include "Number.h"

size_t fkeep_size = 4;
Number *number_0;
Number *number_1;
Number *number_2;
Number *number_10;
Number *number_digit_base;

void Number_Init()
{
    number_0 = Number_NewFromInt32(0);
    number_1 = Number_NewFromInt32(1);
    number_2 = Number_NewFromInt32(2);
    number_10 = Number_NewFromInt32(10);
    number_digit_base = Number_NewFromInt32(DIGIT_BASE);
}

/*
通过指定size,fsize,sign来创建一个Number
*/
Number *Number_New(size_t size, size_t fsize, sign_t sign)
{
    Number *self = (Number *)malloc(sizeof(Number));
    self->size = size;
    self->fsize = fsize;
    self->sign = sign;
    self->digit = (digit_t *)calloc(sizeof(digit_t), self->size);
    return self;
}

/*
通过int32创建Number
*/
Number *Number_NewFromInt32(int v)
{
    size_t i;
    sign_t sign = SIGN_POSITIVE;
    Number *self;
    if (v < 0)
        sign = SIGN_NEGATIVE, v = -v;
    else if (v == 0)
    {
        self = Number_New(1, 0, SIGN_POSITIVE);
        return self;
    }
    self = Number_New(log(v) / log(DIGIT_BASE) + 1, 0, sign);
    for (i = 0; v > 0; i++)
    {
        self->digit[i] = v % DIGIT_BASE;
        v /= DIGIT_BASE;
    }
    return self;
}

/*
复制
*/
Number *Number_NewFromNumber(Number *self)
{
    size_t i;
    Number *result = Number_New(self->size, self->fsize, self->sign);
    for (i = 0; i < self->size; i++)
        result->digit[i] = self->digit[i];
    return result;
}

void Number_Del(Number *self)
{
    free(self->digit);
    free(self);
}

/*
去除收尾的0
*/
Number *stripzero(Number *a)
{
    size_t i = a->size - 1, j;
    while (i > a->fsize && a->digit[i] == 0)
        i--;
    a->size = i + 1;
    i = 0;
    while (i < a->fsize && a->digit[i] == 0)
        i++;
    for (j = i; j < a->size; j++)
        a->digit[j - i] = a->digit[j];
    a->fsize -= i;
    a->size -= i;
    return a;
}

/*
通过字符串创建Number
*/
Number *Number_NewFromString(char *str)
{
    size_t i = 0, p, j, k, fsize, isize = 0;
    digit_t d;
    Number *a;
    sign_t sign = SIGN_POSITIVE;
    if (str[i] == '-')
        sign = SIGN_NEGATIVE, i++;
    while (i < strlen(str) && str[i] != '.')
        i++, isize++;
    p = i;
    fsize = (strlen(str) - p) / DIGIT_BASE_NUM + 1;
    a = Number_New(isize / DIGIT_BASE_NUM + 1 + fsize, fsize, sign);
    k = fsize;
    for (; i > DIGIT_BASE_NUM; i -= DIGIT_BASE_NUM)
    {
        d = 0;
        for (j = i - DIGIT_BASE_NUM; j < i; j++)
            d = d * 10 + str[j] - '0';
        a->digit[k++] = d;
    }
    j = sign == SIGN_NEGATIVE ? 1 : 0;
    d = 0;
    for (; j < i; j++)
        d = d * 10 + str[j] - '0';
    a->digit[k++] = d;
    k = fsize - 1;
    for (i = p + 1; i < strlen(str); i += DIGIT_BASE_NUM)
    {
        d = 0;
        for (j = i; j < i + DIGIT_BASE_NUM; j++)
        {
            if (j < strlen(str))
                d = d * 10 + str[j] - '0';
            else
                d = d * 10;
        }
        a->digit[k--] = d;
    }
    return stripzero(a);
}

/*
比较两个数的绝对值大小
*/
compare_t abs_compare(const Number *a, const Number *b)
{
    size_t iasize, ibsize, i, j;
    iasize = a->size - a->fsize;
    ibsize = b->size - b->fsize;

    if (iasize > ibsize)
        return COMPARE_GT;
    else if (iasize < ibsize)
        return COMPARE_LT;
    for (i = a->size - 1, j = b->size - 1; i > a->fsize && j > b->fsize; i--, j--)
        if (a->digit[i] != b->digit[j])
            break;
    if (a->digit[i] > b->digit[j])
        return COMPARE_GT;
    else if (a->digit[i] < b->digit[j])
        return COMPARE_LT;

    if (a->fsize > 0 && b->fsize == 0)
        return COMPARE_GT;
    else if (a->fsize == 0 && b->fsize > 0)
        return COMPARE_LT;
    else if (a->fsize == 0 && b->fsize == 0)
        return COMPARE_EQ;
    for (i = a->fsize - 1, j = b->fsize - 1; i > 0 && j > 0; i--, j--)
        if (a->digit[i] != b->digit[j])
            break;
    if (i > 0)
        return COMPARE_GT;
    else if (j > 0)
        return COMPARE_LT;
    if (a->digit[i] > b->digit[j])
        return COMPARE_GT;
    else if (a->digit[i] < b->digit[j])
        return COMPARE_LT;

    return COMPARE_EQ;
}

/*
计算两数绝对值的和
*/
Number *abs_add(Number *a, Number *b)
{
    size_t size, fsize, i, shift, bsize;
    digit_t carry = 0;
    Number *c;

    if (a->fsize < b->fsize) // 保证a的fsize是最大的
    {
        Number *temp = a;
        a = b;
        b = temp;
    }
    fsize = a->fsize;
    size = MAX(a->size - a->fsize, b->size - b->fsize) + fsize + 1; // 预留一位进位

    c = Number_New(size, fsize, SIGN_POSITIVE);
    shift = a->fsize - b->fsize;
    bsize = b->size + shift;
    for (i = shift; i < bsize; i++) // 先把b复制到c
        c->digit[i] = b->digit[i - shift];

    for (i = 0; i < a->size; i++)
    {
        c->digit[i] += a->digit[i] + carry;
        carry = c->digit[i] / DIGIT_BASE;
        c->digit[i] %= DIGIT_BASE;
    }
    c->digit[i] += carry;
    while (c->digit[i] > DIGIT_BASE)
    {
        c->digit[i + 1] += c->digit[i] / DIGIT_BASE;
        c->digit[i] %= DIGIT_BASE;
        i++;
    }
    return stripzero(c);
}

/*
计算两数绝对值的差
*/
Number *abs_sub(Number *a, Number *b)
{
    size_t size, fsize, i, shift, asize, bsize;
    compare_t j;
    sign_t sign;
    digit_t borrow = 0;
    Number *c;

    fsize = MAX(a->fsize, b->fsize);
    size = MAX(a->size - a->fsize, b->size - b->fsize) + fsize;
    j = abs_compare(a, b);
    if (j == COMPARE_GT)
        sign = SIGN_POSITIVE;
    else if (j == COMPARE_EQ)
    {
        c = Number_New(1, 0, SIGN_POSITIVE);
        return c;
    }
    else if (j == COMPARE_LT)
    {
        Number *temp = a;
        a = b;
        b = temp;
        sign = SIGN_NEGATIVE;
    }

    c = Number_New(size, fsize, sign);
    shift = a->fsize >= b->fsize ? 0 : b->fsize - a->fsize;
    asize = a->size + shift;
    for (i = shift; i < asize; i++)
        c->digit[i] = a->digit[i - shift];

    shift = shift == 0 ? a->fsize - b->fsize : 0;
    bsize = b->size + shift;
    for (i = shift; i < bsize; i++)
    {
        digit_t x = c->digit[i], y = b->digit[i - shift];
        if (x < y + borrow)
            c->digit[i] = DIGIT_BASE + x - y - borrow, borrow = 1;
        else
            c->digit[i] = x - y - borrow, borrow = 0;
    }

    while (borrow == 1)
    {
        digit_t x = c->digit[i];
        if (x < borrow)
            c->digit[i] = DIGIT_BASE + x - borrow, borrow = 1;
        else
            c->digit[i] = x - borrow, borrow = 0;
    }
    return stripzero(c);
}

/*
计算两数绝对值之积
*/
Number *abs_mul(Number *a, Number *b)
{
    size_t size, fsize, i, j;
    Number *c;
    size = a->size + b->size + 1;
    fsize = a->fsize + b->fsize;
    c = Number_New(size, fsize, SIGN_POSITIVE);
    for (i = 0; i < a->size; i++)
    {
        for (j = 0; j < b->size; j++)
        {
            unsigned int t = c->digit[i + j] + a->digit[i] * b->digit[j]; // 防溢出
            c->digit[i + j + 1] += t / DIGIT_BASE;
            c->digit[i + j] = t % DIGIT_BASE;
        }
    }
    return stripzero(c);
}

/*
计算a的|b|次方
*/
Number *abs_pow(Number *a, Number *b)
{
    Number *c;
    if (b->fsize != 0) // TODO暂不支持小数次方
        return NULL;
    c = Number_NewFromInt32(1);
    b = Number_NewFromNumber(b);
    while (abs_compare(b, number_0) == COMPARE_GT)
    {
        ASSIGN_AUTO_FREE_2(Number_Mul, c, a);
        ASSIGN_AUTO_FREE_2(abs_sub, b, number_1);
    }

    Number_Del(b);
    return stripzero(c);
}

/*
两数绝对值的商和余数
*/
Number **abs_divmod(Number *a, Number *b)
{
    Number *c = Number_NewFromInt32(0);
    Number **result;
    a = Number_NewFromNumber(a); // 同abs_pow
    while (!(abs_compare(a, b) == COMPARE_LT))
    {
        ASSIGN_AUTO_FREE_2(abs_sub, a, b);
        ASSIGN_AUTO_FREE_2(abs_add, c, number_1);
    }
    result = (Number **)malloc(sizeof(Number *) * 2);
    result[0] = c; // 商
    result[1] = a; // 余数
    return result;
}

/*
计算两数绝对值之商
*/
Number *abs_div(Number *a, Number *b)
{
    size_t i;
    compare_t p;
    Number *c;
    Number *k = Number_NewFromInt32(b->fsize);
    Number **dm;
    if (b->size == 1 && b->fsize == 0 && b->digit[0] == 0) // TODO除数不能为0
        return NULL;
    // 去除除数的小数点
    k = abs_pow(number_10, k);
    a = abs_mul(a, k);
    b = abs_mul(b, k);
    Number_Del(k);

    c = Number_New(a->size + fkeep_size, fkeep_size, SIGN_POSITIVE);
    // 计算整数部分的值
    dm = abs_divmod(a, b);
    ASSIGN_AUTO_FREE_2(abs_add, c, dm[0]);
    Number_Del(a);
    a = dm[1];

    p = abs_compare(a, number_0);
    for (i = 0; i < fkeep_size && p != COMPARE_EQ; i++)
    {
        size_t j;
        ASSIGN_AUTO_FREE_2(abs_mul, a, number_digit_base);
        Number_Del(dm[0]);
        free(dm);

        dm = abs_divmod(a, b);
        k = Number_New(i + 2, i + 1, SIGN_POSITIVE);
        for (j = 0; j < dm[0]->size; j++)
            k->digit[j] = dm[0]->digit[j];
        ASSIGN_AUTO_FREE_2(abs_add, c, k);
        Number_Del(k);
        Number_Del(a);
        a = dm[1];
        p = abs_compare(a, number_0);
    }

    Number_Del(dm[0]);
    Number_Del(dm[1]);
    free(dm);
    // 在去除小数点后a,b就不是原来的a,b了
    Number_Del(b);
    return stripzero(c);
}

Number *Number_Add(Number *self, Number *other)
{
    Number *result;
    if (self->sign == SIGN_POSITIVE)
    {
        if (other->sign == SIGN_POSITIVE)
            result = abs_add(self, other);
        else if (other->sign == SIGN_NEGATIVE)
            result = abs_sub(self, other);
    }
    else if (self->sign == SIGN_NEGATIVE)
    {
        if (other->sign == SIGN_POSITIVE)
            result = abs_sub(other, self);
        else if (other->sign == SIGN_NEGATIVE)
        {
            result = abs_add(self, other);
            result->sign = SIGN_NEGATIVE;
        }
    }
    return result;
}

Number *Number_Sub(Number *self, Number *other)
{
    Number *result;
    if (self->sign == SIGN_POSITIVE)
    {
        if (other->sign == SIGN_POSITIVE)
            result = abs_sub(self, other);
        else if (other->sign == SIGN_NEGATIVE)
            result = abs_add(self, other);
    }
    else if (self->sign == SIGN_NEGATIVE)
    {
        if (other->sign == SIGN_POSITIVE)
        {
            result = abs_add(self, other);
            result->sign = SIGN_NEGATIVE;
        }
        else if (other->sign == SIGN_NEGATIVE)
            result = abs_sub(other, self);
    }
    return result;
}

Number *Number_Mul(Number *self, Number *other)
{
    Number *result;
    result = abs_mul(self, other);
    if (self->sign != other->sign)
        result->sign = SIGN_NEGATIVE;
    return result;
}

Number *Number_Div(Number *self, Number *other)
{
    Number *result;
    result = abs_div(self, other);
    if (result == NULL)
        return NULL;
    if (self->sign != other->sign)
        result->sign = SIGN_NEGATIVE;
    return result;
}

Number *Number_Pow(Number *self, Number *other)
{
    Number *result;
    result = abs_pow(self, other);
    if (result == NULL)
        return NULL;
    if (other->sign == SIGN_NEGATIVE)
    {
        Number *t = result;
        result = abs_div(number_1, t);
        free(t);
    } // TODO没有判断self为负数的情况
    return result;
}

compare_t Number_Compare(Number *self, Number *other)
{
    if (self->sign == SIGN_POSITIVE)
    {
        if (other->sign == SIGN_NEGATIVE)
            return COMPARE_GT;
        else if (other->sign == SIGN_POSITIVE)
            return abs_compare(self, other);
    }
    else if (self->sign == SIGN_NEGATIVE)
    {
        if (other->sign == SIGN_POSITIVE)
            return COMPARE_LT;
        else if (other->sign == SIGN_NEGATIVE)
            return -abs_compare(self, other); // 负数绝对值大小与它们本身大小相反
    }
}