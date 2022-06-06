#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// rgb structure
typedef struct pixels
{

    unsigned char red;
    unsigned char green;
    unsigned char blue;

} pixels;

// quad tree structure
typedef struct qtNode
{

    pixels p;
    long long index;
    int area;

    struct qtNode *topLeft;
    struct qtNode *topRight;
    struct qtNode *bottomLeft;
    struct qtNode *bottomRight;

} qtNode;

//structure for storing the information about quad tree array
typedef struct qtInfo
{

    unsigned char blue, green, red;
    int area;
    int topLeft;
    int topRight;
    int bottomLeft;
    int bottomRight;

} qtInfo;


//-----------------------IMG MANIPULATION FUNCTIONS-----------------------------//

pixels **read(int *height, int *width, char *file);
void outputFile(pixels **mat, char *file, int size);
int getMean(pixels **matrix, qtNode **node, int x, int y, int size);


//--------------------------QUAD TREE FUNCTIONS------------------------------//
void traversal(qtNode * node, qtNode ** vector[], unsigned int * index);
void copyToArr(qtNode ** vp, qtInfo ** v, int index);
void readTree(qtInfo * vec, qtNode ** node, int i);
void destroyTree(qtNode** t);

//------------------------OTHER FUNCS--------------------------------------//
int min(int a,int b);