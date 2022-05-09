#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define RGB_Comp 255

typedef struct Pixels
{
    unsigned char blue, green, red;
} Pixels;

typedef struct qtInfo
{
    Pixels data;
    int area;
    int top_left, top_right;
    int bottom_left, bottom_right;
} qtInfo;

typedef struct qtNode
{
    Pixels data;
    int area;
    struct qtNode *children[4];
} qtNode;

typedef struct Dimensions
{
    int x, y, size;
} Dimensions;

//----------------------------------Queue-----------------------------------------------------------//
typedef struct queueNode
{
    qtNode *info;
    int index;
    struct queueNode *next;
} queueNode;

typedef struct Queue
{
    queueNode *front;
    queueNode *rear;
} Queue;

//----------------------------------------Quadtree Operations-----------------------------------------//

unsigned char getAverage(unsigned char **img, Dimensions d, int Pixels)
{
    float avg = 0;
    unsigned char ch;
    int i, j;
    for (i = d.x; i < d.x + d.size; i++)
    {
        for (j = d.y + Pixels; j < d.y + d.size; j += 3)
        {
            avg += img[i][j];
        }
    }
    avg = avg / (d.size * d.size);
    ch = (avg);
    return ch;
}

int checkSimilarity(unsigned char **img, Dimensions d)
{
    int i, j;
    int mean = 0;
    unsigned char red, green, blue;
    red = getAverage(img, d, 0);
    green = getAverage(img, d, 1);
    blue = getAverage(img, d, 2);
    for (i = d.x; i < d.x + d.size; i++)
    {
        for (j = d.y; j < d.y + 3 * d.size; j += 3)
        {
            mean += (red - img[i][j]) * (red - img[i][j]);
            mean += (green - img[i][j + 1]) * (green - img[i][j + 1]);
            mean += (blue - img[i][j + 2]) * (blue - img[i][j + 2]);
        }
    }
    mean = mean / 3 * d.size * d.size;
    return mean;
}

void insert(qtNode *temp, unsigned char **img, Dimensions d, int factor) // factor->level by which compression must take place!
{
    int initSize = d.size;
    temp->area = d.size * d.size;
    temp->data.red = getAverage(img, d, 0);
    temp->data.green = getAverage(img, d, 1);
    temp->data.blue = getAverage(img, d, 2);
    if (checkSimilarity(img, d) <= factor) // already optimized tree!(jevdha divide karaychay tya pekdha already kami ahe)
    {
        temp->children[0] = NULL;
        temp->children[1] = NULL;
        temp->children[2] = NULL;
        temp->children[3] = NULL;
    }
    else
    {
        temp->children[0] = malloc(sizeof(qtNode));
        temp->children[1] = malloc(sizeof(qtNode));
        temp->children[2] = malloc(sizeof(qtNode));
        temp->children[3] = malloc(sizeof(qtNode));
        // insert at topLeft
        d.x -= initSize / 2;
        insert(temp->children[0], img, d, factor);
        // insert at topRight
        d.y += initSize * 3 / 2;
        insert(temp->children[1], img, d, factor);
        // insert at bottomLeft
        d.x += initSize / 2;
        insert(temp->children[2], img, d, factor);
        // insert at bottomRight
        d.y -= initSize * 3 / 2;
        insert(temp->children[3], img, d, factor);
    }
}

void buildQuadTree(qtNode **root, unsigned char **img, int size, int factor)
{
    Dimensions d;
    d.x = 0;
    d.y = 0;
    d.size = size;
    (*root) = malloc(sizeof(qtNode));
    insert(*root, img, d, factor);
    return;
}

//----------------------------------PPM Images Reading---------------------------------------------//

unsigned char **imageRead(char *filename, int *size, int *maxRGB)
{
    unsigned char **img;
    char magic_num[3];
    int i, w, h;
    FILE *f = fopen(filename, "rb");
    fread(&magic_num, sizeof(magic_num), 1, f);
    if (magic_num[0] != 'P' || magic_num[1] != '6')
    {
        printf("Unexpected format! \n");
        exit(1);
    }
    fscanf(f, "%d %d", &w, &h);
    if (w != h)
    {
        printf("Height and width must be the same! \n");
        exit(1);
    }
    printf("%d\t",w);
    printf("%d\t",h);
    // fscanf(f, "%d", maxRGB); --------------------->Giving Seg Fault!
    // if(maxRGB != RGB_comp)
    // {
    // 	printf("maxRGB must be %d! \n",RGB_comp);
    // 	exit(1);
    // }
    // fseek(f, 1, SEEK_CUR);
    // fseek(f,0L,SEEK_END);
    // printf("Size is %d",ftell(f));
    img = (unsigned char **)malloc(h * sizeof(unsigned char *));
    for (i = 0; i < h; i++)
    {
        img[i] = (unsigned char *)malloc(3 * w * sizeof(unsigned char));
        fread(img[i], sizeof(unsigned char), 3 * w, f);
    }
    fclose(f);
    // printf("hi");
    size = &h;
    return img;
}

