

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inverted.h"

int main(int argc, char *argv[]) 
{
    // Validate command-line args
    if (argc < 2) 
    {
        printf("Usage: %s file1.txt file2.txt ...\n", argv[0]);
        return 1;
    }

    SList *file_list = NULL; // File list head

    // Process each input file
    for (int i = 1; i < argc; i++) 
    {
        char *filename = argv[i];

        // Check extension
        char *dot = strrchr(filename, '.'); 
        if (!dot || strcmp(dot, ".txt") != 0) 
        {
            printf("Skipping: Not a .txt file -> %s\n", filename);
            continue;
        }

        // Check file existence
        FILE *fp = fopen(filename, "r");
        if (!fp) 
        {
            printf("Skipping: File does not exist -> %s\n", filename);
            continue;
        }

        // Check file empty
        fseek(fp, 0, SEEK_END);
        long size = ftell(fp);
        fclose(fp);
        if (size == 0) 
        {
            printf("Skipping: File is empty -> %s\n", filename);
            continue;
        }

        // Check duplicate
        int duplicate = 0;
        SList *temp = file_list;
        while (temp) 
        {
            if (strcmp(temp->filename, filename) == 0) 
            {
                duplicate = 1;
                break;
            }
            temp = temp->link;
        }
        if (duplicate) 
        {
            printf("Skipping: Duplicate file -> %s\n", filename);
            continue;
        }

        // Create new file node
        SList *new = malloc(sizeof(SList));
        strcpy(new->filename, filename);
        new->link = NULL;

        // Insert node
        if (!file_list) 
            file_list = new;
        else 
        {
            SList *last = file_list;
            while (last->link) last = last->link;
            last->link = new;
        }
    }

    // Exit if no valid files
    if (!file_list) 
    {
        printf("No valid files to process. Exiting.\n");
        return 1;
    }

    // Init hashtable
    MainNode *hashtable[SIZE] = { NULL };

    int db_created = 0;
    int db_updated = 0;

    int choice;
    while (1) 
    {
        // Menu
        printf("\n----- Inverted Search Menu -----\n");
        printf("1. Create Database\n");
        printf("2. Display Database\n");
        printf("3. Search Database\n");
        printf("4. Save Database\n");
        printf("5. Update Database\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");

        // Input check
        if (scanf("%d", &choice) != 1) 
        {
            printf("Invalid input. Please enter 1-6.\n");
            while (getchar() != '\n');
            continue;
        }

        // Menu operations
        switch (choice) 
        {
            case 1: // Create DB
                if (db_created) 
                    printf("Database already created.\n");
                else if (db_updated) 
                    printf("Database already updated. Cannot create now.\n");
                else 
                {
                    create_database(file_list, hashtable);
                    db_created = 1;
                    printf("Database created successfully.\n");
                }
                break;

            case 2: // Display DB
                display_database(hashtable);
                break;

            case 3: // Search DB
                search_database(hashtable);
                break;

            case 4: // Save DB
                save_database(hashtable);
                break;

            case 5: // Update DB
                if (db_created) 
                    printf("Database already created. Cannot update now.\n");
                else if (db_updated) 
                    printf("Database already updated.\n");
                else 
                {
                    update_database(hashtable);
                    db_updated = 1;
                }
                break;

            case 6: // Exit
                printf("Exiting...\n");
                return 0;

            default:
                printf("Invalid choice. Please enter 1-6.\n");
        }
    }
}
