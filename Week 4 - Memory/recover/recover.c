#include <stdio.h>
#include <stdlib.h>

#define BLOCK 512

int main(int argc, char *argv[])
{
    // Check input validity
    if (argc != 2)
    {
        printf("USAGE: ./recover IMAGE\n");
        return 1;
    }
    // Open file to read
    FILE *file = fopen(argv[1], "r");
    // If file cannot be opened
    if (file == NULL)
    {
        printf("File not valid\n");
        return 1;
    }
    //Set image output count
    int img_count = 0;
    // Create a buffer for a JPEG block
    unsigned char buffer[BLOCK];
    // Set output file
    FILE *img = NULL;
    // Allocate space
    char *filename = malloc(8 * sizeof(char));
    // Loop until the end of input file
    while (fread(buffer, sizeof(char), BLOCK, file) >= 1)
    {
        // If file header matches JPEG file
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer [2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // Write output file
            sprintf(filename, "%03i.jpg", img_count);
            img = fopen(filename, "w");
            img_count++;
        }
        // Continue writing output file until new JPEG
        if (img != NULL)
        {
            fwrite(buffer, sizeof(char), BLOCK, img);
        }
    }
    // Free memory and close files
    free(filename);
    fclose(img);
    fclose(file);

    return 0;
}