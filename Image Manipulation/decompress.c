#include "decompress.h"

//--------------------DECOMPRESSION FUNCTION----------------------------------//

void decompressImage(qtNode *t, pixels ***matrix, int x, int y, int size)
{
    int i, j;

    // already optimized tree
    if (t->topLeft == NULL && t->topRight == NULL && t->bottomLeft == NULL && t->bottomRight == NULL)
    {
        for (i = y; i < y + size; i++)
        {
            for (j = x; j < x + size; j++)
            {
                (*matrix)[i][j].red = t->p.red;
                (*matrix)[i][j].green = t->p.green;
                (*matrix)[i][j].blue = t->p.blue;
            }
        }
    }
    // decompress till tree is optimized
    else
    {
        decompressImage(t->topLeft, matrix, x, y, size / 2);
        decompressImage(t->topRight, matrix, x + (size / 2), y, size / 2);
        decompressImage(t->bottomRight, matrix, x + (size / 2), y + (size / 2), size / 2);
        decompressImage(t->bottomLeft, matrix, x, y + (size / 2), size / 2);
    }
}