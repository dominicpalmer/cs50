#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./resize n infile outfile\n");
        return 1;
    }

    float n = atof(argv[1]);
    if ( n <= 0 || n > 100.0 )
    {
        fprintf(stderr, "Factor n must be a floating point within (0, 100].");
        return 1;
    }

    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    // create new file and info headers
    BITMAPFILEHEADER resized_bf = bf;
    BITMAPINFOHEADER resized_bi = bi;

    // change the dimensions of the output files bitmap
    resized_bi.biWidth = n * bi.biWidth;
    resized_bi.biHeight = n * bi.biHeight;

    // determine padding for scanlines
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int resized_padding = (4 - (resized_bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // change the size of the output file
    resized_bi.biSizeImage = ((((resized_bi.biWidth * resized_bi.biBitCount) + 31) & ~31) / 8) * abs(resized_bi.biHeight);
    resized_bf.bfSize = 54 + resized_bi.biSizeImage;

    // write outfile's BITMAPFILEHEADER
    fwrite(&resized_bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&resized_bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    if (n == 0.5)
    {
        // iterate over every other scanline of infile
        for (int i = 0, height = abs(bi.biHeight); i < height/2; i++)
        {
            // iterate over every other pixel in scanline
            for (int k = 0; k < bi.biWidth/2; k++)
            {
                // temporary storage
                RGBTRIPLE triple;

                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

                // write every other RGBTRIPLE
                fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                if (k == bi.biWidth/2 - 1)
                {
                    break;
                }
                else
                {
                    fseek(inptr, 3, SEEK_CUR);
                }
            }

            // add padding to each line of outfile
            for (int m = 0; m < resized_padding; m++)
            {
                fputc(0x00, outptr);
            }
            // navigate to next scanline, skipping over one

            fseek(inptr, ((bi.biWidth * sizeof(RGBTRIPLE)) + padding) + 3, SEEK_CUR);
        }
    }
    else
    {
        // iterate over infile's scanlines
        for (int i = 0, height = abs(bi.biHeight); i < height; i++)
        {
            // iterate to repeat each scanline n times
            for (int j = 0; j < n; j++)
            {
                // iterate over pixels in scanline
                for (int k = 0; k < bi.biWidth; k++)
                {
                    // temporary storage
                    RGBTRIPLE triple;

                    // read RGB triple from infile
                    fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

                    // write RGB triple to outfile factor times
                    for (int l = 0; l < n; l++)
                    {
                        fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                    }
                }

                fseek(inptr, padding, SEEK_CUR);

                // add padding to output file
                for (int m = 0; m < resized_padding; m++)
                {
                    fputc(0x00, outptr);
                }

                fseek(inptr, -((bi.biWidth * sizeof(RGBTRIPLE)) + padding), SEEK_CUR);

            }

            fseek(inptr, (bi.biWidth * sizeof(RGBTRIPLE)) + padding, SEEK_CUR);

        }
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
