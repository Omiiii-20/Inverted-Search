#include <stdio.h>      
#include <stdlib.h>     
#include <string.h>     
#include <ctype.h>      
#include "inverted.h"   

/// Function to normalize word
static int normalize_token(const char *in, char *out, size_t out_size)
{
    int start = 0;
    int end = (int)strlen(in) - 1;

    // Skip unwanted chars at start
    while (start <= end && !isalnum((unsigned char)in[start])) 
        start++;
    // Skip unwanted chars at end
    while (end >= start && !isalnum((unsigned char)in[end])) 
        end--;

    if (start > end) 
        return 0;

    size_t k = 0;
    // Copy valid chars in lowercase
    for (int i = start; i <= end && k + 1 < out_size; ++i) 
    {
        out[k++] = (char)tolower((unsigned char)in[i]);
    }

    out[k] = '\0';
    return (k > 0);
}

/// Function to search word in database
void search_database(MainNode *hashtable[])
{
    char raw[WORD_SIZE];
    printf("Enter the word to search: ");
    if (scanf("%99s", raw) != 1) 
    {        
        printf("Input error.\n");
        return;
    }

    char token[WORD_SIZE];
    if (!normalize_token(raw, token, sizeof(token))) 
    {   
        printf("Invalid search token.\n");
        return;
    }

    // Find hash index
    int index;
    if (isalpha((unsigned char)token[0]))
        index = token[0] - 'a';
    else
        index = SIZE - 1;

    if (index < 0 || index >= SIZE) 
    {
        printf("Invalid index.\n");
        return;
    }

    MainNode *cur = hashtable[index];

    // If no main nodes in this index
    if (!cur) 
    {                                        
        printf("The word '%s' is not found in any file.\n", raw);
        return;
    }

    // Traverse main nodes
    while (cur) 
    {
        if (strcmp(cur->word, token) == 0) 
        {
            printf("The word '%s' is present in %d file(s)\n", raw, cur->file_count);

            // Traverse sub nodes
            SubNode *s = cur->sub_link;
            while (s) 
            {
                printf("In file %s for %d times\n", s->filename, s->word_count);
                s = s->link;
            }
            return;
        }
        cur = cur->link;
    }

    // If word not found
    printf("The word '%s' is not found in any file.\n", raw);
}
