//
// SharpenBrush.cpp
//
// The implementation of Sharpen Brush. It is a kind of ImpBrush. 
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "sharpenbrush.h"

extern float frand();

SharpenBrush::SharpenBrush( ImpressionistDoc* pDoc, char* name ) :
	ImpBrush(pDoc,name)
{
}

void SharpenBrush::BrushBegin( const Point source, const Point target )
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg=pDoc->m_pUI;

	int size = pDoc->getSize();



	glPointSize( (float)size );

	BrushMove( source, target );
}

void SharpenBrush::BrushMove( const Point source, const Point target )
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
		SetSharpenColor( source );

		glVertex2d( target.x, target.y );

	glEnd();
}

void SharpenBrush::BrushEnd( const Point source, const Point target )
{
	// do nothing so far
}

double* SharpenBrush::SharpenedPoint(const Point source)
{
	ImpressionistDoc* pDoc = GetDocument();

	Point* points = NULL;
	GLubyte** pixels = NULL;
	double* IndicatorColor = NULL; 
	double* color = NULL;


	points = new Point[9];
	pixels = new GLubyte * [9];
	IndicatorColor = new double[3]();
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
		IndicatorColor[i] = (( 0 * pixels[0][i] - 1 * pixels[1][i] + 0 * pixels[2][i] - 1 * pixels[3][i] + 5 * pixels[4][i] - 1 * pixels[5][i] + 0 * pixels[6][i] - 1 * pixels[7][i] + 0 * pixels[8][i]));
	}


		for (int i = 0; i <= 2; i++)
		{
			
				if (IndicatorColor[i] < 0)
					color[i] = 0;
				else if (IndicatorColor[i] > 255)
					color[i] = 255;
				else 
					color[i] = IndicatorColor[i];
		}
		
		return color;
}

void SharpenBrush::SetSharpenColor(const Point source)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;


	float alpha = pDoc->getAlpha();
	GLubyte alphaValue = (GLubyte)(alpha * 255);

	GLubyte color[4] = { SharpenedPoint(source)[0],SharpenedPoint(source)[1],SharpenedPoint(source)[2], alphaValue};


	//float RedScalar = pDoc->getRedScalar();
	//float GreenScalar = pDoc->getGreenScalar();
	//float BlueScalar = pDoc->getBlueScalar();

	//color[0] = RedScalar * color[0];
	//color[1] = GreenScalar * color[1];
	//color[2] = BlueScalar * color[2];

	glColor4ubv(color);
	glFlush();
}

