#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

int count_letters(string t);
int count_words(string t);
int count_sentences(string t);
int calculate_index(int num_letters, int num_words, int num_sentences);

int main(void){
    char text[500];
/** Assume:
will contain at least one word;
will not start or end with a space; and
will not have multiple spaces in a row.
*/
    printf("Text: ");
    fgets(text, sizeof(text), stdin);
    printf("%s\n", text);
    printf("Length: %li\n", strlen(text));

    int letters = count_letters(text);
    int words = count_words(text);
    int sentences = count_sentences(text);
    int index = calculate_index(letters, words, sentences);
    if(index >= 16){
        printf("16+ reading level\n");
    }else{
        printf("%i reading level\n", index);
    }

    //now find letters / 100 words
    //if there are only 50 words we should do divide by 50/100 or multiply by 2
    //sentences per 100 words. same thing


}

int count_letters(char t[]){
    int count = 0;
    for(int i = 0, n = strlen(t); i<n; i++){
        if(t[i] != ' ' && t[i] != '.' && t[i] != '!' && t[i] != '?'){
            count++;
        }
    }
    printf("letters: %i\n", count);
    return count;
}
int count_words(char t[]){
    //number of words should be the number of spaces + 1 for the last word
    //number of letters is just the length of the string minus the number of spaces
    //divide by 100
    int count = 1;
    for(int i = 0, n = strlen(t); i<n; i++){
        if(t[i] == ' '){
            count++;
        }
    }
    printf("words: %i\n", count);
    return count;
}
int count_sentences(char t[]){
    int count = 0;
    for(int i = 0, n = strlen(t); i<n; i++){
        if(t[i] == '.' || t[i] == '!' || t[i] == '?'){
            count++;
        }
    }
    printf("sentences: %i\n", count);
    return count;
}
int calculate_index(int num_letters, int num_words, int num_sentences){
    float L;
    float S;
    L = num_letters / ((float)num_words /100.0);//L = letters per 100 words
    S = num_sentences / ((float)num_words /100.0);//s = average number of sentences per 100 words
    printf("L: %f\n", L);
    printf("S: %f\n", S);
    float index = 0.0588 * L - 0.296 * S - 15.8;

    return round(index);
    //if more than 16, say 16+
}
