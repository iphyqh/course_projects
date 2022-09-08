#pragma once
#ifndef GRADIENTRUSH_H
#define GRADIENTRUSH_H

#include "ImpBrush.h"

class GradientBrush : public ImpBrush
{
public:
	GradientBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);

	void BrushBegin(const Point source, const Point target);
	void BrushMove(const Point source, const Point target);
	void BrushEnd(const Point source, const Point target);
	char* BrushName(void);
	double getSobelX(const Point source);
	double getSobelY(const Point source);
	double BlurredPoint(const Point soucre);
	static int* c_pDirections;
	static int	c_nDirectionCount;
};

#endif