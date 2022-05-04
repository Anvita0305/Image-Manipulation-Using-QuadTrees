#include <stdio.h>
#include <stdlib.h>
#define RGB_COMP 255

typedef struct PPMPixel
{
    unsigned char red, green, blue;
} PPMPixel;

typedef struct PPMImage
{
    int height, width;
    PPMPixel *data;
} PPMImage;

static PPMImage *readPPM(const char *filename)
{
    char magic_number[16];
    char arr[16];
    PPMImage *img_grid;
    FILE *fp;
    int c, rgb_comp_color;
    int i, j;
    // open PPM file for reading
    fp = fopen(filename, "rb");
    if (!fp)
    {
        printf("Unable to open file '%s'\n", filename);
        exit(1);
    }

    fgets(magic_number, sizeof(magic_number), fp); // read image format

    // check the image format
    if (magic_number[0] != 'P' || magic_number[1] != '6')
    {
        printf("Invalid image format!\n");
        exit(1);
    }

    // alloc memory form image
    img_grid = malloc(sizeof(PPMImage));
    if (!img_grid)
    {
        printf("Unable to allocate memory\n");
        exit(1);
    }

    // check for comments
    c = getc(fp);
    while (c == '#')
    {
        while (getc(fp) != '\n')
            ;
        c = getc(fp);
    }
    ungetc(c, fp); // pushes the character on the stream so that it can take new value

    // read image size information
    fscanf(fp, "%d %d", &img_grid->height, &img_grid->width); // read height and width
    printf("Height of the Image is %d\n", img_grid->height);
    printf("Width of the Image is %d\n", img_grid->width);

    while (fgetc(fp) != '\n');

    // memory allocation for pixel data
    img_grid->data = (PPMPixel *)malloc(img_grid->height * img_grid->width * sizeof(PPMPixel)); // height*width*pixel_size

    if (!img_grid)
    {
        printf("Unable to allocate memory\n");
        exit(1);
    }

    fread(img_grid->data, 3*img_grid->height, img_grid->width, fp);

    fclose(fp);
    return img_grid;
}

int main()
{
    PPMImage *image;
    image = readPPM("sample1.ppm");
    printf("Image Read Successfully!");
    return 0;
}