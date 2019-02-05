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
    // f must be a integer value,
    int n = atoi(argv[1]);
    // infile must be the name of a BMP to be resized
    char *infile = argv[2];
    // outfile must be the name of the resized version to be written
    char *outfile = argv[3];

    // If your program is not executed with such,
    // it should remind the user of correct usage,
    // as with printf, and main should return.
    if (!n || !infile || !outfile)
    {
        return error();
    }

    // f must be a integer value in (0, 100) range
    if (n < 0 || n > 100)
    {
        return error();
    }

    // Copy.c starts
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

    // change width and update padding for scanlines
    int orgPadding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int orgWidth = bi.biWidth;
    int orgHeight = bi.biHeight;
    bi.biWidth *= n;
    bi.biHeight *= n;
    int newPadding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // Set the new size
    bi.biSizeImage = ((sizeof(RGBTRIPLE) * bi.biWidth) + newPadding) * abs(bi.biHeight);
    bf.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + bi.biSizeImage;

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // temporary storage
    int lineWidth = sizeof(RGBTRIPLE) * bi.biWidth;
    RGBTRIPLE *triples = malloc(lineWidth);
    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(orgHeight); i < biHeight; i++)
    {
        int tripleCnt = 0;

        // iterate over pixels in scanline
        for (int j = 0; j < orgWidth; j++)
        {
            RGBTRIPLE triple;

            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

            for (int wr = 0; wr < n; wr++)
            {
                triples[tripleCnt++] = triple;
            }
        }

        // Write n times the previous line encluding padding
        for (int j = 0; j < n; j++)
        {
            // write RGB triple to outfile
            fwrite(triples, sizeof(RGBTRIPLE), tripleCnt, outptr);

            // add the new padding
            for (int k = 0; k < newPadding; k++)
            {
                fputc(0x00, outptr);
            }
        }

        // skip over original padding, if any
        fseek(inptr, orgPadding, SEEK_CUR);
    }
    free(triples);

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}

// Shows a usage message and returns 1
int error(void)
{
    printf("Usage: ./resize n infile outfile\n");
    return 1;
}