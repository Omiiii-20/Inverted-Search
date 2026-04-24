 #include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "inverted.h"

/// Function to normalize a word (remove special chars, make lowercase)
static int normalize_token(const char *in, char *out, size_t out_size)
{
    int start = 0;
    int end = (int)strlen(in) - 1;

    // Skip unwanted chars at beginning
    while (start <= end && !isalnum((unsigned char)in[start])) 
        start++;
    // Skip unwanted chars at end
    while (end >= start && !isalnum((unsigned char)in[end])) 
        end--;

    if (start > end) 
        return 0;   // Empty after cleaning

    size_t k = 0;
    // Copy valid chars in lowercase
    for (int i = start; i <= end && k + 1 < out_size; ++i) 
    {
        out[k++] = (char)tolower((unsigned char)in[i]);
    }
    out[k] = '\0'; 
    return (k > 0);
}

/// Function to create database from file list
void create_database(SList *file_list, MainNode *hashtable[])
{
    // Initialize hash table
    for (int i = 0; i < SIZE; ++i) 
        hashtable[i] = NULL;

    // Traverse each file in list
    for (SList *f = file_list; f != NULL; f = f->link) 
    {
        FILE *fp = fopen(f->filename, "r");
        if (!fp) 
        {
            fprintf(stderr, "Warning: cannot open '%s'. Skipping.\n", f->filename);
            continue;
        }

        char raw[WORD_SIZE];
        // Read each word
        while (fscanf(fp, "%99s", raw) == 1) 
        {
            char token[WORD_SIZE];
            if (!normalize_token(raw, token, sizeof(token))) 
                continue;

            // Find hash index
            int index;
            if (isalpha((unsigned char)token[0]))
                index = token[0] - 'a';
            else
                index = SIZE - 1;

            // Search for word in bucket
            MainNode *m = hashtable[index];
            MainNode *found = NULL;
            while (m) 
            {
                if (strcmp(m->word, token) == 0) 
                { 
                    found = m; 
                    break; 
                }
                m = m->link;
            }

            if (!found) 
            {
                // Create new main node
                MainNode *newm = (MainNode*)malloc(sizeof(MainNode));
                if (!newm) 
                { 
                    fprintf(stderr,"malloc failed\n"); 
                    fclose(fp); 
                    return; 
                }

                strncpy(newm->word, token, sizeof(newm->word)-1);
                newm->word[sizeof(newm->word)-1] = '\0';
                newm->file_count = 1;
                newm->sub_link = NULL;

                // Create first subnode
                SubNode *s = (SubNode*)malloc(sizeof(SubNode));
                if (!s) 
                {
                    fprintf(stderr,"malloc failed\n"); 
                    fclose(fp); 
                    return; 
                }

                strncpy(s->filename, f->filename, sizeof(s->filename)-1);
                s->filename[sizeof(s->filename)-1] = '\0';
                s->word_count = 1;
                s->link = NULL;

                newm->sub_link = s;
                // Insert at start of bucket
                newm->link = hashtable[index];
                hashtable[index] = newm;
            } 
            else 
            {
                // Word found → check subnode
                SubNode *sn = found->sub_link;
                SubNode *sfound = NULL;
                while (sn) 
                {
                    if (strcmp(sn->filename, f->filename) == 0) 
                    { 
                        sfound = sn; 
                        break; 
                    }
                    sn = sn->link;
                }

                if (!sfound) 
                {
                    // Create new subnode
                    SubNode *s = (SubNode*)malloc(sizeof(SubNode));
                    if (!s) 
                    { 
                        fprintf(stderr,"malloc failed\n"); 
                        fclose(fp); 
                        return; 
                    }

                    strncpy(s->filename, f->filename, sizeof(s->filename)-1);
                    s->filename[sizeof(s->filename)-1] = '\0';
                    s->word_count = 1;
                    s->link = found->sub_link;
                    found->sub_link = s;
                    found->file_count++;
                } 
                else 
                {
                    // Increment count
                    sfound->word_count++;
                }
            }
        } // End while word loop

        fclose(fp);
    } // End file loop
}
