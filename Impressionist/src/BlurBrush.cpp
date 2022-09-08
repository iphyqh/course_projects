//
// BlurBrush.cpp
//
// The implementation of Blur Brush. It is a kind of ImpBrush. 
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "blurbrush.h"

extern float frand();

BlurBrush::BlurBrush( ImpressionistDoc* pDoc, char* name ) :
	ImpBrush(pDoc,name)
{
}

void BlurBrush::BrushBegin( const Point source, const Point target )
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg=pDoc->m_pUI;

	int size = pDoc->getSize();



	glPointSize( (float)size );

	BrushMove( source, target );
}

void BlurBrush::BrushMove( const Point source, const Point target )
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg=pDoc->m_pUI;

	if ( pDoc == NULL ) {
		printf( "PointBrush::BrushMove  document is NULL\n" );
		return;
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBegin( GL_POINTS );
		SetBlurColor( source );

		glVertex2d( target.x, target.y );

	glEnd();
}

void BlurBrush::BrushEnd( const Point source, const Point target )
{
	// do nothing so far
}

double* BlurBrush::BlurredPoint(const Point source)
{
	ImpressionistDoc* pDoc = GetDocument();

	Point* points = NULL;
	GLubyte** pixels = NULL;
	double* color = NULL; // gs stands for grayscale

	points = new Point[9];
	pixels = new GLubyte * [9];
	color = new double[3]();


	points[0] = Point(source.x - 1, source.y - 1);
	points[1] = Point(source.x, source.y - 1);
	points[2] = Point(source.x, source.y - 1);
	points[3] = Point(source.x - 1, source.y);
	points[4] = source;
	points[5] = Point(source.x + 1, source.y);
	points[6] = Point(source.x - 1, source.y + 1);
	points[7] = Point(source.x, source.y + 1);
	points[8] = Point(source.x + 1, source.y + 1);



	for (int i = 0; i <= 8; i++)
		pixels[i] = pDoc->GetOriginalPixel(points[i]);


	for (int i = 0; i <= 2; i++)
	{
		color[i] = 1 * pixels[0][i] + 2 * pixels[1][i] + 1 * pixels[2][i] + 2 * pixels[3][i] + 4 * pixels[4][i] + 2 * pixels[5][i] + 1 * pixels[6][i] + 2 * pixels[7][i] + 1 * pixels[8][i];
	}
	for (int i = 0; i <= 2; i++) 
	{
		color[i] = color[i] / 16;
	}

	return color;

}

void BlurBrush::SetBlurColor(const Point source)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;


	float alpha = pDoc->getAlpha();
	GLubyte alphaValue = (GLubyte)(alpha * 255);

	GLubyte color[4] = { BlurredPoint(source)[0], BlurredPoint(source)[1], BlurredPoint(source)[2], alphaValue};


	//float RedScalar = pDoc->getRedScalar();
	//float GreenScalar = pDoc->getGreenScalar();
	//float BlueScalar = pDoc->getBlueScalar();

	//color[0] = RedScalar * color[0];
	//color[1] = GreenScalar * color[1];
	//color[2] = BlueScalar * color[2];

	glColor4ubv(color);
	glFlush();
}

