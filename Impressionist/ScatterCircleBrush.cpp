//
// ScatterCircleBrush.cpp
//
// The implementation of Scatter Circle Brush. It is a kind of ImpBrush. 
// 
// The method of drawing a circle comes from the answers of this webpage: https://stackoverflow.com/questions/22444450/drawing-circle-with-opengl
// 

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "scattercirclebrush.h"
#include <math.h>

#define PI 3.14159265

extern float frand();

ScatterCircleBrush::ScatterCircleBrush( ImpressionistDoc* pDoc, char* name ) :
	ImpBrush(pDoc,name)
{
}

void ScatterCircleBrush::BrushBegin( const Point source, const Point target )
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg=pDoc->m_pUI;
	for (int i = 0; i < 5; i++)
		BrushMove( source, target );
}

void ScatterCircleBrush::BrushMove( const Point source, const Point target )
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg=pDoc->m_pUI;
	int size = pDoc->getSize();
	
	if (dlg->getSizeBool() == TRUE) {
		size = pDoc->getSize() + rand() % 20 + 1;
	}

	if ( pDoc == NULL ) {
		printf( "CircleBrush::BrushMove  document is NULL\n" );
		return;
	}

	// Lines 54 ~ 59 referenced the implementation of https://stackoverflow.com/a/58230318
	int i;
	int triangleAmount = 30; //# of triangles used to draw circle

	//GLfloat radius = 0.8f; //radius
	GLfloat twicePi = 2.0f * PI;
	
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
		for (int i = 0; i < 5; i++) {

			glBegin(GL_TRIANGLE_FAN);

			int x = target.x + rand() % size - size / 2; int y = target.y + rand() % size - size / 2;
			Point newSource = Point(x, y);
			SetColor(newSource);
			glVertex2f(x, y); // center of circle
			for (i = 0; i <= triangleAmount; i++) {
				glVertex2f(
					x + ((size / 2) * cos(i * twicePi / triangleAmount)),
					y + ((size / 2) * sin(i * twicePi / triangleAmount))
				);
			}

			glEnd();
			glFlush();

		}
	}
}

void ScatterCircleBrush::BrushEnd( const Point source, const Point target )
{
	// do nothing so far
}

