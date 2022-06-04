#include "rotate.h"

// flip the image along getMirrorImage axis

void getWaterImage(qtNode **t)
{
	if ((*t)->topLeft != NULL && (*t)->topLeft != NULL && (*t)->topLeft != NULL && (*t)->topLeft != NULL)
	{
		qtNode *temp;

		getWaterImage(&(*t)->topLeft);
		getWaterImage(&(*t)->topRight);
		getWaterImage(&(*t)->bottomRight);
		getWaterImage(&(*t)->bottomLeft);

		temp = (*t)->topLeft;
		(*t)->topLeft = (*t)->bottomLeft;
		(*t)->bottomLeft = temp;

		temp = (*t)->topRight;
		(*t)->topRight = (*t)->bottomRight;
		(*t)->bottomRight = temp;
	}
	// empty tree condition
	else
	{
		return;
	}
}

// flip the image along getWaterImage axis

void getMirrorImage(qtNode **node)
{
	if ((*node)->topLeft != NULL && (*node)->topLeft != NULL && (*node)->topLeft != NULL && (*node)->topLeft != NULL)
	{
		qtNode *temp;

		getMirrorImage(&(*node)->topLeft);
		getMirrorImage(&(*node)->topRight);
		getMirrorImage(&(*node)->bottomRight);
		getMirrorImage(&(*node)->bottomLeft);

		temp = (*node)->topLeft;
		(*node)->topLeft = (*node)->topRight;
		(*node)->topRight = temp;

		temp = (*node)->bottomLeft;
		(*node)->bottomLeft = (*node)->bottomRight;
		(*node)->bottomRight = temp;
	}
	// empty tree condition
	else
	{
		return;
	}
}
