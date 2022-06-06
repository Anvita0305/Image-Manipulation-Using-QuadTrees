#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "filters.h"

int main(int argc, char *argv[])
{
    // compression
    if (strcmp(argv[1], "-c") == 0)
    {
        unsigned int index = 0;
        unsigned int i;
        int factor, width, height;

        pixels **r;
        qtNode *node = NULL;

        r = read(&height, &width, argv[3]);
        compressImage(r, &node, 0, 0, width, atoi(argv[2]));

        // Vector of pointers to the tree nodes
        qtNode **vector = malloc(sizeof(qtNode *));
        traversal(node, &vector, &index);

        qtInfo *vec = malloc(sizeof(qtInfo) * index);
        copyToArr(vector, &vec, index);

        for (i = 0; i < index; i++)
            free(vector[i]);
        free(vector);

        // Write vector in a file
        FILE *f = fopen(argv[4], "wb");
        // fprintf(f, "P6\n");------------------>segmentation error!
        // fprintf(f, "%d %d\n", width, width);
        // fprintf(f, "255\n");

        unsigned int count = 0;
        for (i = 0; i < index; i++)
        {
            if (vec[i].topLeft == -1)
            {
                count++;
            }
        }

        fwrite(&count, sizeof(int), 1, f);
        fwrite(&index, sizeof(int), 1, f);

        for (i = 0; i < index; i++)
        {
            fwrite(&vec[i], sizeof(qtInfo), 1, f);
            if (vec[i].topLeft == -1)
            {
                count++;
            }
        }
        free(vec);
        fclose(f);
    }
    // decompression
    else if (strcmp(argv[1], "-d") == 0)
    {

        unsigned int index, i;
        unsigned int colors;

        FILE *f = fopen(argv[2], "rb");
        fread(&colors, sizeof(int), 1, f);
        // Read number of nodes
        fread(&index, sizeof(int), 1, f);

        qtInfo *vec = malloc(sizeof(qtInfo) * index);

        // Read node vector
        fread(vec, sizeof(qtInfo), index, f);
        fclose(f);

        qtNode *node = NULL;
        readTree(vec, &node, 0);
        free(vec);

        int size = sqrt(node->area);

        // Alloc pixels matrix
        pixels **mat = (pixels **)malloc(sizeof(pixels *) * size);
        for (i = 0; i < size; i++)
        {
            mat[i] = malloc(sizeof(pixels) * size);
        }

        decompressImage(node, &mat, 0, 0, size);
        outputFile(mat, argv[3], size);
    }
    // mirroring
    else if (strcmp(argv[1], "-r") == 0)
    {
        unsigned int i;
        int factor, width, height;
        pixels **r;
        qtNode *t = NULL;

        factor = atoi(argv[3]);
        r = read(&height, &width, argv[4]);

        compressImage(r, &t, 0, 0, width, factor);

        if (strcmp(argv[2], "w") == 0)
        {
            getWaterImage(&t);
        }
        else if (strcmp(argv[2], "m") == 0)
        {
            getMirrorImage(&t);
        }
        else if (strcmp(argv[2], "l90") == 0)
        {
            rotateLeft(&t);
        }
        else if (strcmp(argv[2], "r90") == 0)
        {
            rotateRight(&t);
        }

        // Alloc p matrix
        pixels **mat = (pixels **)malloc(sizeof(pixels *) * width);
        for (i = 0; i < width; i++)
        {
            mat[i] = malloc(sizeof(pixels) * width);
        }

        decompressImage(t, &mat, 0, 0, width);
        outputFile(mat, argv[5], width);

        for (i = 0; i < width; i++)
        {
            free(mat[i]);
        }
        free(mat);

        for (i = 0; i < height; i++)
        {
            free(r[i]);
        }
        free(r);
    }

    // else if (strcmp(argv[1], "-f") == 0)
    // {
    //     printf("hi");
    //     unsigned int i;
    //     int factor, width, height;
    //     pixels **r;
    //     qtNode *t = NULL;
    //     qtNode *res = NULL;

    //     factor = atoi(argv[3]);
    //     r = read(&height, &width, argv[4]);

    //     compressImage(r, &t, 0, 0, width, factor);

    //     if (strcmp(argv[2], "g") == 0)
    //     {
    //         grayScale(t, &res);
    //     }
    //     //     else if (strcmp(argv[2], "n") == 0)
    //     //     {
    //     //         negativeImage(t,&res);
    //     //     }

    //     // Alloc p matrix
    //     pixels **mat = (pixels **)malloc(sizeof(pixels *) * width);
    //     for (i = 0; i < width; i++)
    //     {
    //         mat[i] = malloc(sizeof(pixels) * width);
    //     }

    //     decompressImage(res, &mat, 0, 0, width);
    //     // outputFile(mat, argv[5], width);
    //     // destroyTree(&t);

    //     // for (i = 0; i < width; i++)
    //     // {
    //     //     free(mat[i]);
    //     // }
    //     // free(mat);

    //     // for (i = 0; i < height; i++)
    //     // {
    //     //     free(r[i]);
    //     // }
    //     // free(r);
    // }

    else
    {
        if (strcmp(argv[1], "-g") == 0)
        {
            unsigned int i;
            int threshold, width, height;

            pixels **r1;
            pixels **r2;

            qtNode *t1 = NULL;
            qtNode *t2 = NULL;

            threshold = atoi(argv[2]);

            r1 = read(&height, &width, argv[3]);
            r2 = read(&height, &width, argv[4]);

            compressImage(r1, &t1, 0, 0, width, threshold);
            compressImage(r2, &t2, 0, 0, width, threshold);

            qtNode *node_overlay = NULL;
            grayScale(t1, &node_overlay);

            // Alloc rgb matrix
            pixels **mat = (pixels **)malloc(sizeof(pixels *) * width);
            for (i = 0; i < width; i++)
            {
                mat[i] = malloc(sizeof(pixels) * width);
            }

            decompressImage(node_overlay, &mat, 0, 0, width);
            outputFile(mat, argv[5], width);

            // Free
            destroyTree(&t1);
            destroyTree(&t2);

            for (i = 0; i < width; i++)
            {
                free(mat[i]);
            }
            free(mat);

            for (i = 0; i < height; i++)
            {
                free(r1[i]);
            }
            free(r1);

            for (i = 0; i < height; i++)
            {
                free(r2[i]);
            }
            free(r2);
        }

        if (strcmp(argv[1], "-n") == 0)
        {
            unsigned int i;
            int threshold, width, height;

            pixels **r1;
            pixels **r2;

            qtNode *t1 = NULL;
            qtNode *t2 = NULL;

            threshold = atoi(argv[2]);

            r1 = read(&height, &width, argv[3]);
            r2 = read(&height, &width, argv[4]);

            compressImage(r1, &t1, 0, 0, width, threshold);
            compressImage(r2, &t2, 0, 0, width, threshold);

            qtNode *node_overlay = NULL;
            negativeImage(t1, &node_overlay);

            // Alloc rgb matrix
            pixels **mat = (pixels **)malloc(sizeof(pixels *) * width);
            for (i = 0; i < width; i++)
            {
                mat[i] = malloc(sizeof(pixels) * width);
            }

            decompressImage(node_overlay, &mat, 0, 0, width);
            outputFile(mat, argv[5], width);

            // Free
            destroyTree(&t1);
            destroyTree(&t2);

            for (i = 0; i < width; i++)
            {
                free(mat[i]);
            }
            free(mat);

            for (i = 0; i < height; i++)
            {
                free(r1[i]);
            }
            free(r1);

            for (i = 0; i < height; i++)
            {
                free(r2[i]);
            }
            free(r2);
        }

        if (strcmp(argv[1], "-s") == 0)
        {
            unsigned int i;
            int threshold, width, height;

            pixels **r1;
            pixels **r2;

            qtNode *t1 = NULL;
            qtNode *t2 = NULL;

            threshold = atoi(argv[2]);

            r1 = read(&height, &width, argv[3]);
            r2 = read(&height, &width, argv[4]);

            compressImage(r1, &t1, 0, 0, width, threshold);
            compressImage(r2, &t2, 0, 0, width, threshold);

            qtNode *node_overlay = NULL;
            sepia(t1, &node_overlay);

            // Alloc rgb matrix
            pixels **mat = (pixels **)malloc(sizeof(pixels *) * width);
            for (i = 0; i < width; i++)
            {
                mat[i] = malloc(sizeof(pixels) * width);
            }

            decompressImage(node_overlay, &mat, 0, 0, width);
            outputFile(mat, argv[5], width);

            // Free
            destroyTree(&t1);
            destroyTree(&t2);

            for (i = 0; i < width; i++)
            {
                free(mat[i]);
            }
            free(mat);

            for (i = 0; i < height; i++)
            {
                free(r1[i]);
            }
            free(r1);

            for (i = 0; i < height; i++)
            {
                free(r2[i]);
            }
            free(r2);
        }

        // union of two images
        else
        {
            if (strcmp(argv[1], "-u") == 0)
            {
                unsigned int i;
                int threshold, width, height;

                pixels **r1;
                pixels **r2;

                qtNode *t1 = NULL;
                qtNode *t2 = NULL;

                threshold = atoi(argv[2]);

                r1 = read(&height, &width, argv[3]);
                r2 = read(&height, &width, argv[4]);

                compressImage(r1, &t1, 0, 0, width, threshold);
                compressImage(r2, &t2, 0, 0, width, threshold);

                qtNode *node_overlay = NULL;
                unionOfImages(t1, t2, &node_overlay);

                // Alloc rgb matrix
                pixels **mat = (pixels **)malloc(sizeof(pixels *) * width);
                for (i = 0; i < width; i++)
                {
                    mat[i] = malloc(sizeof(pixels) * width);
                }

                decompressImage(node_overlay, &mat, 0, 0, width);
                outputFile(mat, argv[5], width);

                // Free
                destroyTree(&t1);
                destroyTree(&t2);

                for (i = 0; i < width; i++)
                {
                    free(mat[i]);
                }
                free(mat);

                for (i = 0; i < height; i++)
                {
                    free(r1[i]);
                }
                free(r1);

                for (i = 0; i < height; i++)
                {
                    free(r2[i]);
                }
                free(r2);
            }
        }
    }
    return 0;
}