#include <stdio.h>
#include <cs50.h>

int main(void){
    char buffer[100];
    long number = -1;
    int sum = 0;
    while(number < 0){
        printf("Enter your card number:");
        if(fgets(buffer, sizeof(buffer), stdin)){
            if(1 == sscanf(buffer, "%ld", &number)){
                printf("%ld\n", number);
            }
        }
    }

    //multiply every other number starting at second to last
    for(long i = number/10; i > 0; i/=100){
        int digit = (2 * (i%10));
        if(digit >= 10){
            sum+= digit%10;//ones place
            sum+= digit/10;//tens place
        }
        else{
            sum += digit; //doubles then adds the digits of the product
        }
    }
    //add not multiplied digits
    for(long i = number; i > 0; i/=100){
        sum += i%10;
    }
    //% by 10 == 0 then all is good
    if(sum % 10 == 0){
        printf("VISA");
    }else{
        printf("%i\n", sum);
        printf("Invalid Number\n");
    }
    //for some reason the sum is 19 instead of 20 for the 4003600000000014 card
}
