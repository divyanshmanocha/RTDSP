from math import sqrt
import matplotlib.pyplot as plt


a0 = sqrt(2)
b0 = 1/sqrt(2)

y = [0, 0, 0]
x = [1]

result = [0]

for i in range(0, 8):
    y[0] = a0 * y[1] - y[2] + b0 * x[0]
    result.append(y[0])
    y[2] = y[1]
    y[1] = y[0]
    x[0] = 0

plt.plot(result)
plt.title("Filter Output")
plt.ylabel("y: sin(x)")
plt.xlabel("x: sample")
plt.show()
