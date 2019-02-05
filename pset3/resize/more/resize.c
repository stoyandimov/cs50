// Resizes images

#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

// Shows a usage message and returns 1
int error(void);


int main(int argc, char *argv[])
{
    // Program accept exactly three command-line arguments
    // (+1 for the executable name)
    if (argc != 4)
    {
        return error();
    }
    // f must be a floating-point value,
    float f = atof(argv[1]);
    // fn_in must be the name of a BMP to be resized
    char *fn_in = argv[2];
    // fn_out must be the name of the resized version to be written
    char *fn_out = argv[3];

    // If your program is not executed with such,
    // it should remind the user of correct usage,
    // as with printf, and main should return.
    if (!f || !fn_in || !fn_out)
    {
        return error();
    }

    // f must be a floating-point value in (0.0, 100.0) range
    if (f < 0 || f > 100)
    {
        return error();
    }

    // open input file
    FILE *inptr = fopen(fn_in, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", fn_in);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(fn_out, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", fn_out);
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

    bi.biWidth *= f;


    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // determine padding for scanlines
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        // iterate over pixels in scanline
        for (int j = 0; j < bi.biWidth; j++)
        {
            // temporary storage
            RGBTRIPLE triple;

            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

            for (int fi = 0; fi < f; fi++)
            {
                // write RGB triple to outfile
                fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
            }
        }

        // skip over padding, if any
        fseek(inptr, padding, SEEK_CUR);

        // then add it back (to demonstrate how)
        for (int k = 0; k < padding; k++)
        {
            fputc(0x00, outptr);
        }
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    return 0;
}

// Shows a usage message and returns 1
int error(void)
{
    printf("Usage: ./resize f infile outfile\n");
    return 1;
}