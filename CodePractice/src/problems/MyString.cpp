#include <stdio.h>
#include "MyString.h"
#include <string.h>
#include "Utils.h"

int StringToInt(char* str)
{
    int result = 0;
    bool negative = false;

    for (int i = 0; str[i] != '\0'; i++)
    {
        if (str[i] == '-')
        {
            negative = true;
        }
        else
        {
            result = (str[i] - '0') + result*10;
        }
    }

    if (negative)
    {
        result = ((~result) + 1);
    }

    return result;
}

char findFirstNonRepeatingChar(char* str)
{
    enum {MAX_CHAR = 128};
    int charHash[MAX_CHAR] = {0};

    for (int i = 0; str[i] != '\0'; i++)
    {
        charHash[str[i]]++;
    }

    for (int i = 0; str[i] != '\0'; i++)
    {
        if (charHash[str[i]] == 1)
        {
            return str[i];
        }
    }

    return '\0';
}

// Reverse a word from start to end in specified string str
void reverseWord(char* str, int start, int end)
{
    if (start >= end)
    {
        return;
    }

    int i, j;

    for (i = start, j = end; i < j; i++, j--)
    {
        swapChar(&str[i], &str[j]);
    }
}


void reverseStringWords(char* str)
{
    int len = strlen(str);
    int left, right;
    int leftWordlength = 0;
    int rightWordlength = 0;

#if LOGIC_ONE
    for (left = 0, right = (len - 1); (left < right); left++, right--)
    {
        // If left char is space, reverse already swap word on the right and
        // then append the space char
        if (str[left] == ' ')
        {
            // reverse the work on the right
            reverseWord(str, right+1, right+leftWordlength);
            leftWordlength = 0;
        }

        if (str[right] == ' ')
        {
            // reverse the work on the left
            reverseWord(str, left-rightWordlength, left-1);
            rightWordlength = 0;
        }

        swapChar(&str[left], &str[right]);

        leftWordlength++;
        rightWordlength++;
    }
#else
    // First reverse the whole string in one shot
    reverseWord(str, 0, len-1);

    // now reverse just the individual words
    for (left = 0; str[left] != '\0'; left++)
    {
        //don't count space char
        if (str[left] == ' ')
        {
            // reverse the work on the right
            reverseWord(str, left-leftWordlength, (left-1));
            leftWordlength = 0;
        }
        else
        {
            leftWordlength++;
        }
    }
    reverseWord(str, left-leftWordlength, (left-1));

#endif
}

void deleteSubString(char* str, char* substr)
{

}