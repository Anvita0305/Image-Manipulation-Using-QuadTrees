#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct pixels
{
    char red;
    char blue;
    char green;
} pixels;

// quadTree structure
typedef struct quadTree
{
    pixels p;
    int data;
    int index;
    int area;
    struct quadTree *topLeft;
    struct quadTree *topRight;
    struct quadTree *bottomLeft;
    struct quadTree *bottomRight;
} quadTree;

// quadTree info
typedef struct quadInfo
{
    char red;
    char green;
    char blue;
    int area;
    // stores indices in a vector
    int topLeft;
    int topRight;
    int bottomLeft;
    int bottomRight;
} quadInfo;

void initTree(quadTree **q)
{
    *q = NULL;
    return;
}

//--------------------------image manipulation functions--------------------//

//reads the data from .ppm image and converts it to a 2D Matrix
pixels **read(int *height, int *width, char *file)
{
    FILE *f;
    char magic_num[3];
    int color;
    int i;

    f = fopen(file, "rb");
    if (!f)
    {
        printf("Unable to Open the File!\n");
        exit;
    }

    fscanf(f, "%s", magic_num);
    if (magic_num[0] != 'P' || magic_num[1] != '6')
    {
        printf("Invalid Image Format!\n");
        exit;
    }
    fscanf(f, "%d ", width);
    fscanf(f, "%d\n", height);
    fscanf(f, "%d", &color);

    char garbage;
    fread(&garbage, sizeof(char), 1, f);

    pixels **r = (pixels **)malloc(sizeof(pixels *) * (*height));

    for (i = 0; i < (*height); i++)
    {
        r[i] = malloc(sizeof(pixels) * (*width));
        fread(r[i], sizeof(pixels), (*width), f);
    }

    fclose(f);
    return r;
}

//converting 2D matrix data to an image
void convertToImg(pixels** img, char* fileName,int size)
{
    int i;
    FILE* fp=fopen(fileName,"w");
    if(!fp)
    {
        printf("Invalid File Name!\n");
        return;
    }
    //ppm file header specifications
    fprintf(fp,"P6\n");
    fprintf(fp,"%d %d\n",size,size);
    fprintf(fp,"255");
    //write back pixels
    for ( i = 0; i < size; i++)
    {
        fwrite(img[i],sizeof(pixels),size,fp);
    }
    fclose(fp);
    return;
}


//------------------------------SUPPL FUNCTIONS------------------------//

float getMean(pixels **img, quadTree *t, int horiz, int ver, int dist)
{
    // calculate mean of image to check if the image can be compressed
    float mean = 0;
    int r = 0, g = 0, b = 0;
    int i, j;
    t = malloc(sizeof(quadTree));
    t->area = dist * dist;
    for (i = horiz; i < horiz + dist; i++)
    {
        for (j = ver; j < ver + dist; j++)
        {
            r = r + img[i][j].red;
            b = b + img[i][j].blue;
            g = g + img[i][j].green;
        }
    }
    r = r / t->area;
    b = b / t->area;
    g = g / t->area;

    for (i = horiz; i < horiz + dist; i++)
    {
        for (j = ver; j < ver + dist; j++)
        {
            mean = mean + pow(r - img[i][j].red, 2) + pow(g - img[i][j].green, 2) + pow(b - img[i][j].blue, 2);
        }
    }
    mean = mean / (3 * t->area);
    // printf("mean:%f\n",mean);
    // printf("area:%d\n",t->area);

    return mean;
}

//************************************************************************//
/*
 * Function used to copy nodes addresses
 * from the tree into a pointer vector
 */
void traversal(quadTree *node, quadTree **vector[], unsigned int *index)
{
    if (node != NULL)
    {
        if ((*index) > 0)
            (*vector) = realloc((*vector), sizeof(quadTree *) * ((*index) + 1));
        (*vector)[(*index)] = node;
        node->index = (*index);
        (*index)++;

        traversal(node->topLeft, vector, index);
        traversal(node->topRight, vector, index);
        traversal(node->bottomRight, vector, index);
        traversal(node->bottomLeft, vector, index);
    }
    else
        return;
}

/*
 *	Using the vector from above
 *	computes the vector that needs to be written
 */
void copy_to_vector(quadTree **vp, quadInfo **v, int index)
{

    unsigned int i;

    for (i = 0; i < index; i++)
    {
        (*v)[i].red = vp[i]->p.red;
        (*v)[i].blue = vp[i]->p.blue;
        (*v)[i].green = vp[i]->p.green;
        (*v)[i].area = vp[i]->area;

        if (vp[i]->topLeft != NULL)
            (*v)[i].topLeft = vp[i]->topLeft->index;
        else
            (*v)[i].topLeft = -1;

        if (vp[i]->topRight != NULL)
            (*v)[i].topRight = vp[i]->topRight->index;
        else
            (*v)[i].topRight = -1;

        if (vp[i]->bottomRight != NULL)
            (*v)[i].bottomRight = vp[i]->bottomRight->index;
        else
            (*v)[i].bottomRight = -1;

        if (vp[i]->bottomLeft != NULL)
            (*v)[i].bottomLeft = vp[i]->bottomLeft->index;
        else
            (*v)[i].bottomLeft = -1;
    }
}
//**********************************************************************//

