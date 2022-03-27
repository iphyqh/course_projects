// 
// impressionistDoc.cpp
//
// It basically maintain the bitmap for answering the color query from the brush.
// It also acts as the bridge between brushes and UI (including views)
//

#include <FL/fl_ask.H>

#include "impressionistDoc.h"
#include "impressionistUI.h"

#include "ImpBrush.h"

// Include individual brush headers here.
#include "PointBrush.h"
#include "LineBrush.h"
#include "CircleBrush.h"
#include "ScatterBrush.h"
#include "ScatterLineBrush.h"
#include "ScatterCircleBrush.h"
#include "AlphaBrush.h"
#include "GradientBrush.h"
#include "BlurBrush.h"
#include "SharpenBrush.h"
#include "ScatterMagicStarBrush.h"
#include "MagicStarBrush.h"
#define DESTROY(p)	{  if ((p)!=NULL) {delete [] p; p=NULL; } }

ImpressionistDoc::ImpressionistDoc()
{
	// Set NULL image name as init. 
	m_imageName[0] = '\0';

	m_nWidth = -1;
	m_ucBitmap = NULL;
	m_ucPainting = NULL;
	m_ucPaintingUndo = NULL;
	m_ucAlphaBrush = NULL;
	m_usGradientMap = NULL;
	m_usEdgeMap = NULL;

	// create one instance of each brush
	ImpBrush::c_nBrushCount = NUM_BRUSH_TYPE;
	ImpBrush::c_pBrushes = new ImpBrush * [ImpBrush::c_nBrushCount];

	ImpBrush::c_pBrushes[BRUSH_POINTS] = new PointBrush(this, "Points");

	// Note: You should implement these 5 brushes.  They are set the same (PointBrush) for now
	ImpBrush::c_pBrushes[BRUSH_LINES] = new LineBrush(this, "Lines");

	ImpBrush::c_pBrushes[BRUSH_CIRCLES] = new CircleBrush(this, "Circles");

	ImpBrush::c_pBrushes[BRUSH_SCATTERED_POINTS] = new ScatterBrush(this, "Scattered Points");

	ImpBrush::c_pBrushes[BRUSH_SCATTERED_LINES]	= new ScatterLineBrush( this, "Scattered Lines" );

	ImpBrush::c_pBrushes[BRUSH_SCATTERED_CIRCLES] = new ScatterCircleBrush( this, "Scattered Circles" );

	ImpBrush::c_pBrushes[BRUSH_ALPHA] = new AlphaBrush(this, "Alpha Brash");
	
	ImpBrush::c_pBrushes[BRUSH_GRADIENT] = new GradientBrush(this, "Gradient Brash");
	
	ImpBrush::c_pBrushes[BRUSH_BLUR] = new BlurBrush(this, "Blur Brash");

	ImpBrush::c_pBrushes[BRUSH_SHARPEN] = new SharpenBrush(this, "Sharpen Brash");

	ImpBrush::c_pBrushes[BRUSH_MAGICSTAR] = new MagicStarBrush(this, "Magic Star Brush");

	ImpBrush::c_pBrushes[BRUSH_SCATTERMAGICSTAR] = new ScatterMagicStarBrush(this, "Scatter Magic Star Brush");

	LineBrush::c_pDirections = new int [LineBrush::c_nDirectionCount];
	LineBrush::c_pDirections[RIGHT] = 0;
	LineBrush::c_pDirections[GRADIENT] = 1;
	LineBrush::c_pDirections[BRUSHDIR] = 2;

	// make one of the brushes current
	m_pCurrentBrush	= ImpBrush::c_pBrushes[0];
	m_pCurrentDirection = 0;


}


//---------------------------------------------------------
// Set the current UI 
//---------------------------------------------------------
void ImpressionistDoc::setUI(ImpressionistUI* ui) 
{
	m_pUI	= ui;
}

//---------------------------------------------------------
// Returns the active picture/painting name
//---------------------------------------------------------
char* ImpressionistDoc::getImageName() 
{
	return m_imageName;
}

//---------------------------------------------------------
// Called by the UI when the user changes the brush type.
// type: one of the defined brush types.
//---------------------------------------------------------
void ImpressionistDoc::setBrushType(int type)
{
	m_pCurrentBrush	= ImpBrush::c_pBrushes[type];
}

