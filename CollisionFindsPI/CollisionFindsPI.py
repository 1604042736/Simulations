m1 = 1
m2 = 100
while True:
    n = 0
    v1, x1 = 0, 2
    v2, x2 = -1, 3
    while True:
        if v2 > v1 > 0:
            break
        dt = 0.0001
        x1 += v1 * dt
        x2 += v2 * dt
        if x1 > x2:  # 发生碰撞
            v1_ = ((m1 - m2) * v1 + 2 * m2 * v2) / (m1 + m2)
            v2_ = ((m2 - m1) * v2 + 2 * m1 * v1) / (m1 + m2)
            v1 = v1_
            v2 = v2_
            n += 1
        if x1 < 1:  # 碰到墙
            v1 = -v1
            n += 1
    print(m2, n)
    m2 *= 100
