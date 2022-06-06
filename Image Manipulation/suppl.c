#include "suppl.h"

//-----------------------IMG MANIPULATION FUNC-----------------------------//

// reads the .ppm file and stores the data in 2D matrix form
pixels **read(int *height, int *width, char *file)
{
    FILE *f;
    char magic_num[3];
    int color;
    int i;

    f = fopen(file, "rb");

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

// gives the output file in .ppm format
void outputFile(pixels **mat, char *file, int size)
{

    FILE *f = fopen(file, "w");

    fprintf(f, "P6\n");
    fprintf(f, "%d %d\n", size, size);
    fprintf(f, "255\n");

    int i;
    for (i = 0; i < size; i++)
    {
        fwrite(mat[i], sizeof(pixels), size, f);
    }
    fclose(f);
}

int getMean(pixels **matrix, qtNode **t, int x, int y, int size)
{
    int i, j;
    unsigned long long int blue = 0, green = 0, red = 0, mean = 0;

    (*t) = malloc(sizeof(qtNode));
    (*t)->area = size * size;

    for (i = y; i < y + size; i++)
        for (j = x; j < x + size; j++)
        {
            blue = blue + matrix[i][j].blue;
            green = green + matrix[i][j].green;
            red = red + matrix[i][j].red;
        }

    blue = blue / ((*t)->area);
    red = red / ((*t)->area);
    green = green / ((*t)->area);

    (*t)->p.blue = blue;
    (*t)->p.red = red;
    (*t)->p.green = green;

    // Compute score

    for (i = y; i < y + size; i++)
    {
        for (j = x; j < x + size; j++)
        {
            mean = mean + ((red - matrix[i][j].red) * (red - matrix[i][j].red)) + ((green - matrix[i][j].green) * (green - matrix[i][j].green)) + ((blue - matrix[i][j].blue) * (blue - matrix[i][j].blue));
        }
    }

    mean = mean / (3 * (*t)->area);

    return mean;
}

//--------------------------QUAD TREE FUNCTIONS------------------------------//

// stores the quad tree info in array

void traversal(qtNode *node, qtNode **vector[], unsigned int *index)
{
    if (node != NULL)
    {
        if ((*index) > 0)
            (*vector) = realloc((*vector), sizeof(qtNode *) * ((*index) + 1));

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

// transfers the contents to qtInfo
void copyToArr(qtNode **vp, qtInfo **v, int index)
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

// reads the data from matrix
void readTree(qtInfo *vec, qtNode **node, int i)
{
    (*node) = malloc(sizeof(qtNode));

    (*node)->p.red = vec[i].red;
    (*node)->p.blue = vec[i].blue;
    (*node)->p.green = vec[i].green;
    (*node)->area = vec[i].area;
    (*node)->index = i;

    if (vec[i].topLeft != -1 && vec[i].topRight != -1 && vec[i].bottomLeft != -1 && vec[i].bottomRight != -1)
    {
        readTree(vec, &(*node)->topLeft, vec[i].topLeft);
        readTree(vec, &(*node)->topRight, vec[i].topRight);
        readTree(vec, &(*node)->bottomLeft, vec[i].bottomLeft);
        readTree(vec, &(*node)->bottomRight, vec[i].bottomRight);
    }
    else
    {
        (*node)->topLeft = NULL;
        (*node)->topRight = NULL;
        (*node)->bottomLeft = NULL;
        (*node)->bottomRight = NULL;
    }
}

void destroyTree(qtNode** t)
{
    if(!t)
    {
        destroyTree(&(*t)->topLeft);
        destroyTree(&(*t)->topRight);
        destroyTree(&(*t)->bottomLeft);
        destroyTree(&(*t)->bottomRight);
    }
    free(*t);
}

//-----------------------------OTHER FUNCS---------------------------------//
int min(int a,int b)
{
    if(a>b)
    {
        return a;
    }
    else
    {
        return b;
    }
}