//---------------------------------------------------------
// Called by the UI when the user changes the Stroke Direction type.
// type: one of the defined stroke direction types.
//---------------------------------------------------------
void ImpressionistDoc::setStrokeDirectionType(int type)
{
	m_pCurrentDirection = LineBrush::c_pDirections[type];
}

//---------------------------------------------------------
// Returns the size of the brush.
//---------------------------------------------------------
int ImpressionistDoc::getSize()
{
	return m_pUI->getSize();
}

//---------------------------------------------------------
// Returns the line width of the brush.
//---------------------------------------------------------
int ImpressionistDoc::getWidth()
{
	return m_pUI->getWidth();
}

//---------------------------------------------------------
// Returns the size of the brush.
//---------------------------------------------------------

int ImpressionistDoc::getAngle()
{
	return m_pUI->getAngle();
}

int ImpressionistDoc::getLineAngle()
{
	return lineAngle;
}

void ImpressionistDoc::setLineAngle(int angle)
{
	lineAngle = angle;
}
//---------------------------------------------------------
// Returns the alpha value.
//---------------------------------------------------------
float ImpressionistDoc::getAlpha()
{
	return m_pUI->getAlpha();
}


bool ImpressionistDoc::getSizeBool()
{
	return m_pUI->getSizeBool();
}

bool ImpressionistDoc::getCustomEdgeClip()
{
	return m_pUI->getCustomEdgeClip();
}

bool ImpressionistDoc::getEdgeClip()
{
	return m_pUI->getEdgeClip();
}

int ImpressionistDoc::getSpace()
{
	return m_pUI->getSpace();
}

int ImpressionistDoc::getThreshold()
{
	return m_pUI->getThreshold();
}

float ImpressionistDoc::getRedScalar()
{
	return m_pUI->getRedScalar();
}

float ImpressionistDoc::getGreenScalar()
{
	return m_pUI->getGreenScalar();
}

float ImpressionistDoc::getBlueScalar()
{
	return m_pUI->getBlueScalar();
}


//---------------------------------------------------------
// Load the specified image
// This is called by the UI when the load image button is 
// pressed.
//---------------------------------------------------------
int ImpressionistDoc::loadImage(char *iname) 
{
	// try to open the image to read
	unsigned char*	data;
	int				width, 
					height;

	if ( (data=readBMP(iname, width, height))==NULL ) 
	{
		fl_alert("Can't load bitmap file");
		return 0;
	}

	// reflect the fact of loading the new image
	m_nWidth		= width;
	m_nPaintWidth	= width;
	m_nHeight		= height;
	m_nPaintHeight	= height;

	// release old storage
	if ( m_ucBitmap ) delete [] m_ucBitmap;
	if ( m_ucPainting ) delete [] m_ucPainting;

	m_ucBitmap		= data;

	// allocate space for draw view
	m_ucPainting = new unsigned char[width * height * 3];
	memset(m_ucPainting, 0, width * height * 3);
	m_ucPaintingUndo = new unsigned char[width * height * 3];
	memset(m_ucPaintingUndo, 0, width * height * 3);
	m_pUI->m_mainWindow->resize(m_pUI->m_mainWindow->x(),
		m_pUI->m_mainWindow->y(),
		width * 2,
		height + 25);

	// display it on origView
	m_pUI->m_origView->resizeWindow(width, height);
	m_pUI->m_origView->refresh();

	// refresh paint view as well
	m_pUI->m_paintView->resizeWindow(width, height);
	m_pUI->m_paintView->refresh();


	return 1;
}


//----------------------------------------------------------------
// Save the specified image
// This is called by the UI when the save image menu button is 
// pressed.
//----------------------------------------------------------------
int ImpressionistDoc::saveImage(char *iname) 
{

	writeBMP(iname, m_nPaintWidth, m_nPaintHeight, m_ucPainting);

	return 1;
}

