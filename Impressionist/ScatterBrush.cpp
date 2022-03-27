//
// PointBrush.cpp
//
// The implementation of Point Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "ScatterBrush.h"

extern float frand();

ScatterBrush::ScatterBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

void ScatterBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	int size = pDoc->getSize();



	glPointSize(0.1f);

	BrushMove(source, target);
}

void ScatterBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("PointBrush::BrushMove  document is NULL\n");
		return;
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBegin(GL_POINTS);
	SetColor(source);
	int size = pDoc->getSize();
	for (int i = 0; i < 3 * size; i++) {
		int x = target.x + rand() % size - size / 2; int y = target.y + rand() % size - size / 2;
		Point newSource = Point(x, y);
		SetColor(newSource);
		glVertex2d(x, y);
	}
	glEnd();
}

void ScatterBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}
