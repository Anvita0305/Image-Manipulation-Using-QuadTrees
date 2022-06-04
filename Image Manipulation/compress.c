#include "compress.h"

//------------------------COMPRESSION FUNCTION-------------------------------//
void compressImage(pixels **matrix, qtNode **t, int x, int y, int size, int reqFactor)
{
    int mean = getMean(matrix, t, x, y, size);
    int i, j;
    unsigned long long int blue = 0, green = 0, red = 0;

    (*t) = malloc(sizeof(qtNode));
    (*t)->area = size * size;

    for (i = y; i < y + size; i++)
    {
        for (j = x; j < x + size; j++)
        {
            blue = blue + matrix[i][j].blue;
            green = green + matrix[i][j].green;
            red = red + matrix[i][j].red;
        }
    }

    blue = blue / ((*t)->area);
    red = red / ((*t)->area);
    green = green / ((*t)->area);

    (*t)->p.blue = blue;
    (*t)->p.red = red;
    (*t)->p.green = green;

    // Traversal condition
    if (mean > reqFactor)
    {
        compressImage(matrix, &(*t)->topLeft, x, y, size / 2, reqFactor);
        compressImage(matrix, &(*t)->topRight, x + (size / 2), y, size / 2, reqFactor);
        compressImage(matrix, &(*t)->bottomRight, x + (size / 2), y + (size / 2), size / 2, reqFactor);
        compressImage(matrix, &(*t)->bottomLeft, x, y + (size / 2), size / 2, reqFactor);

        return;
    }
    else
    {
        (*t)->topRight = NULL;
        (*t)->topLeft = NULL;
        (*t)->bottomLeft = NULL;
        (*t)->bottomRight = NULL;
        return;
    }
}
