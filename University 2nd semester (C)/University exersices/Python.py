def f(number):
    y = number**2 - 4
    print(round(y, 2))


num_1, num_2 = input("Enter numbers: ").split()
num_1, num_2 = float(num_1), float(num_2)

if num_2 < num_1:
    num_1, num_2 = num_2, num_1

x = num_1 + 1

while x < num_2:
    x += 0.01
    f(x)