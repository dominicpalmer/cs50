import cs50


condition = True
height = 0
while condition:
    print("Height: ")
    height = cs50.get_int()
    condition = height < 0 or height > 23

for i in range(height):
    print(" " * (height - 1 - i), end = "")
    print("#" * (i + 1), end = "")
    print("  ", end = "")
    print("#" * (i + 1), end = "\n")