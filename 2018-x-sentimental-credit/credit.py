import math

def luhn_validator(x):
    #calculate length of the card number
    card_length = math.floor(math.log10(x)) + 1

    #if length of card number not equal to 13, 15 or 16, print INVALID and exit program, otherwise carry on
    if card_length != 13 and card_length != 15 and card_length != 16:
        print("INVALID\n")
        exit(0)

    #send card number digits in reverse to an array
    card_number_array = []
    while x != 0:
        card_number_array.append(x % 10)
        x = x // 10

    #store first digit of card_number_array as first_digit
    first_digit = card_number_array[card_length - 1]

    #sum odd elements of card_number_array
    sum1 = 0
    n = 0
    while n < card_length:
        sum1 = sum1 + card_number_array[n]
        n = n + 2

    #sum 2*evenelements of card_number_array
    sum2 = 0
    temp_array = []
    u = 1
    while u < card_length:
        j = 2 * card_number_array[u]
        if j >= 10:
            first = j % 10
            j = j // 10
            sum2 = sum2 + first + j
        else:
            sum2 = sum2 + j
        u = u + 2

    #total sum ready for validation
    total_sum = sum1 + sum2

    #check if total_sum mod 10 is 0
    if total_sum % 10 == 0:
        return 0
    else:
        return 1

def determine_issuer(x, y):
    if x == 3 and y == 15:
        print("AMEX")
    elif x == 5 and (y == 16):
        print("MASTERCARD")
    elif x == 4 and (y == 13 or y == 16):
        print("VISA")
    else:
        print("INVALID")

def main():
    #request card number
    card_number = 0
    while card_number < 0 or card_number == 0:
        print("Number: ", end = "")
        card_number = (int)(input())
    temp = card_number
    cl = math.floor(math.log10(card_number)) + 1

    #send card number digits in reverse to an array
    card_number_array1 = []
    while temp != 0:
        card_number_array1.append(temp%10)
        temp = temp // 10

    #store first digit of card_number_array as first_digit
    first_digit = card_number_array1[cl-1]
    #run the luhn_validator, then determine the issuer if required
    if luhn_validator(card_number) == 1:
        print("INVALID\n")
        exit(0)
    else:
        determine_issuer(first_digit, cl)
    return 0

if __name__ == "__main__":
    main()
