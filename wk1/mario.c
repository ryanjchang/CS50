#include <cs50.h>
#include <stdio.h>

int main(void){
    int rows = 0;
    char buffer[100];
    while(rows < 1){
        printf("Height: ");
        if(fgets(buffer, sizeof(buffer), stdin)){
            if(1 == sscanf(buffer, "%d", &rows)){
                //printf("%d", rows);
            }
        }
    }
//if there are 3 rows
//2 spaces followed by 1 # followed by 1 space followed by 1 #
//1 space then 2#s then 1 space then 1 2#s

///spaces is rows - 1
//bricks is i + 1

    for(int i = 0; i < rows; i++){//each row
        for(int j = i; j < rows - 1; j++){//spaces
            printf(" ");//spaces
        }
        for(int j = 0; j < i + 1; j++){//spaces
            printf("#");//bricks
        }
        printf(" ");
        for(int j = 0; j < i + 1; j++){//spaces
            printf("#");//bricks
        }
        printf("\n");
    }
}
