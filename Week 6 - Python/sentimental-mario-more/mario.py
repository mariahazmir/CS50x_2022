# TODO

n = 0


def main():
    # If user input valid integer
    try:
        while True:
            # Get height
            height = int(input("Height: "))
            # Exit loop if height is between 1 to 8
            if height > 0 and height < 9:
                break
    # If user input invalid integer
    except ValueError:
        main()

    n = height

    # Print pyramid
    for i in range(n):
        # Print space from left edge
        print(" " * (n - i - 1), end="")
        # Print left pyramid blocks
        print("#" * (i + 1), end="")
        # Print gap between left and right pyramids
        print("  ", end="")
        # Print right pyramid blocks
        print("#" * (i + 1))


main()
