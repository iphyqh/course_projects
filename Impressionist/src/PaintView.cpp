//
// paintview.cpp
//
// The code maintaining the painting view of the input images
//

#include "impressionist.h"
#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "paintview.h"
#include "ImpBrush.h"
#include <math.h>
#include <vector>
#include <random>
#include <algorithm>
#include <iterator>
#include <chrono>
#define LEFT_MOUSE_DOWN		1
#define LEFT_MOUSE_DRAG		2
#define LEFT_MOUSE_UP		3
#define RIGHT_MOUSE_DOWN	4
#define RIGHT_MOUSE_DRAG	5
#define RIGHT_MOUSE_UP		6


#ifndef WIN32
#define min(a, b)	( ( (a)<(b) ) ? (a) : (b) )
#define max(a, b)	( ( (a)>(b) ) ? (a) : (b) )
#endif

static int		eventToDo;
static int		isAnEvent=0;

static Point	coord;
double getAngle(Point source, Point target) {


	double tan_val = (double)(target.y - source.y) / (double)(target.x - source.x);
	double theta = atan(tan_val) * 180 / 3.1415926;
	return theta;
	//m_pDoc->setAngle(90);
}
PaintView::PaintView(int			x, 
					 int			y, 
					 int			w, 
					 int			h, 
					 const char*	l)
						: Fl_Gl_Window(x,y,w,h,l)
{
	m_nWindowWidth	= w;
	m_nWindowHeight	= h;

}


// The auto paint function
void PaintView::autoPaint(int space)
{
#ifndef MESA
	// To avoid flicker on some machines.
	glDrawBuffer(GL_FRONT_AND_BACK);
#endif // !MESA


	ImpressionistUI* dlg = m_pDoc->m_pUI;
	int paintWidth = m_pDoc->m_nPaintWidth;
	int paintHeight = m_pDoc->m_nPaintHeight;

	std::vector<int> seqi;
	std::vector<int> seqj;
	coord.x = 0;
	coord.y = 0;

	for (int i = 0; i <= paintWidth * paintHeight; )
	{
		seqi.push_back(i);
		i += space;
	}


	std::random_shuffle(seqi.begin(), seqi.end());

	for (auto i = seqi.begin(); i != seqi.end(); ++i) {
			int y = *i / paintWidth;
			int x = *i - y * paintWidth;
			Point source(x + m_nStartCol, m_nEndRow - y);
			Point target(x, m_nWindowHeight - y);

			m_pDoc->m_pCurrentBrush->BrushBegin(source, target);

	}

	/* Not perfect randomness*/
	/*for (int i = 0; i <= paintWidth; )
	{
		seqi.push_back(i);
		i += space;
	}
	for (int j = 0; j <= paintHeight;)
	{
		seqj.push_back(j);
		j += space;
	}



	std::random_shuffle(seqi.begin(), seqi.end());

	std::random_shuffle(seqj.begin(), seqj.end());

	for (auto i = seqi.begin(); i != seqi.end(); ++i) {
		for (auto j = seqj.begin(); j != seqj.end(); ++j)
		{	
			Point source(*i + m_nStartCol, m_nEndRow - *j);
			Point target(*i, m_nWindowHeight - *j);
			
			m_pDoc->m_pCurrentBrush->BrushBegin(source, target);
		}
		
	}*/

	 


	//Below has no random sequence of paint
	//for (int i = 0; i < paintWidth; ) 
	//{
	//	for (int j = 0; j < paintHeight;) 
	//	{
	//		Point source(i + m_nStartCol, m_nEndRow - j);
	//		Point target(i, m_nWindowHeight - j);

	//		m_pDoc->m_pCurrentBrush->BrushBegin(source, target);
	//		/*SaveCurrentContent();*/

	//		j = j + space;
	//	}
	//	i = i + space;
	//}

	redraw();
	SaveCurrentContent();

#ifndef MESA
	// To avoid flicker on some machines.
	glDrawBuffer(GL_BACK);
#endif // !MESA

}


void PaintView::autoPaintEdge(int threshold)
{
#ifndef MESA
	// To avoid flicker on some machines.
	glDrawBuffer(GL_FRONT_AND_BACK);
#endif // !MESA


	// A modified version of autoPaint
	ImpressionistUI* dlg = m_pDoc->m_pUI;
	int paintWidth = m_pDoc->m_nPaintWidth;
	int paintHeight = m_pDoc->m_nPaintHeight;

	std::vector<int> seqi;
	std::vector<int> seqj;
	coord.x = 0;
	coord.y = 0;

	for (int i = 0; i <= paintWidth * paintHeight; )
	{
		seqi.push_back(i);
		i += 1;
	}

	GLubyte color[4] = { 255, 255, 255, 255 };
	GLubyte black[4] = { 0, 0, 0, 0 };
	glPointSize(1);

	for (auto i = seqi.begin(); i != seqi.end(); ++i) {
		int y = *i / paintWidth;
		int x = *i - y * paintWidth;
		Point source(x + m_nStartCol, m_nEndRow - y);
		Point target(x, m_nWindowHeight - y);

		glBegin(GL_POINTS);

		if (sqrt((getSobelX(source) * getSobelX(source) + getSobelY(source) * getSobelY(source))) > threshold)
		{		
			glColor4ubv(color);
		}

		else 
		{
			glColor4ubv(black);
		}

		glVertex2d(target.x, target.y);

		glEnd();

	}


	redraw();
	SaveCurrentContent();

#ifndef MESA
	// To avoid flicker on some machines.
	glDrawBuffer(GL_BACK);
#endif // !MESA
}



