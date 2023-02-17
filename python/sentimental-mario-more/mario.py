def main():
    height = get_height()
    for row in range(height):
        for col in range(height):
            if col >= height - row - 1:
                print('#', end='')
            else:
                print(' ', end='')
        print(f"  {'#' * (row + 1)}")


# Continually prompts user for height until it receives an integer between 1 and 8, which it then returns
def get_height():
    valid_input = False
    while not valid_input:
        height = input("Height: ")
        if not height.isdigit():
            print("Please input an integer between 1 and 8")
        elif int(height) not in range(1, 9):
            print("Please input an integer between 1 and 8")
        else:
            valid_input = True
    return int(height)


if __name__ == "__main__":
    main()