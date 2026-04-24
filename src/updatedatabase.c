#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inverted.h"

/// Function to update database from backup file
void update_database(MainNode *hashtable[])
{
    char filename[50];
    FILE *fp;

    // Get backup file name
    printf("Enter the backup file name: ");  
    scanf("%s", filename);

    // Open file
    fp = fopen(filename, "r");
    if (fp == NULL)  
    {
        printf("Error: Unable to open file %s\n", filename);
        return;
    }

    char line[1000];

    // Validate first line
    if (fgets(line, sizeof(line), fp) == NULL || line[0] != '#')
    {
        printf("Error: Invalid backup file format.\n");
        fclose(fp);
        return;
    }

    // Clear old data
    for (int i = 0; i < SIZE; i++)
        hashtable[i] = NULL;

    // Read main nodes
    while (fgets(line, sizeof(line), fp))
    {
        int index, file_count;
        char word[50];

        // Parse main node
        if (sscanf(line, "%d %s %d", &index, word, &file_count) != 3)
            continue;

        MainNode *mnew = malloc(sizeof(MainNode));
        strcpy(mnew->word, word);
        mnew->file_count = file_count;
        mnew->sub_link = NULL;

        // Insert main node
        mnew->link = hashtable[index];
        hashtable[index] = mnew;

        // Read sub nodes
        for (int i = 0; i < file_count; i++)
        {
            char fname[50];
            int word_count;
            fscanf(fp, "%s %d", fname, &word_count);

            SubNode *snew = malloc(sizeof(SubNode));
            strcpy(snew->filename, fname);
            snew->word_count = word_count;

            // Insert sub node
            snew->link = mnew->sub_link;
            mnew->sub_link = snew;
        }
    }

    fclose(fp);
    printf("Database updated from backup file successfully.\n");
}
