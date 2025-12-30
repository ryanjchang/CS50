#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

int main(int argc, string argv[]){//don't actually need these tbh

    //ask user for word 1
    char buffer[30];
    char word1[30];
    char word2[30];
    int sum1 = 0;
    int sum2 = 0;
    //use ascii values for this
    //use a normal array with the int values of each letter
    //index 0 = 'a', index 1 = 'b'
    int point_values[26] = {1, 3, 3, 2, //a, b, c, d...
                            1, 4, 2, 4,
                            1, 8, 5, 1,
                            3, 1, 1, 3,
                            10, 1, 1, 1,
                            1, 4, 4, 8,
                            4, 10};

    printf("Player 1: ");
    if(fgets(buffer, sizeof(buffer), stdin)){
        if(1 == sscanf(buffer, "%s", word1)){//don't need & (address of) because it is an array and already has pointer
            printf("%s\n", word1);
        }
    }
    //this doesn't work with non words but whatever

    printf("Player 2: ");
    if(fgets(buffer, sizeof(buffer), stdin)){
        if(1 == sscanf(buffer, "%s", word2)){
            printf("%s\n", word2);
        }
    }
//check each letter with that dictionary and add that value to the sum
    for(int i = 0, n = strlen(word1); i < n; i++){
        sum1 += point_values[tolower(word1[i]) - 'a'];
    }
    for(int i = 0, n = strlen(word2); i < n; i++){
        sum2 += point_values[tolower(word2[i]) - 'a'];
    }

    printf("Sum 1: %i\n", sum1);
    printf("Sum 2: %i\n", sum2);

    if(sum1 > sum2){
        printf("Player 1 wins!\n");
    }else if(sum1 < sum2){
        printf("Player 2 wins!\n");
    }else{
        printf("Tie!\n");
    }
}
