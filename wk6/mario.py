
class Mario():
    try:
        height = int(input("Enter a height: "))

        if height > 0 and height <= 8:
            for i in range(height):
                to_print = ""
                #spaces
                to_print += " " * (height - i - 1)
                # #
                to_print +=  "#" * (i + 1)
                #space
                to_print += " "
                #height #
                to_print += "#" * (i + 1)
                #height - 1 spaces
                to_print += " " * (height - i - 1)

                print (to_print)
        else:
            print("Enter a height 1-8")
    except ValueError:
        print("Enter an integer")


