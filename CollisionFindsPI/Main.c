#include <stdio.h>

#include "Number.h"

int n;
Number *m1, *m2;
Number *x1, *x2;
Number *v1, *v2;
Number *dt;

int main()
{
    Number_Init();

    m1 = Number_NewFromInt32(1);
    m2 = Number_NewFromInt32(100);
    v1 = Number_NewFromInt32(0);
    v2 = Number_NewFromInt32(-1);
    x1 = Number_NewFromInt32(2);
    x2 = Number_NewFromInt32(4);
    dt = Number_NewFromString("0.0001");

    Number *sm = Number_Add(m1, m2);  // m1+m2
    Number *dm1 = Number_Sub(m1, m2); // m1-m2
    Number *dm2 = Number_Sub(m2, m1); // m2-m1
    for (; !(Number_Compare(v2, v1) == COMPARE_GT && Number_Compare(v1, number_0) == COMPARE_GT);)
    {
        Number *dx1 = Number_Mul(v1, dt);
        Number *dx2 = Number_Mul(v2, dt);
        ASSIGN_AUTO_FREE_2(Number_Add, x1, dx1);
        ASSIGN_AUTO_FREE_2(Number_Add, x2, dx2);
        if (Number_Compare(x1, x2) == COMPARE_GT) // 发生碰撞
        {
            Number *t1_1 = Number_Mul(dm1, v1);        //(m1-m2)*v1
            Number *t2_1 = Number_Mul(dm2, v2);        //(m2-m1)*v2
            Number *t1_2 = Number_Mul(m2, v2);         // m2*v2
            Number *t2_2 = Number_Mul(m1, v1);         // m1*v1
            Number *t1_3 = Number_Mul(number_2, t1_2); // 2*m2*v2
            Number *t2_3 = Number_Mul(number_2, t2_2); // 2*m1*v1
            Number *t1_4 = Number_Add(t1_1, t1_3);     //(m1-m2)*v1+2*m2*v2
            Number *t2_4 = Number_Add(t2_1, t2_3);     //(m2-m1)*v2+2*m1*v1
            Number_Del(v1);
            Number_Del(v2);
            v1 = Number_Div(t1_4, sm);
            v2 = Number_Div(t2_4, sm);
            Number_Del(t1_1);
            Number_Del(t1_2);
            Number_Del(t1_3);
            Number_Del(t1_4);
            Number_Del(t2_1);
            Number_Del(t2_2);
            Number_Del(t2_3);
            Number_Del(t2_4);
            n++;
        }
        if (Number_Compare(x1, number_1) == COMPARE_LT) // 碰到墙
        {
            v1->sign = 1 - v1->sign;
            n++;
        }
        Number_Del(dx1);
        Number_Del(dx2);
    }
    QUICK_PRINT(m2);
    printf(" %d\n", n);
}