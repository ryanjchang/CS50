#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

const int JPEG_SIZE = 512;

int main(int argc, char *argv[])
{
    //check that command is correct
    if(argc != 2){
        printf("Usage: ./recover card.raw\n");
        return 1;
    }
    //read the file
    FILE *input = fopen(argv[1], "rb");
    FILE *output = NULL;
    if(input == NULL){
        printf("Could not open file");
        return 1;
    }
    uint8_t buffer[JPEG_SIZE];
    int count = 0;
    bool writing_jpg = false;

    //while the next block is a 512 bit chunk
    while(fread(buffer, 1, JPEG_SIZE, input) == JPEG_SIZE){
        //find the 4 byte header
        int fourth_bit = 14;
        if(buffer[0] == 0xff
        && buffer[1] == 0xd8
        && buffer[2] == 0xff
        && buffer[3] >> 4 == fourth_bit
        ){//starts w 1110, shift right by 4, get value 8 + 4 + 2 = 14
            if(writing_jpg){//close current jpg
                fclose(output);
            }else{//start reading a new jpg
                writing_jpg = true;
            }
            //file to write to
            char filename[8];//8 bytes - 1 per char + 1 for null terminator
            sprintf(filename, "%03d.jpg", count);
            output = fopen(filename, "wb");
            count += 1;
        }
        if(writing_jpg){
            //if we found a header/are not done reading
            fwrite(buffer, sizeof(buffer), 1, output);
        }
    }
    if(writing_jpg && output != NULL){
        //potential last incomplete block
        fclose(output);
    }
    //free up all memory
    fclose(input);
}
