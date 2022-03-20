# TODO

from cs50 import get_string
from sys import exit

# Get card number as string
cardnumber = get_string("Number: ")
# Identify number of digits in card number
cardlength = len(cardnumber)

# Validate number of digits in card number
if cardlength != 13 and cardlength != 15 and cardlength != 16:
    print("INVALID")
    exit()

# Validate checksum
sum1, sum2 = 0, 0
x = cardnumber
y = cardlength

# 1 multiply every other digit by 2, start from the second last digit, get the products, sum up the digits, let it be [sum1]
# 2 sum up the digits that weren't multiplied by 2, let it be [sum2]
# 3 sum up [sum1] and [sum2], let it be [checksum]
# 4 if the last digit of [checksum] is 0, the number is valid

if y % 2 == 0:
    for i in range(y):
        num = int(x[i])
        if i % 2 == 0:
            multiple = num * 2
            if multiple >= 10:
                sum1 += multiple // 10
                sum1 += multiple % 10
            else:
                sum1 += multiple
        else:
            sum2 += num
else:
    for i in range(y):
        num = int(x[i])
        if i % 2 != 0:
            multiple = num * 2
            if multiple >= 10:
                sum1 += multiple // 10
                sum1 += multiple % 10
            else:
                sum1 += multiple
        else:
            sum2 += num

checksum = (sum1 + sum2) % 10

# Identify card type
if checksum == 0:
    # Get first and second digits
    first = int(cardnumber[0])
    second = int(cardnumber[1])
    # AMEX
    if first == 3 and (second == 4 or second == 7):
        print("AMEX")
    # VISA
    elif first == 4:
        print("VISA")
    # MASTERCARD
    elif first == 5 and second > 0 and second < 6:
        print("MASTERCARD")
    else:
        print("INVALID")
else:
    print("INVALID")