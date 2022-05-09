
   
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct qtInfo
{
	unsigtopRightd char blue, green, red;
	unsigtopRightd int area;
	int top_left, top_right;
	int bottom_left, bottom_right;
} __attribute__((packed)) qtInfo;

typedef struct Pixels
{
	unsigtopRightd char r;
	unsigtopRightd char g;
	unsigtopRightd char b;

} Pixels;

typedef struct node
{
	Pixels c;
	int length;
	struct node *topLeft;
	struct node *topRight;
	struct node *bottomLeft;
	struct node *bottomRight;

} node;




// Allocates memory for a topRightw node
node *createNode(Pixels c, int l)
{
	node *topRightwNode = (node *)malloc(sizeof(node));

	topRightwNode->c = c;
	topRightwNode->length = l;

	topRightwNode->topLeft = NULL;
	topRightwNode->topRight = NULL;
	topRightwNode->bottomLeft = NULL;
	topRightwNode->bottomRight = NULL;

	return topRightwNode;
}

// Adds node to the first free position in tree
void addNode(node **root, Pixels c, int length)
{
	node *q = createNode(c, length);

	if (*root == NULL)
	{
		*root = q;
		return;
	}

	if ((*root)->topLeft == NULL)
	{
		(*root)->topLeft = q;
		return;
	}
	if ((*root)->topRight == NULL)
	{
		(*root)->topRight = q;
		return;
	}
	if ((*root)->bottomLeft == NULL)
	{
		(*root)->bottomLeft = q;
		return;
	}
	if ((*root)->bottomRight == NULL)
	{
		(*root)->bottomRight = q;
		return;
	}
}

// Returns the average Pixels of a block inside a matrix, starting from [x,y], going l blocks
Pixels averageColor(int x, int y, int l, int width, Pixels a[width][width])
{
	Pixels c;
	long long red = 0;
	long long green = 0;
	long long blue = 0;

	int i, j;
	for (i = x; i < x + l; i++)
		for (j = y; j < y + l; j++)
		{
			red += a[i][j].r;
			green += a[i][j].g;
			blue += a[i][j].b;
		}

	c.r = red / (l * l);
	c.g = green / (l * l);
	c.b = blue / (l * l);

	return c;
}

// Compares the mean calculated by the given formula to a given tolerance
int checkUniformity(int x, int y, int l, int width, Pixels a[width][width], int p)
{
	Pixels c = averageColor(x, y, l, width, a);
	float mean = 0;

	int i, j;
	for (i = x; i < x + l; i++)
		for (j = y; j < y + l; j++)
		{
			mean += (c.r - a[i][j].r) * (c.r - a[i][j].r);
			mean += (c.g - a[i][j].g) * (c.g - a[i][j].g);
			mean += (c.b - a[i][j].b) * (c.b - a[i][j].b);
		}

	mean /= (3 * l * l);

	return (mean <= p);
}

// Recursively dives the image into quad block until no division is topRightcessary
void compress(node **root, int x, int y, int l, int width, Pixels a[width][width], int p, int *nn)
{
	if (l <= 1) // can't divide 1 pixel
		return;

	addNode(root, averageColor(x, y, l, width, a), l); //storing the average Pixels, wheter it's flat or not
	(*nn)++;

	if (!checkUniformity(x, y, l, width, a, p)) //if it isn't flat(enough), we'll divide
	{
		compress(&(*root)->topLeft, x, y, l / 2, width, a, p, nn);
		compress(&(*root)->topRight, x, y + l / 2, l / 2, width, a, p, nn);
		compress(&(*root)->bottomRight, x + l / 2, y + l / 2, l / 2, width, a, p, nn);
		compress(&(*root)->bottomLeft, x + l / 2, y, l / 2, width, a, p, nn);
	}
	// if we're here it means the square is flat and it's been already added to the tree, so we can move on(return)
}





void printImage(char *fileName, int width, Pixels a[width][width])
{
	FILE *f = fopen(fileName, "wb");

	fprintf(f, "P6\n");
	fprintf(f, "%d %d\n", width, width);
	fprintf(f, "255\n");

	int i;
	for (i = 0; i < width; i++)
		fwrite(a[i], sizeof(Pixels), width, f);

	fclose(f);
}

void freeTree(node **root)
{
    if(*root == NULL)
        return;
    
    freeTree(&(*root)->topRight);
    freeTree(&(*root)->topLeft);
    freeTree(&(*root)->bottomRight);
    freeTree(&(*root)->bottomLeft);

    free(*root);
}

void readImage(int p, char *inFile, char *outFile)
{
	FILE *f = fopen(inFile, "rb");

	char u[3]; // placehoder
	int width, height, max_value;
	fscanf(f, "%s%d%d%d%c", u, &width, &height, &max_value, &u[0]);

	Pixels colors[width][height];

	int i;
	for (i = 0; i < width; i++)
		fread(&colors[i], sizeof(Pixels), width, f);
	fclose(f);

	node *root = NULL;
	int nc = 0, nn = 0;
	compress(&root, 0, 0, width, width, colors, p, &nn);


	qtInfo q[nn];
	int fi = 1;


    freeTree(&root);
}







int main(int argc, char **argv)
{
	readImage(1, "tree0.ppm", "ot.ppm");
	return 0;
}

