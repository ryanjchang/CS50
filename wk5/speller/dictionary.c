// Implements a dictionary's functionality

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 26;

int words = 0;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    //hash the word
    int position = hash(word);

    //go through linked list until find it or reach end
    node *head = table[position];
    while(head != NULL){
        //case insensitive
        if(strcasecmp(head -> word, word) == 0){
            return true;
        }
        head = head -> next;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function

    //case insensitive
    return toupper(word[0]) - 'A';
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    for(int i = 0; i < N; i++){
        table[i] = NULL;
    }

    FILE *file = fopen(dictionary, "r");
    if(file == NULL){
        printf("Failed to load dictionary\n");
        return false;
    }
    char c;
    char word[LENGTH + 1];
    int index = 0;
    while(fread(&c, sizeof(char), 1, file) > 0){
        if(isalpha(c) || (c == '\'' && index > 0)){//if letter or apostrophe
            word[index] = c;
            index++;
        }else if(index > 0){//can assume dict only has letters/apostrophes
            //reset index and end word
            word[index] = '\0';
            index = 0;
            words++;
            //add the word we have built up to the hash table
            int position = hash(word);

            //prepend it into the singly linked list
            //create our new node
            node *new_node = malloc(sizeof(node));
            if(new_node == NULL){
                fclose(file);
                unload();
                return false;
            }
            //set word to our new word
            strcpy(new_node -> word, word);
            //set next to the first node in the hash table
            new_node -> next = table[position];
            //set the first node in the hash table to our new node
            table[position] = new_node;
        }
    }
    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return words;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    //need to iterate through the dictionary and free each node from linked list
    for(int i = 0; i < N; i++){
        //get the pointer to the first node in the table
        node *temp = table[i];
        while(temp != NULL){//while there are still nodes
            //store the pointer to the next node
            node *next = temp -> next;
            //free the current node
            free(temp);
            //temp = next node
            temp = next;
        }
    }
    return true;
}
