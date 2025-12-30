class Credit():
    #MASTERCARDS 51, 52, 53, 54, or 55
    #AMEX 34 or 37

    card_number = input("Enter your credit card number: ")
    mastercards = [51, 52, 53, 54, 55]
    amexes = [34, 37]
    visas = [4]

    sum = 0
    digits = 0

    try:
        for i in range(len(card_number) - 1, 0):#every other *2 starting w/ second to last digit
            if digits % 2 == 0:#if it is even
                value = 2 * int(card_number[i])
            else:
                value = int(card_number[i])
            digits += 1
            sum += value
        if sum % 10 == 0:
            prefix = int(card_number[0:2])
            if(int(card_number[0]) == 4):
                print("Visa")
            elif prefix in mastercards:
                print("Mastercard")
            elif prefix in amexes:
                print("Amex")
            else:
                print("Card number not visa or mastercard or amex")
        else:
            print("Card number invalid. Must fit algorithm")

    except ValueError:
        print("Card number invalid. Must be integer")
