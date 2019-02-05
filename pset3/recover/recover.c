#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int error(int sig);

int main(int argc, char *argv[])
{
    // Ensure only card file as argument
    if (argc != 2)
    {
        return error(1);
    }

    // Ensure file can be opened for reading
    char *fname = argv[1];
    FILE *file = fopen(fname, "r");
    if (file == NULL)
    {
        return error(2);
    }

    // Iterate over the file reading 512 B (FAT32 block size) at a time
    // If first 4B matches the pattern 0xff 0xd8 0xff 0xe*
    // then create a new file for writing
    // write the bytes until the next 4B patern is found
    // Continue this until the file is finished

    int fCnt = 0;
    int bSize = 512;
    unsigned char *block = malloc(bSize);
    int bLen = 0;
    FILE *img = NULL;

    char imgFile[7];
    do
    {
        bLen = fread(block, bSize, 1, file);
        // Check if next 4 bytes are JPEG from header
        if (block[0] == 0xff &&
            block[1] == 0xd8 &&
            block[2] == 0xff &&
            (block[3] & 0xf0) == 0xe0)
        {
            // If a file is open, close it and open a new one
            if (img != NULL)
            {
                fclose(img);
            }

            sprintf(imgFile, "%03i.jpg", fCnt++);
            img = fopen(imgFile, "w");

        }
        // If image file is open, write 1 block to it
        if (img != NULL)
        {
            fwrite(block, bSize, bLen, img);
        }
    }
    while (bLen > 0);

    // Close the last image and free resource
    if (img != NULL)
    {
        fclose(img);
    }
    free(block);
    fclose(file);
    return 0;
}

// Outputs error and returns a err exit code
int error(int sig)
{
    fprintf(stderr, "Usage: ./recover image\n");
    return sig;
}