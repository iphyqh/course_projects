//
// ScatterLineBrush.cpp
//
// The implementation of Scatter Line Brush. It is a kind of ImpBrush. 
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "ScatterLineBrush.h"
#include <math.h>

#define PI 3.14159265
extern float frand();

ScatterLineBrush::ScatterLineBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

void ScatterLineBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	int size = pDoc->getSize();
	if (dlg->getSizeBool() == TRUE) {
		size = pDoc->getSize() + rand() % 20 + 1;
	}

	BrushMove(source, target);
}

void ScatterLineBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("PointBrush::BrushMove  document is NULL\n");
		return;
	}
	int size = pDoc->getSize();
	int width = pDoc->getWidth();
	int angle = pDoc->getAngle();
	double sinParameter = sin(angle * PI / 180);
	double cosParameter = cos(angle * PI / 180);

	glLineWidth((float)width);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	if (size == 1) {
		glPointSize((float)size);
		glBegin(GL_POINTS);
		SetColor(source);

		glVertex2d(target.x, target.y);

		glEnd();
	}

	if (size > 1) {
		for (int i = 0; i < 10; i++) {
			glBegin(GL_LINES);

			int x = target.x + rand() % size - size / 2; int y = target.y + rand() % size - size / 2;
			Point newSource = Point(x, y);
			SetColor(newSource);
			glVertex2d(x - (int)(size / 2 * cosParameter), y - (int)(size / 2 * sinParameter));
			glVertex2d(x + (int)(size / 2 * cosParameter), y + (int)(size / 2 * sinParameter));

			glEnd();
		}
	}
}

void ScatterLineBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}
