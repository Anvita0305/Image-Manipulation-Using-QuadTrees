#include "union.h"

void unionOfImages(qtNode *t1, qtNode *t2, qtNode **res)
{
    (*res) = malloc(sizeof(qtNode));
    if (t1->topLeft != NULL && t2->topLeft != NULL)
    {
        unionOfImages(t1->topLeft, t2->topLeft, &(*res)->topLeft);
        unionOfImages(t1->topRight, t2->topRight, &(*res)->topRight);
        unionOfImages(t1->bottomLeft, t2->bottomLeft, &(*res)->bottomLeft);
        unionOfImages(t1->bottomRight, t2->bottomRight, &(*res)->bottomRight);
    }
    else
    {
        if (t1->topLeft == NULL && t2->topLeft != NULL)
        {
            unionOfImages(t1, t2->topLeft, (&(*res)->topLeft));
            unionOfImages(t1, t2->topRight, &(*res)->topRight);
            unionOfImages(t1, t2->bottomLeft, &(*res)->bottomLeft);
            unionOfImages(t1, t2->bottomRight, &(*res)->bottomRight);
        }
        else
        {
            if (t1->topLeft != NULL && t2->topLeft == NULL)
            {
                unionOfImages(t1->topLeft, t2, &(*res)->topLeft);
                unionOfImages(t1->topRight, t2, &(*res)->topRight);
                unionOfImages(t1->bottomLeft, t2, &(*res)->bottomLeft);
                unionOfImages(t1->bottomRight, t2, &(*res)->bottomRight);
            }
            else
            {

                (*res)->topRight = NULL;
                (*res)->topLeft = NULL;
                (*res)->bottomLeft = NULL;
                (*res)->bottomRight = NULL;
            }
        }
    }
    (*res)->area = min(t1->area, t2->area);
    (*res)->p.blue = (t1->p.blue + t2->p.blue) / 2;
    (*res)->p.red = (t1->p.red + t2->p.red) / 2;
    (*res)->p.green = (t1->p.green + t2->p.green) / 2;

    return;
}