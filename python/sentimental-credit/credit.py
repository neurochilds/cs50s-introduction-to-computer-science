import sys

"""
This program determines if a credit card number is valid using the algorithm described in words below:
1. Multiply every other digit by 2, starting with the number’s second-to-last digit, and then add those products’ digits together.
2. Add the sum to the sum of the digits that weren’t multiplied by 2.
3. If the total’s last digit is 0 (or, put more formally, if the total modulo 10 is congruent to 0), the number is valid!
"""


def main():
    card_number = input("Card number: ")
    if not card_number.isdigit():
        print("INVALID")
        sys.exit()

    first_digit = int(card_number[0])
    first_two_digits = int(card_number[:2])
    card_length = len(card_number)

    checksum = 0
    index = -1
    for i in range(card_length):
        if index % -2 == 0:
            numb = str(int(card_number[index]) * 2) # Multiply every other digit by 2, starting with the number’s second-to-last digit
            for i in range(len(numb)): # And then add those products digits together
                checksum += int(numb[i]) # And add this to the global sum

        else:
            checksum += int(card_number[index]) # Add all digits that weren't multiplied by to the global sum

        index -= 1 # Move to the next digit along, starting from the last digit and finishing with the first

    is_valid = checksum % 10 == 0 # If the total's last digit is 0, the number is valid

    amex = [34, 37] # Amex cards start with these numbers
    mastc = [51, 52, 53, 54, 55] # Mastercards start with these numbers

    if is_valid:
        if first_digit == 4 and (card_length == 13 or card_length == 16):
            print('VISA')

        elif first_two_digits in amex and card_length == 15:
            print('AMEX')

        elif first_two_digits in mastc and card_length == 16:
            print('MASTERCARD')

        else:
            print('INVALID')
    else:
        print('INVALID')


if __name__ == "__main__":
    main()