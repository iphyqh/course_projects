//
// ImpBrush.cpp
//
// The implementation of virtual brush. All the other brushes inherit from it.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "ImpBrush.h"

// Static class member initializations
int			ImpBrush::c_nBrushCount	= 0;
ImpBrush**	ImpBrush::c_pBrushes	= NULL;
int			ImpBrush::autoSize = 0;

ImpBrush::ImpBrush(ImpressionistDoc*	pDoc, 
				   char*				name) :
					m_pDoc(pDoc), 
					m_pBrushName(name)
{
}

//---------------------------------------------------
// Return m_pDoc, which connects the UI and brushes
//---------------------------------------------------
ImpressionistDoc* ImpBrush::GetDocument(void)
{
	return m_pDoc;
}

//---------------------------------------------------
// Return the name of the current brush
//---------------------------------------------------
char* ImpBrush::BrushName(void)
{
	return m_pBrushName;
}

//----------------------------------------------------
// Set the color to paint with to the color at source,
// which is the coord at the original window to sample 
// the color from
//----------------------------------------------------
void ImpBrush::SetColor (const Point source)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;


	float alpha = pDoc->getAlpha();
	GLubyte alphaValue = (GLubyte) (alpha * 255);

	GLubyte color[4] = { 0, 0, 0, alphaValue };

	memcpy ( color, pDoc->GetOriginalPixel( source ), 3 );

	//float RedScalar = pDoc->getRedScalar();
	//float GreenScalar = pDoc->getGreenScalar();
	//float BlueScalar = pDoc->getBlueScalar();
	//
	//color[0] = RedScalar * color[0];
	//color[1] = GreenScalar * color[1];
	//color[2] = BlueScalar * color[2];

	glColor4ubv( color );
	glFlush();
}
void ImpBrush::SetColor(const Point source, int alpha)
{
	ImpressionistDoc* pDoc = GetDocument();

	GLubyte color[4];

	memcpy(color, pDoc->GetOriginalPixel(source), 3);
	double chosen_R = pDoc->getRedScalar();
	double chosen_G = pDoc->getGreenScalar();
	double chosen_B = pDoc->getBlueScalar();

	// std::cout << "a" << (int)color[0] << " " << (int)color[1] << " " << (int)color[2] << std::endl;
	// std::cout << "RGB: " << pDoc->getColorR() << " " << G << " " << B << std::endl;
	color[0] = (int)(color[0] * 1);
	color[1] = (int)(color[1] * 1);
	color[2] = (int)(color[2] * 1);
	// std::cout << (int)color[0] << " " << (int)color[1] << " " << (int)color[2] << std::endl;
	color[3] = alpha;
	glColor4ubv(color);
}