//-----------------------COMPRESSION FUNC----------------------------//
void compressImage(pixels **img, quadTree **t, int horiz, int ver, int dist, int factor)
{
    int r = 0, g = 0, b = 0;
    int i, j;
    *t = malloc(sizeof(quadTree));
    (*t)->area = dist * dist;
    float mean = getMean(img, *t, horiz, ver, dist);

    for (i = horiz; i < horiz + dist; i++)
    {
        for (j = ver; j < ver + dist; j++)
        {
            r = r + img[i][j].red;
            b = b + img[i][j].blue;
            g = g + img[i][j].green;
        }
    }
    r = r / (*t)->area;
    b = b / (*t)->area;
    g = g / (*t)->area;

    (*t)->p.blue = b;
    (*t)->p.red = r;
    (*t)->p.green = g;

    if (mean > factor)
    {
        // printf("hi in comp\n");
        compressImage(img, &(*t)->topLeft, horiz, ver, (dist / 2), factor);
        compressImage(img, &(*t)->topRight, horiz + (dist / 2), ver, (dist / 2), factor);
        compressImage(img, &(*t)->bottomLeft, horiz, ver + (dist / 2), (dist / 2), factor);
        compressImage(img, &(*t)->bottomRight, horiz + (dist / 2), ver + (dist / 2), (dist / 2), factor);
    }
    else
    {
        (*t)->topLeft = NULL;
        (*t)->topRight = NULL;
        (*t)->bottomLeft = NULL;
        (*t)->bottomRight = NULL;
    }
    return;
}
//---------------------COMPRESSION FUNC ENDS-------------------------------//

//---------------------DECOMPRESSION FUNC---------------------------------//

void decompressImage(pixels ***img, quadTree *t, int horiz, int ver, int dist)
{
    int i, j;
    // if the tree is already optimized
    if (t->topLeft==NULL && t->topRight==NULL && t->bottomLeft==NULL && t->bottomRight==NULL)
    {
        // write back pixels as they are
        for (i = ver; i < ver + dist; i++)
        {
            for (j = horiz; j < horiz + dist; j++)
            {
                {
                    (*img)[i][j].blue = t->p.blue;
                    (*img)[i][j].red = t->p.red;
                    (*img)[i][j].green = t->p.green;
                }
            }
        }
    }
    else
    {
        decompressImage(img,t->topLeft,horiz,ver,dist/2);
        decompressImage(img,t->topRight,horiz+dist/2,ver,dist/2);
        decompressImage(img,t->bottomLeft,horiz,ver+dist/2,dist/2);
        decompressImage(img,t->bottomRight,horiz+dist/2,ver+dist/2,dist/2);
    }
    return;
}

//---------------------DECOMPRESSION FUNC ENDS----------------------------//





int main()
{
    int opt;
    char *fileName;
    char name[50];
    pixels **img;
    int height;
    int width;
    int factor;
    quadTree *t;
    unsigned int index = 0;
    int i;

    //compression 
    img = read(&height, &width, "test1.ppm");
    printf("Image Read Successfully!\n");
    getMean(img, t, 0, 0, width);
    printf("Enter the Threshold Value:\n");
    scanf("%d", &factor);
    compressImage(img, &t, 0, 0, width, factor);
    printf("Image Compressed Successfully!\n");
    // Vector of pointers to the tree nodes
    quadTree **vector = malloc(sizeof(quadTree *));
    traversal(t, &vector, &index);

    quadInfo *vec = malloc(sizeof(quadInfo) * index);
    copy_to_vector(vector, &vec, index);

    for (i = 0; i < index; i++)
        free(vector[i]);
    free(vector);

    // Write vector in a file
    FILE *f = fopen("bin2.out", "wb");

    unsigned int count = 0;
    for (i = 0; i < index; i++)
    {
        if (vec[i].topLeft == -1)
            count++;
    }

    fwrite(&count, sizeof(int), 1, f);
    fwrite(&index, sizeof(int), 1, f);

    for (i = 0; i < index; i++)
    {
        fwrite(&vec[i], sizeof(quadInfo), 1, f);
        if (vec[i].topLeft == -1)
            count++;
    }
    free(vec);

    // Free memory
    for (i = 0; i < height; i++)
        free(img[i]);
    free(img);

    fclose(f);

    //decompression
    

    return 0;
}