void PaintView::draw()
{
	#ifndef MESA
	// To avoid flicker on some machines.
	glDrawBuffer(GL_FRONT_AND_BACK);
	#endif // !MESA

	if(!valid())
	{

		glClearColor(0.7f, 0.7f, 0.7f, 1.0);

		// We're only using 2-D, so turn off depth 
		glDisable( GL_DEPTH_TEST );

		ortho();

		glClear( GL_COLOR_BUFFER_BIT );
	}

	Point scrollpos;// = GetScrollPosition();
	scrollpos.x = 0;
	scrollpos.y	= 0;

	m_nWindowWidth	= w();
	m_nWindowHeight	= h();

	int drawWidth, drawHeight;
	drawWidth = min( m_nWindowWidth, m_pDoc->m_nPaintWidth );
	drawHeight = min( m_nWindowHeight, m_pDoc->m_nPaintHeight );

	int startrow = m_pDoc->m_nPaintHeight - (scrollpos.y + drawHeight);
	if ( startrow < 0 ) startrow = 0;

	m_pPaintBitstart = m_pDoc->m_ucPainting + 
		3 * ((m_pDoc->m_nPaintWidth * startrow) + scrollpos.x);

	m_nDrawWidth	= drawWidth;
	m_nDrawHeight	= drawHeight;

	m_nStartRow		= startrow;
	m_nEndRow		= startrow + drawHeight;
	m_nStartCol		= scrollpos.x;
	m_nEndCol		= m_nStartCol + drawWidth;



	if ( m_pDoc->m_ucPainting && !isAnEvent) 
	{
		RestoreContent();

	}

	if ( m_pDoc->m_ucPainting && isAnEvent) 
	{

		// Clear it after processing.
		isAnEvent	= 0;	

		Point source( coord.x + m_nStartCol, m_nEndRow - coord.y );
		Point target( coord.x, m_nWindowHeight - coord.y );
		ImpressionistUI* dlg = m_pDoc->m_pUI;
		// This is the event handler
		switch (eventToDo) 
		{
		case LEFT_MOUSE_DOWN:
			updateUndo();
			m_pDoc->m_pCurrentBrush->BrushBegin( source, target );
			leftClick[0] = target;
			leftClick[1] = target;
			break;
		case LEFT_MOUSE_DRAG:
			leftClick[1] = target;
			
			if (m_pDoc->m_pCurrentDirection == 2) {
				m_pDoc->setLineAngle(getAngle(leftClick[0], leftClick[1]));
			}

			leftClick[0] = target;

			m_pDoc->m_pCurrentBrush->BrushMove( source, target );
			break;
		case LEFT_MOUSE_UP:
			m_pDoc->m_pCurrentBrush->BrushEnd( source, target );
			SaveCurrentContent();
			RestoreContent();
			break;
		case RIGHT_MOUSE_DOWN:
			rightClick[0] = target;
			break;
		case RIGHT_MOUSE_DRAG:
			RestoreContent();
			//then draw a new red line
			glLineWidth((float)1);
			glBegin(GL_LINES);
			
			glColor3ub(255, 0, 0); //set the color to red
			glVertex2d(rightClick[0].x, rightClick[0].y);
			glVertex2d(target.x, target.y);
			glEnd();
			break;
		case RIGHT_MOUSE_UP:
			RestoreContent();
			//dlg->setAngle(90);
			rightClick[1] = target;
			if (m_pDoc->m_pCurrentDirection == 0)
				dlg->setAngle(getAngle(rightClick[0], rightClick[1]));
			break;

		default:
			printf("Unknown event!!\n");		
			break;
		}
	}

	glFlush();

	#ifndef MESA
	// To avoid flicker on some machines.
	glDrawBuffer(GL_BACK);
	#endif // !MESA

}


int PaintView::handle(int event)
{
	ImpressionistUI* dlg = m_pDoc->m_pUI;
	switch(event)
	{
	case FL_ENTER:
	    redraw();
		break;
	case FL_PUSH:
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		if (Fl::event_button()>1)
			eventToDo=RIGHT_MOUSE_DOWN;
		else
			eventToDo=LEFT_MOUSE_DOWN;
		isAnEvent=1;
		redraw();
		break;
	case FL_DRAG:
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		if (Fl::event_button()>1)
			eventToDo=RIGHT_MOUSE_DRAG;
		else
			eventToDo=LEFT_MOUSE_DRAG;
		isAnEvent=1;
		redraw();
		break;
	case FL_RELEASE:
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		if (Fl::event_button()>1)
			eventToDo=RIGHT_MOUSE_UP;
		else
			eventToDo=LEFT_MOUSE_UP;
		isAnEvent=1;
		redraw();
		break;
	case FL_MOVE:
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		
		/*glPointSize((float)30);
		glBegin(GL_POINTS);
		glColor3ub(255, 0, 0);
		glVertex2d(coord.x, coord.y);
		glEnd();*/
		dlg->drawMarker(coord);
		break;
		
	default:
		return 0;
		break;

	}



	return 1;
}