//----------------------------------------------------------------
// Clear the drawing canvas
// This is called by the UI when the clear canvas menu item is 
// chosen
//-----------------------------------------------------------------
int ImpressionistDoc::clearCanvas() 
{

	// Release old storage
	if ( m_ucPainting ) 
	{
		delete [] m_ucPainting;

		// allocate space for draw view
		m_ucPainting	= new unsigned char [m_nPaintWidth*m_nPaintHeight*3];
		memset(m_ucPainting, 0, m_nPaintWidth*m_nPaintHeight*3);

		// refresh paint view as well	
		m_pUI->m_paintView->refresh();
	}
	
	return 0;
}

//----------------------------------------------------------------
// Auto Paint the drawing canvas
// This is called by the UI when the auto paint menu item is
// chosen
//-----------------------------------------------------------------
int ImpressionistDoc::autoPaint()
{

	m_pUI->m_paintView->autoPaint(getSpace());

	return 0;
}

//----------------------------------------------------------------
// Auto Paint edge on the drawing canvas
// This is called by the UI when the auto paint menu item is
// chosen
//-----------------------------------------------------------------
int ImpressionistDoc::autoPaintEdge()
{

	m_pUI->m_paintView->autoPaintEdge(getThreshold());

	return 0;
}



//------------------------------------------------------------------
// Get the color of the pixel in the original image at coord x and y
//------------------------------------------------------------------
GLubyte* ImpressionistDoc::GetOriginalPixel( int x, int y )
{
	if ( x < 0 ) 
		x = 0;
	else if ( x >= m_nWidth ) 
		x = m_nWidth-1;

	if ( y < 0 ) 
		y = 0;
	else if ( y >= m_nHeight ) 
		y = m_nHeight-1;

	return (GLubyte*)(m_ucBitmap + 3 * (y*m_nWidth + x));
}
GLubyte* ImpressionistDoc::GetGradientMapPixel(int x, int y)
{

	if (!m_usGradientMap) { fl_alert("Load a gradient map first!"); return (GLubyte*)(m_ucBitmap + 3 * (y * m_nWidth + x));
	}
	if (x < 0)
		x = 0;
	else if (x >= m_nWidth)
		x = m_nWidth - 1;

	if (y < 0)
		y = 0;
	else if (y >= m_nHeight)
		y = m_nHeight - 1;

	return (GLubyte*)(m_usGradientMap + 3 * (y * m_nWidth + x));
}

GLubyte* ImpressionistDoc::GetEdgeMapPixel(int x, int y)
{

	if (!m_usEdgeMap) {
		fl_alert("Load a edge map first!"); return (GLubyte*)(m_ucBitmap + 3 * (y * m_nWidth + x));
	}
	if (x < 0)
		x = 0;
	else if (x >= m_nWidth)
		x = m_nWidth - 1;

	if (y < 0)
		y = 0;
	else if (y >= m_nHeight)
		y = m_nHeight - 1;

	return (GLubyte*)(m_usEdgeMap + 3 * (y * m_nWidth + x));
}
//----------------------------------------------------------------
// Get the color of the pixel in the original image at point p
//----------------------------------------------------------------
GLubyte* ImpressionistDoc::GetOriginalPixel( const Point p )
{
	return GetOriginalPixel( p.x, p.y );
}
GLubyte* ImpressionistDoc::GetGradientMapPixel(const Point p)
{
	return GetGradientMapPixel(p.x, p.y);
}

GLubyte* ImpressionistDoc::GetEdgeMapPixel(const Point p)
{
	return GetEdgeMapPixel(p.x, p.y);
}

int ImpressionistDoc::loadAlphaBrush(char* iname)
{
	// try to open the image to read
	unsigned char* data;
	int				width,
		height;

	if ((data = readBMP(iname, width, height)) == NULL)
	{
		fl_alert("Can't load bitmap file");
		return 0;
	}

	m_nAlphaBrushWidth = width;
	m_nAlphaBrushHeight = height;
	/*unsigned char* alphaMap = new unsigned char[width * height];
	for (int i = 0; i < width * height; i++) {
		alphaMap[i] = (data[3 * i] + data[3 * i + 1] + data[3 * i + 2]) / 3;
	}*/
	//load the bit map
	if (m_ucAlphaBrush) delete[] m_ucAlphaBrush;
	m_ucAlphaBrush = data;
	//m_ucAlphaBrush = alphaMap;
	//delete[] data;
	return 1;
}