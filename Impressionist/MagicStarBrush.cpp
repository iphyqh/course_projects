//
// CircleBrush.cpp
//
// The implementation of Circle Brush. It is a kind of ImpBrush. 
// 
// The method of drawing a circle comes from the answers of this webpage: https://stackoverflow.com/questions/22444450/drawing-circle-with-opengl
// 

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "MagicStarBrush.h"
#include <math.h>

#define PI 3.14159265

extern float frand();

MagicStarBrush::MagicStarBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

void MagicStarBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	BrushMove(source, target);
}

void MagicStarBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;
	int size = pDoc->getSize();

	if (dlg->getSizeBool() == TRUE) {
		size = pDoc->getSize() + rand() % 20 + 1;
	}

	if (pDoc == NULL) {
		printf("MagicStarBrush::BrushMove  document is NULL\n");
		return;
	}

	
	GLfloat Pi = 2.0f * PI;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (size == 1) {
		glPointSize((float)size);
		glBegin(GL_POINTS);
		SetColor(source);

		glVertex2d(target.x, target.y);

		glEnd();
	}

	if (size > 1)
	{
		glBegin(GL_TRIANGLES);
		SetColor(source);
		//The first set of triangle
		glVertex2f(target.x, target.y+size);
		glVertex2f(target.x-sqrt(3)/2*size, target.y - size/2);
		glVertex2f(target.x + sqrt(3) / 2 * size, target.y - size / 2);
		//The second set of triangle
		glVertex2f(target.x, target.y - size);
		glVertex2f(target.x - sqrt(3) / 2 * size, target.y +size / 2);
		glVertex2f(target.x + sqrt(3) / 2 * size, target.y + size / 2);
		glEnd();
	}
}

void MagicStarBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}

