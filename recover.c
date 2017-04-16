#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// using an unsigned int of 1 byte
typedef uint8_t  BYTE;

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover image\n");
        return 1;
    }
    // open card file
    FILE *image = fopen(argv[1], "r");
    // checks if file opened
    if (image == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", argv[1]);
        return 2;
    }
    // Using buffer array where each element is of one byte
    BYTE buffer[512];
    char name[8] = "111.txt";
    FILE *reimage = fopen(name, "w");
    int imagecount = 0;
    while(fread(buffer, 1, 512, image) == 512)
    {   
        // check for jpeg tag
        if (buffer[0] == 0xff && 
           buffer[1] == 0xd8 && 
           buffer[2] == 0xff && 
           (buffer[3] & 0xf0) == 0xe0)
        {
            // close previous file
            // rename file pointer
            // open new file
            fclose(reimage);
            sprintf(name,"%03i.jpg",imagecount++);
            reimage = fopen(name, "w");
        }
        // write block into file
        fwrite(buffer, 1, 512, reimage);
    }
    
    // close any open files
    fclose(reimage);
    fclose(image);
    return 0;
}