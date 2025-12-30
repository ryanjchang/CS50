#include <stdio.h>
#include <string.h>
#include <cs50.h>
#include <ctype.h>
// ./substitution NQXPOMAFTRHLZGECYJIUWSKDVB
bool check_char(string key);
int main(int argc, string argv[]){
    string key = argv[1];
    string plainText;
   if(argc != 2){
        printf("Usage: ./substitution key\n");
        return 1;
    }
    if(!check_char(key)){
        printf("Key must contain 26 unique characters\n");
        return 1;
    }
    plainText = get_string("Plaintext: ");
    int length = strlen(plainText);
    char cipherText[length + 1];
    for(int i = 0; i < length; i++){
        if(islower(plainText[i])){
            int index = plainText[i] -= 97;//lowercase a is 97
            cipherText[i] = key[index];//ciphertext a becomes key a - 97 = index 0 which is cipher a
            if(isupper(cipherText[i])){
                cipherText[i] +=32;//change back to uppercase
            }
        }else if(isupper(plainText[i])){
            int index = plainText[i] -= 65;//capital A is 65
            cipherText[i] = key[index];
            if(islower(cipherText[i])){
                cipherText[i] -=32;//change to lowercase
            }
        }else{
            cipherText[i] = plainText[i];
        }
    }
    cipherText[length] = '\0';//specifies that string is over by adding \0 to the end
    printf("ciphertext: %s\n", cipherText);
    return 0;//success
}
bool check_char(string key){
    int length = strlen(key);
    if(length != 26){
        printf("Key must be of size 26\n");
        return false;
    }
    for(int i = 0; i < length; i++){
        key[i] = toupper(key[i]);
    }
    for(int i = 0; i < length; i++){
        if(!isalpha(key[i])){
            return false;
        }
        for(int j = i+1; j < length; j++){
            if(key[i] == key[j]){
                return false;
            }
        }
    }
    return true;
}
