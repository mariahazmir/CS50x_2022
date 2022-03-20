#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);

int main(void)
{
    // Get text
    string text = get_string("Text: ");
    int letters = count_letters(text);
    int words = count_words(text);
    int sentences = count_sentences(text);

    // Calculate Coleman-Liau index
    float L = 100 * ((float)letters / (float)words);
    float S = 100 * ((float)sentences / (float)words);
    int index = round(0.0588 * L - 0.296 * S - 15.8);

    // Display grade
    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index > 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", index);
    }
}

// Count number of letters
int count_letters(string text)
{
    int let = 0;
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        //Making sure only alphabets are counted
        if (isalpha(text[i]))
        {
            let++;
        }
    }
    return let;
}

// Count number of words
int count_words(string text)
{
    int wor = 1;
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        // Instead of counting words, count number of spaces + 1
        if (isspace(text[i]) > 0)
        {
            wor++;
        }
    }
    return wor;
}

// Count number of sentences
int count_sentences(string text)
{
    int sen = 0;
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        // Instead of counting sentence, count number of .'s, !'s or ?'s.
        if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            sen++;
        }
    }
    return sen;
}


