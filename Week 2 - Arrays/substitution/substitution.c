#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(int argc, string argv[])
{

    //Validate number of commands
    if (argc != 2)
    {
        printf("Type: ./substitution KEY\n");
        return 1;
    }

    //Validate length of key (26)
    int argv_length = strlen(argv[1]);
    if (argv_length != 26)
    {
        printf("Key must be up to 26 CHARACTERS\n");
        return 1;
    }

    //Validate each character in the key
    for (int i = 0; i < argv_length; i++)
    {

        //If key contains non-alphabetical character
        if (!isalpha(argv[1][i]))
        {
            printf("Key must be ALPHABETICAL\n");
            return 1;
        }

        //If key contains repeated characters
        for (int j = i + 1; j < argv_length; j++)
        {
            if (argv[1][i] == argv[1][j])
            {
                printf("Key MUST NOT contain REPEATED alphabets\n");
                return 1;
            }
        }

    }


    string plaintext = get_string("plaintext: ");
    int plaintext_length = strlen(plaintext);
    printf("ciphertext: ");


    for (int i = 0; i < plaintext_length; i++)
    {
        //If character is uppercase
        if (isupper(plaintext[i]))
        {
            printf("%c", toupper(argv[1][plaintext[i] - 'A']));
        }
        //If character is lowercase
        else if (islower(plaintext[i]))
        {
            printf("%c", tolower(argv[1][plaintext[i] - 'a']));
        }
        //If character is not alphabetical
        else
        {
            printf("%c", plaintext[i]);
        }
    }
    printf("\n");

}