//
// LineBrush.h
//
// The header file for Line Brush. 
//

#ifndef LINEBRUSH_H
#define LINEBRUSH_H

#include "ImpBrush.h"

enum
{
	RIGHT = 0,
	GRADIENT,
	BRUSHDIR,
	NUM_DIRECTION_TYPE // Make sure this stays at the end!
};

class LineBrush : public ImpBrush
{
public:
	LineBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);
	void BrushBegin(const Point source, const Point target);
	void BrushMove(const Point source, const Point target);
	void BrushEnd(const Point source, const Point target);
	char* BrushName(void);
	double getSobelX(const Point source);
	double getSobelY(const Point source);
	double getSobelMagnitude(const Point source);
	double BlurredPoint(const Point soucre);

	static int* c_pDirections;
	static int	c_nDirectionCount;
};

#endif#