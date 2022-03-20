#include <cs50.h>
#include <stdio.h>

int main(void)

{

    //n is height
    //i1 is left row or left x-axis
    //i2 is right row or right axis
    //j is column or y-axis
    //k is space or gap

    int n;

    do
    {
        //Prompt user for height
        n = get_int("Height: ");
    }
    //If user inputs height number besides than 1 to 8, ask again
    while (n < 1 || n > 8);

    //For each column
    for (int j = 0; j < n; j++)

    {

        //For each space
        for (int k = n - j - 2; k >= 0; k--)

        {
            //Print gaps
            printf(" ");
        }

        //Left pyramid or left rows
        for (int i1 = 0; i1 <= j; i1++)
        {
            //Make blocks
            printf("#");
        }

        //Add gap between the 2 pyramids
        if (j <= n)
        {
            printf("  ");
        }

        //Right pyramid or right rows
        for (int i2 = 0; i2 <= j; i2++)
        {
            printf("#");
        }

        //Move to next row
        printf("\n");

    }

}