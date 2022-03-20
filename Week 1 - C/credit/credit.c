#include <cs50.h>
#include <stdio.h>

int main(void)

{

    //n is card number
    //i is number of digits
    //long x is derived card number

    //A Get the card number

    long n = get_long("Number: ");

    //B Count the number of digits

    int i = 0;
    long x = n;
    while (x > 0)
    {
        x = x / 10;
        i++;
    }

    //C Check validity of number of digits

    //If number of digits is not valid
    if (i != 13 && i != 15 && i != 16)
    {
        printf("INVALID\n");
        return 0;
    }

    //Checksum calculation steps
    /*
    1 multiply every other digit by 2, start from the second last digit, get the products, sum up the digits, let it be [sum1]
    2 sum up the digits that weren't multiplied by 2, let it be [sum2]
    3 sum up [sum1] and [sum2], let it be [tot]
    4 if the last digit of [tot] is 0, the number is valid
    */

    //D Calculate checksum

    int sum1 = 0;
    int sum2 = 0;
    int tot = 0;
    int cal1;
    int cal2;
    int mul1;
    int mul2;
    long y = n;
    //y is derived card number

    do

    {

        //1 Remove last digit + every other digit from card number and add to sum2
        cal2 = y % 10;
        y = y / 10;
        sum2 = sum2 + cal2;

        //2 Remove second last digit from card number
        cal1 = y % 10;
        y = y / 10;

        //3 Multiply second last digit + every other digit and add to sum1
        cal1 = cal1 * 2;
        mul1 = cal1 % 10;
        mul2 = cal1 / 10;
        sum1 = sum1 + mul1 + mul2;

    }

    while (y > 0);

    tot = sum1 + sum2;

    //E Check validity of Luhn Algorithm

    //If Luhn Algorithm is not valid
    if (tot % 10 != 0)
    {
        printf("INVALID\n");
        return 0;
    }

    //F Identify credit card type

    //Get starting digits
    long z = n;
    //z is derived card number

    //Divide card number until only 2 digits are left
    do
    {
        z = z / 10;
    }
    while (z > 100);

    //Check starting digits for card type (Amex:3, Visa:4, Mastercard:5)

    //Check for Amex
    if ((z / 10 == 3) && (z % 10 == 4 || z % 10 == 7))
    {
        printf("AMEX\n");
    }

    //Check for Visa
    else if (z / 10 == 4)
    {
        printf("VISA\n");
    }

    //Check for Mastercard
    else if ((z / 10 == 5) && (0 < z % 10 && z % 10 < 6))
    {
        printf("MASTERCARD\n");
    }

    //If none of them matches
    else
    {
        printf("INVALID\n");
    }

}