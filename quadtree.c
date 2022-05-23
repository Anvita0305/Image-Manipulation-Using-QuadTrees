#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

// red green blue struct
typedef struct pixels
{
	unsigned char red;
	unsigned char green;
	unsigned char blue;

} pixels;

// main quad tree structure
typedef struct QuadtreeNode
{

	pixels rgb;
	long long index;
	unsigned int area;
	struct QuadtreeNode *top_left, *top_right;
	struct QuadtreeNode *bottom_left, *bottom_right;

} QuadtreeNode;

// quadtree node
typedef struct vQuadtreeNode
{
	unsigned char blue, green, red;
	unsigned int area;
	int top_left, top_right;
	int bottom_left, bottom_right;
} vQuadtreeNode;

// read a ppm img
pixels **read(int *height, int *width, char *file)
{
	FILE *f;
	char magic_num[3];
	int color;
	int i;

	f = fopen(file, "rb");

	fscanf(f, "%s", magic_num);
	fscanf(f, "%d ", width);
	fscanf(f, "%d\n", height);
	fscanf(f, "%d", &color);

	// char garbage;
	// fread(&garbage, sizeof(char), 1, f);

	pixels **r = (pixels **)malloc(sizeof(pixels *) * (*height));

	for (i = 0; i < (*height); i++)
	{
		r[i] = malloc(sizeof(pixels) * (*width));
		fread(r[i], sizeof(pixels), (*width), f);
	}

	fclose(f);
	return r;
}

// compression func
void compression(pixels **matrix, QuadtreeNode **node, int x, int y, int size, int threshold)
{
	int i, j;
	unsigned long long int blue = 0, green = 0, red = 0, mean = 0;

	(*node) = malloc(sizeof(QuadtreeNode));
	(*node)->area = size * size;

	// Compute medium rgb on the current area

	for (i = y; i < y + size; i++)
		for (j = x; j < x + size; j++)
		{
			blue = blue + matrix[i][j].blue;
			green = green + matrix[i][j].green;
			red = red + matrix[i][j].red;
		}

	blue = blue / (size * size);
	red = red / (size * size);
	green = green / (size * size);

	(*node)->rgb.blue = blue;
	(*node)->rgb.red = red;
	(*node)->rgb.green = green;

	// Compute score

	for (i = y; i < y + size; i++)
		for (j = x; j < x + size; j++)
		{
			mean = mean + ((red - matrix[i][j].red) * (red - matrix[i][j].red)) + ((green - matrix[i][j].green) * (green - matrix[i][j].green)) + ((blue - matrix[i][j].blue) * (blue - matrix[i][j].blue));
		}

	mean = mean / (3 * size * size);

	// Traversal condition

	if (mean > threshold)
	{
		compression(matrix, &(*node)->top_left, x, y, size / 2, threshold);
		compression(matrix, &(*node)->top_right, x + (size / 2), y, size / 2, threshold);
		compression(matrix, &(*node)->bottom_right, x + (size / 2), y + (size / 2), size / 2, threshold);
		compression(matrix, &(*node)->bottom_left, x, y + (size / 2), size / 2, threshold);

		return;
	}
	else
	{
		(*node)->top_right = NULL;
		(*node)->top_left = NULL;
		(*node)->bottom_left = NULL;
		(*node)->bottom_right = NULL;

		return;
	}
}

void traversal(QuadtreeNode *node, QuadtreeNode **vector[], unsigned int *index)
{
	if (node != NULL)
	{
		if ((*index) > 0)
			(*vector) = realloc((*vector), sizeof(QuadtreeNode *) * ((*index) + 1));

		(*vector)[(*index)] = node;
		node->index = (*index);
		(*index)++;

		traversal(node->top_left, vector, index);
		traversal(node->top_right, vector, index);
		traversal(node->bottom_right, vector, index);
		traversal(node->bottom_left, vector, index);
	}
	else
		return;
}

void copy_to_vector(QuadtreeNode **vp, vQuadtreeNode **v, int index)
{

	unsigned int i;

	for (i = 0; i < index; i++)
	{
		(*v)[i].red = vp[i]->rgb.red;
		(*v)[i].blue = vp[i]->rgb.blue;
		(*v)[i].green = vp[i]->rgb.green;
		(*v)[i].area = vp[i]->area;

		if (vp[i]->top_left != NULL)
			(*v)[i].top_left = vp[i]->top_left->index;
		else
			(*v)[i].top_left = -1;

		if (vp[i]->top_right != NULL)
			(*v)[i].top_right = vp[i]->top_right->index;
		else
			(*v)[i].top_right = -1;

		if (vp[i]->bottom_right != NULL)
			(*v)[i].bottom_right = vp[i]->bottom_right->index;
		else
			(*v)[i].bottom_right = -1;

		if (vp[i]->bottom_left != NULL)
			(*v)[i].bottom_left = vp[i]->bottom_left->index;
		else
			(*v)[i].bottom_left = -1;
	}
}

void write_ppm(pixels **mat, char *file, int size)
{

	FILE *f = fopen(file, "w");

	fprintf(f, "P6\n");
	fprintf(f, "%d %d\n", size, size);
	fprintf(f, "255\n");

	int i;
	for (i = 0; i < size; i++)
		fwrite(mat[i], sizeof(pixels), size, f);

	fclose(f);
}

//destroy tree
void destroy_tree(QuadtreeNode **node)
{
	if ((*node)->top_left != NULL && (*node)->top_left != NULL && (*node)->top_left != NULL && (*node)->top_left != NULL)
	{
		destroy_tree(&(*node)->top_left);
		destroy_tree(&(*node)->top_right);
		destroy_tree(&(*node)->bottom_right);
		destroy_tree(&(*node)->bottom_left);
	}
	free((*node));
}

int main()
{

	unsigned int index = 0;
	unsigned int i;
	int width, height, fact;
	char name[50];

	pixels **r;
	QuadtreeNode *node = NULL;

	r = read(&height, &width, "test0.ppm");
	printf("Enter the Compression factor:\n");
	scanf("%d", &fact);
	compression(r, &node, 0, 0, width, fact);

	// Vector of pointers to the tree nodes
	QuadtreeNode **vector = malloc(sizeof(QuadtreeNode *));
	traversal(node, &vector, &index);

	vQuadtreeNode *vec = malloc(sizeof(vQuadtreeNode) * index);
	copy_to_vector(vector, &vec, index);

	for (i = 0; i < index; i++)
		free(vector[i]);
	free(vector);

	// Write vector in a file
	printf("Enter Compressed Img Name\n");
	scanf("%s", name);
	FILE *f = fopen(name, "wb");

	fprintf(f, "P6\n");
	fprintf(f, "%d %d\n", 256, 256);
	fprintf(f, "255\n");

	unsigned int count = 0;
	for (i = 0; i < index; i++)
	{
		if (vec[i].top_left == -1)
			count++;
	}

	fwrite(&count, sizeof(int), 1, f);
	fwrite(&index, sizeof(int), 1, f);

	if (name)
	{
		printf("Image Compressed Successfully!\n");
	}

	for (i = 0; i < index; i++)
	{
		fwrite(&vec[i], sizeof(vQuadtreeNode), 1, f);
		if (vec[i].top_left == -1)
			count++;
	}
	fclose(f);
	destroy_tree(&node);
	return 0;
}
