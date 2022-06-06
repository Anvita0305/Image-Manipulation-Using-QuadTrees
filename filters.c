#include "union.h"

void grayScale(qtNode* t,qtNode** res)
{
    pixels** r=malloc(sizeof(pixels));
    (*res)=malloc(sizeof(qtNode));
    if(t->topLeft!=NULL)
    {
        grayScale(t->topLeft,&(*res)->topLeft);
        grayScale(t->topRight,&(*res)->topRight);
        grayScale(t->bottomLeft,&(*res)->bottomLeft);
        grayScale(t->bottomRight,&(*res)->topLeft);
    }
    (*res)->p.red=t->p.red*0.3;
    (*res)->p.blue=t->p.green*0.59;
    (*res)->p.green=t->p.blue*0.11;
    return;
}

void negativeImage(qtNode* t,qtNode** res)
{
    pixels** r=malloc(sizeof(pixels));
    (*res)=malloc(sizeof(qtNode));
    if(t->topLeft!=NULL)
    {
        negativeImage(t->topLeft,&(*res)->topLeft);
        negativeImage(t->topRight,&(*res)->topRight);
        negativeImage(t->bottomLeft,&(*res)->bottomLeft);
        negativeImage(t->bottomRight,&(*res)->topLeft);
    }
    (*res)->p.red=255-t->p.red;
    (*res)->p.blue=255-t->p.green;
    (*res)->p.green=255-t->p.blue;
    return;
}

void getTint(qtNode* t,qtNode** res,int R,int G,int B)
{
    if(R>255)
}