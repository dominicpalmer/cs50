#include <stdio.h>
#include <stdlib.h>

int main(int argc, char * argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover image");
        return 1;
    }

    // declare infile
    char* infile = argv[1];

    // open infile
    FILE* input = fopen(infile, "r");

    // ensure file opened
    if (infile == NULL)
    {
        fprintf(stderr, "Image could not be opened.");
        return 2;
    }

    // picture detection toggle, jpeg image counter, buffer and image declaration
    int picture_found = 0;
    int picture_count = 0;
    unsigned char buffer[512];
    FILE* image;

    // increment over the whole file until the input pointer reaches the end
    while (fread(buffer, 512, 1, input) == 1)
    {
        // check whether the block could be the start of a jpeg
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff)
        {
            // if an image is already open, close it to allow a new one to be created
            if (picture_found == 1)
            {
                fclose(image);
            }
            // if no picture is open, set picture_found toggle to 1
            else
            {
                picture_found = 1;
            }

            // declare a picture name jpeg file, open it, increment the picture counter
            char picture_name[10];
            if (picture_count < 10)
            {
                sprintf(picture_name, "00%i.jpg", picture_count);
                image = fopen(picture_name, "a");
                picture_count++;
            }
            else
            {
                sprintf(picture_name, "0%i.jpg", picture_count);
                image = fopen(picture_name, "a");
                picture_count++;
            }
        }

        // write the buffer to the currently open image (if any)
        if (picture_found == 1 && sizeof(image) < 2048)
        {
            fwrite(buffer, 1, 512, image);
        }
    }

    return 0;
}