//-------------------------------------------Image Manipulation Operations---------------------------//
void initQueue(Queue q)
{
    q.front = NULL;
    q.rear = NULL;
    return;
}

void enqueue(Queue *q, qtNode *data, int *index)
{
    queueNode *newNode = malloc(sizeof(queueNode));
    newNode->info = data;
    newNode->index = *index;
    newNode->next = NULL;
    if (q->front == NULL)
        q->front = newNode;
    else
        q->rear->next = newNode;
    q->rear = newNode;
    *index += 1;
}

qtNode *dequeue(Queue *q)
{
    queueNode *c = q->front;
    qtNode *data = q->front->info;
    q->front = q->front->next;
    free(c);
    return data;
}

void addNode(qtInfo *root, Queue *q)
{
    int firstChild = q->rear->index + 1; // index from which we can start adding
    queueNode *qnode = q->front;
    int k = qnode->index;
    (root[k]).data.red = qnode->info->data.red;
    (root[k]).data.green = qnode->info->data.green;
    (root[k]).data.blue = qnode->info->data.blue;
    root[k].area = qnode->info->area;
    if (qnode->info->children[0] != NULL)
    {
        root[k].top_left = firstChild;
        root[k].top_right = firstChild + 1;
        root[k].bottom_right = firstChild + 2;
        root[k].bottom_left = firstChild + 3;
    }
    else
    {
        root[k].top_left = -1;
        root[k].top_right = -1;
        root[k].bottom_right = -1;
        root[k].bottom_left = -1;
    }
}

int countNodes(qtNode *root)
{
    int s = 0, i;
    if (root->children[0] == NULL)
        return 1;
    else
    {
        for (i = 0; i < 4; i++)
            s = s + countNodes(root->children[i]);
        return s + 1;
    }
}

void freeQTree(qtNode *node)
{
	if(node == NULL)
		return;
	freeQTree(node->children[0]);
	freeQTree(node->children[1]);
	freeQTree(node->children[2]);
	freeQTree(node->children[3]);
	free(node);
}

void freeMatrix(unsigned char **img, int size)
{
	int i;
	for(i = 0; i < size; i++)
	{
		free(img[i]);
	}
	free(img);
}

void compress(char *ipFile, char *otFile, int factor)
{
    int i, size = 0, maxRGB = 0;
    unsigned char **img;
    qtNode *root = NULL, *tmpnode = NULL;
    img = imageRead(ipFile, &size, &maxRGB);
    buildQuadTree(&root, img, size, factor);
    int numOfNodes;
    numOfNodes = countNodes(root);
    qtInfo *v = malloc(numOfNodes * sizeof(qtInfo));
    Queue q;
    q.front = NULL;
    q.rear = NULL;
    int index = 0;
    enqueue(&q, root, &index);
    while (q.front != NULL)
    {
        addNode(v, &q);
        tmpnode = dequeue(&q);
        if (tmpnode->children[0] != NULL)
        {
            enqueue(&q, tmpnode->children[0], &index);
            enqueue(&q, tmpnode->children[1], &index);
            enqueue(&q, tmpnode->children[2], &index);
            enqueue(&q, tmpnode->children[3], &index);
        }
        free(tmpnode);
    }
    int nrColors = 0;
    for (i = 0; i < numOfNodes; i++)
    {
        if (v[i].top_left == -1)
            nrColors++;
    }
    FILE *fptr = fopen(otFile, "wb");
    if(!fptr)
    {
        printf("Unable to Open the File!\n");
    }
    // printf("%d",nrColors);
    fwrite(&nrColors, sizeof(unsigned int), 1, fptr);
    fwrite(&numOfNodes, sizeof(unsigned int), 1, fptr);
    fwrite(v, sizeof(qtInfo), numOfNodes, fptr);
    fclose(fptr);
    freeMatrix(img, size);
    free(v);
}




int main()
{
    int w, h;
    if ((imageRead("test0.ppm", NULL, NULL)))
    {
        printf("Image Read Successfully!\n");
    }
    else
    {
        printf("Error...Try Again!\n");
    }
    compress("test0.ppm", "test0_output.ppm", 1);
    FILE *fp;
    fp = fopen("test0.ppm", "rb");
    fscanf(fp,"%d %d",&h,&w);
    printf("%d\t",h);
    printf("%d\t",w);
    fseek(fp,0,SEEK_END);
    printf("Size is %d\n",ftell(fp));
    fseek(fp,0,SEEK_SET);
    // Dimensions d;
    // printf("Size of the Image before Compression is %d\n",d.size);
    // printf("Compressed image data:\n");
    FILE *fptr;
    fptr = fopen("test1_output.ppm", "rb");
    fscanf(fptr,"%d %d",&h,&w);
    printf("%d\t",h);
    printf("%d\t",w);
    fseek(fptr,0,SEEK_END);
    printf("Size is %d",ftell(fptr));
    fseek(fptr,0,SEEK_SET);
    // unsigned char **c=imageRead("test1_output.ppm", 0,0);

    return 0;
}