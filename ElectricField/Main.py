import matplotlib.pyplot as plt
from matplotlib.colors import ListedColormap
import numpy as np

charge_points = [
    (10, 10, 100),
    (90, 90, 100),
    (10, 90, 100),
    (90, 10, 100),
]  # 点电荷信息
x_interval, y_interval = (0, 200), (0, 200)  # 坐标区间
k = 8.987551e9


def get_strength():
    """获取电场强度"""
    w = x_interval[1] - x_interval[0]  # 宽度
    h = y_interval[1] - y_interval[0]  # 高度
    strength = [[np.array([0.0, 0.0]) for _ in range(w)] for _ in range(h)]

    for px, py, q in charge_points:
        for i in range(w):
            for j in range(h):
                x = x_interval[0] + i
                y = y_interval[0] + j
                if x == px and y == py:
                    continue
                r = np.array([x, y]) - np.array([px, py])
                r_square = r[0] ** 2 + r[1] ** 2
                strength[i][j] += k * q / r_square * (r / np.sqrt(r_square))
    return strength


def show_magnitude():
    """显示电场强度大小"""
    strength = get_strength()
    magnitude = []  # 所有大小
    w = len(strength[0])
    h = len(strength)
    for i in range(w):
        for j in range(h):
            x, y = strength[i][j]
            strength[i][j] = np.sqrt(x**2 + y**2)
            magnitude.append(strength[i][j])
    magnitude.sort(reverse=True)
    colors = {}
    for i, v in enumerate(magnitude):
        colors[v] = "#" + hex(i).replace("0x", "").ljust(6, "0")
    cmap = ListedColormap(
        ["#" + hex(i).replace("0x", "").ljust(6, "0") for i in range(len(magnitude))]
    )
    plt.imshow(strength, cmap=cmap)
    plt.show()


show_magnitude()
