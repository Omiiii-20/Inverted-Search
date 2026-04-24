#include <stdio.h>
#include "inverted.h"

/// Function to display database
void display_database(MainNode *hashtable[])
{
    // Print table header
    printf("%-10s%-15s%-15s%-20s%-15s\n", "[index]", "[word]", "[filecount]", "[filename]", "[wordcount]");

    // Traverse each index
    for (int i = 0; i < SIZE; i++)
    {
        MainNode *main = hashtable[i];

        // Traverse main nodes
        while (main != NULL)
        {
            SubNode *sub = main->sub_link;
            int first = 1;

            // Traverse sub nodes
            while (sub != NULL)
            {
                if (first)
                {
                    // Print full row (main + sub)
                    printf("%-10d%-15s%-15d%-20s%-15d\n",
                           i, main->word, main->file_count, sub->filename, sub->word_count);
                    first = 0;
                }
                else
                {
                    // Print only sub node details
                    printf("%-44s%-25s%-20d\n", "", sub->filename, sub->word_count);
                }
                sub = sub->link;
            }

            main = main->link;
        }
    }
}
