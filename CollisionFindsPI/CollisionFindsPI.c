#include <stdio.h>

long long m1 = 1, m2 = 100;
double dt = 0.0001;

int main()
{
    while (1)
    {
        long long n = 0;
        double x1 = 2, x2 = 3;
        double v1 = 0, v2 = -1;
        for (; !(v2 > v1 && v1 > 0);)
        {
            x1 += v1 * dt;
            x2 += v2 * dt;
            if (x1 > x2)
            {
                double v1_ = ((m1 - m2) * v1 + 2 * m2 * v2) / (m1 + m2);
                double v2_ = ((m2 - m1) * v2 + 2 * m1 * v1) / (m1 + m2);
                v1 = v1_;
                v2 = v2_;
                n += 1;
            }
            if (x1 < 1)
            {
                v1 = -v1;
                n += 1;
            }
        }
        printf("%lld %lld\n", m2, n);
        m2 *= 100;
    }
}