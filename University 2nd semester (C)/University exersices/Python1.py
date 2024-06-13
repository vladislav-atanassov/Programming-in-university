import random 

n = int(input("How many?: "))

a = [ random.randint(1, 100) for _ in range(n)]

print(a)