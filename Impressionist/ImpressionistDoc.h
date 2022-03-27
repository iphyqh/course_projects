// 
// impressionistDoc.h
//
// header file for Doc 
//

#ifndef ImpressionistDoc_h
#define ImpressionistDoc_h

#include "impressionist.h"
#include "bitmap.h"

static int lineAngle;

class ImpressionistUI;

class ImpressionistDoc 
{
public:
	ImpressionistDoc();

	void	setUI(ImpressionistUI* ui);		// Assign the UI to use

	int		loadImage(char *iname);			// called by the UI to load image
	int		loadAlphaBrush(char* iname);
	int		saveImage(char *iname);			// called by the UI to save image


	int     clearCanvas();                  // called by the UI to clear the drawing canvas
	void	setBrushType(int type);			// called by the UI to set the brushType
	void	setStrokeDirectionType(int type); // called by the UI to set the DirectionType

	int		autoPaint();

	int		autoPaintEdge();

	int		getSize();						// get the UI size
	void	setSize(int size);				// set the UI size
	
	int		getWidth();						// get the UI line width
	void	setWidth(int width);			// set the UI line width

	int		getAngle();						// get the UI line angle
	void	setAngle(int angle);			// set the UI line angle

	int		getSpace();						// get the UI auto paint space
	int		getThreshold();					// get the UI auto paint edge threshold

	int		getLineAngle();					// get the line angel, not via UI
	void	setLineAngle(int angle);

	float	getAlpha();						// get the alpha value
	void	setAlpha(float alpha);			// set the alpha value

	bool	getSizeBool();

	bool	getCustomEdgeClip();

	bool	getEdgeClip();

	float ImpressionistDoc::getRedScalar();     // get the Red scalar value
	float ImpressionistDoc::getGreenScalar();	// get the Green scalar value
	float ImpressionistDoc::getBlueScalar();    // get the Blue scalar value

	char*	getImageName();					// get the current image name
	

// Attributes
public:
	// Dimensions of original window.
	int				m_nWidth, 
					m_nHeight;
	// Dimensions of the paint window.
	int				m_nPaintWidth, 
					m_nPaintHeight;	
	// Dimensions of the alpha paint window.
	int				m_nAlphaBrushWidth,
		m_nAlphaBrushHeight;
	// Bitmaps for original image and painting.
	unsigned char*	m_ucBitmap;
	unsigned char*	m_ucPainting;
	unsigned char* m_ucPaintingUndo;
	unsigned char* m_ucAlphaBrush;
	unsigned char* m_usGradientMap;
	unsigned char* m_usEdgeMap;
	// The current active brush.
	ImpBrush*			m_pCurrentBrush;	
	int m_pCurrentDirection;
	// Size of the brush.
	int m_nSize;
	// Size of the linebrush width
	int m_nLineWidth;



	ImpressionistUI*	m_pUI;

// Operations
public:
	// Get the color of the original picture at the specified coord
	GLubyte* GetOriginalPixel( int x, int y );   
	// Get the color of the original picture at the specified point	
	GLubyte* GetOriginalPixel( const Point p );  
	GLubyte* GetGradientMapPixel(int x, int y);
	GLubyte* GetGradientMapPixel(const Point p);
	GLubyte* GetEdgeMapPixel(int x, int y);
	GLubyte* GetEdgeMapPixel(const Point p);


private:
	char			m_imageName[256];

};

extern void MessageBox(char *message);

#endif
