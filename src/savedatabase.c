#include "inverted.h"

/// Function to save database to backup file
void save_database(MainNode *hashtable[])
{
    FILE *fp;
    char backup_filename[100];

    // Check if database is empty
    int empty = 1;
    for (int i = 0; i < SIZE; i++)
    {
        if (hashtable[i] != NULL)
        {
            empty = 0;
            break;
        }
    }

    if (empty)
    {
        printf("Database is empty. Nothing to save.\n");
        return;
    }

    // Get backup file name
    printf("Enter the backup filename: ");
    scanf("%s", backup_filename);

    // Open file for writing
    fp = fopen(backup_filename, "w");
    if (fp == NULL)
    {
        perror("Error opening backup file");
        return;
    }

    // Traverse hash table
    for (int i = 0; i < SIZE; i++)
    {
        MainNode *mtemp = hashtable[i];

        // Traverse main nodes
        while (mtemp != NULL)
        {
            // Save main node data
            fprintf(fp, "#%d;%s;%d;", i, mtemp->word, mtemp->file_count);

            // Traverse sub nodes
            SubNode *stemp = mtemp->sub_link;
            while (stemp != NULL)
            {
                fprintf(fp, "%s;%d;", stemp->filename, stemp->word_count);
                stemp = stemp->link;
            }

            // End of one main node
            fprintf(fp, "#\n");
            mtemp = mtemp->link;
        }
    }

    fclose(fp);
    printf("Database saved successfully in %s\n", backup_filename);
}
