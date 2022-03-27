//
// PointBrush.cpp
//
// The implementation of Point Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "AlphaBrush.h"

extern float frand();

AlphaBrush::AlphaBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

void AlphaBrush::BrushBegin(const Point source, const Point target)
{
	

	glPointSize(1);
	BrushMove(source, target);

	BrushMove(source, target);
}

void AlphaBrush::BrushMove(const Point source, const Point target)
{
	/*ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("PointBrush::BrushMove  document is NULL\n");
		return;
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBegin(GL_POINTS);
	SetColor(source);

	glVertex2d(target.x, target.y);

	glEnd();*/
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("PointBrush::BrushMove  document is NULL\n");
		return;
	}

	if (pDoc->m_ucAlphaBrush == NULL) {
		fl_alert("No alpha brush loaded");
		return;
	}

	glBegin(GL_POINTS);
	int beginX = target.x - pDoc->m_nAlphaBrushWidth / 2;
	int beginY = target.y - pDoc->m_nAlphaBrushHeight / 2;
	for (int i = 0; i < pDoc->m_nAlphaBrushWidth; ++i)
	{
		for (int j = 0; j < pDoc->m_nAlphaBrushHeight; ++j)
		{

			//get alpha value of the brush
			int alpha = pDoc->m_ucAlphaBrush[3 * (j * pDoc->m_nAlphaBrushWidth + i)];
			//int alpha = pDoc->m_ucAlphaBrush[j * pDoc->m_nAlphaBrushWidth + i];
			//set the color
			SetColor(source, alpha);
			//draw the point
			glVertex2d(beginX + i, beginY + j);
		}
	}
	glEnd();
	glFlush();
}

void AlphaBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}