void PaintView::refresh()
{
	redraw();
}

void PaintView::resizeWindow(int width, int height)
{
	resize(x(), y(), width, height);
}

void PaintView::SaveCurrentContent()
{
	// Tell openGL to read from the front buffer when capturing
	// out paint strokes
	glReadBuffer(GL_FRONT);

	glPixelStorei( GL_PACK_ALIGNMENT, 1 );
	glPixelStorei( GL_PACK_ROW_LENGTH, m_pDoc->m_nPaintWidth );
	
	glReadPixels( 0, 
				  m_nWindowHeight - m_nDrawHeight, 
				  m_nDrawWidth, 
				  m_nDrawHeight, 
				  GL_RGB, 
				  GL_UNSIGNED_BYTE, 
				  m_pPaintBitstart );
}


void PaintView::RestoreContent()
{
	glDrawBuffer(GL_BACK);

	glClear( GL_COLOR_BUFFER_BIT );

	glRasterPos2i( 0, m_nWindowHeight - m_nDrawHeight );
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	glPixelStorei( GL_UNPACK_ROW_LENGTH, m_pDoc->m_nPaintWidth );
	glDrawPixels( m_nDrawWidth, 
				  m_nDrawHeight, 
				  GL_RGB, 
				  GL_UNSIGNED_BYTE, 
				  m_pPaintBitstart);

//	glDrawBuffer(GL_FRONT);
}


void PaintView::updateUndo() {
	//unsigned char* tmp = m_pDoc->m_ucPaintingUndo;
	delete[] m_pDoc->m_ucPaintingUndo;
	m_pDoc->m_ucPaintingUndo = new unsigned char[m_pDoc->m_nPaintHeight * m_pDoc->m_nPaintWidth * 3];
	memcpy(m_pDoc->m_ucPaintingUndo, m_pDoc->m_ucPainting, m_pDoc->m_nPaintHeight * m_pDoc->m_nPaintWidth * 3);


}


double PaintView::BlurredPoint(const Point source)
{
	ImpressionistDoc* pDoc = m_pDoc;

	Point* points = NULL;
	GLubyte** pixels = NULL;
	double* gs = NULL; // gs stands for grayscale

	points = new Point[9];
	pixels = new GLubyte * [9];
	gs = new double[9]();


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

	for (int i = 0; i <= 8; i++) {
		gs[i] = 0.299 * pixels[i][0];
	}

	for (int i = 0; i <= 8; i++) {
		for (int channel = 1; channel < 3; channel++) {
			if (channel == 1)
			{
				gs[i] += 0.587 * pixels[i][channel];
			}
			if (channel == 2)
			{
				gs[i] += 0.114 * pixels[i][channel];
			}
		}
	}

	double BlurredSource = 0;


	BlurredSource = 1 * gs[0] + 2 * gs[1] + 1 * gs[2] + 2 * gs[3] + 4 * gs[4] + 2 * gs[5] + 1 * gs[6] + 2 * gs[7] + 1 * gs[8];

	BlurredSource = BlurredSource / 16;

	delete[] points;
	delete[] pixels;
	delete[] gs;

	return BlurredSource;

}


double PaintView::getSobelX(const Point source)
{
	ImpressionistDoc* pDoc = m_pDoc;

	Point* points = NULL;
	GLubyte** pixels = NULL;
	double* gs = NULL; // gs stands for grayscale

	points = new Point[9];
	pixels = new GLubyte * [9];
	gs = new double[9]();

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
		gs[i] = BlurredPoint(points[i]);


	double Gy = (gs[0] * (1) + gs[1] * 2 + gs[2] * 1 + gs[6] * (-1) + gs[7] * (-2) + gs[8] * (-1));

	delete[] points;
	delete[] pixels;
	delete[] gs;

	return Gy;


}

double PaintView::getSobelY(const Point source)
{
	ImpressionistDoc* pDoc = m_pDoc;

	Point* points = NULL;
	GLubyte** pixels = NULL;
	double* gs = NULL; // gs stands for grayscale

	points = new Point[9];
	pixels = new GLubyte * [9];
	gs = new double[9]();

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
		gs[i] = BlurredPoint(points[i]);


	double Gx = (gs[0] * (-1) + gs[2] * 1 + gs[3] * (-2) + gs[5] * 2 + gs[6] * (-1) + gs[8] * 1);

	delete[] points;
	delete[] pixels;
	delete[] gs;

	return Gx;
}
