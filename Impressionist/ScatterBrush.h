//
// PointBrush.h
//
// The header file for Point Brush. 
//

#ifndef SCATTERBRUSH_H
#define SCATTERBRUSH_H

#include "ScatterBrush.h"

class ScatterBrush : public ImpBrush
{
public:
	ScatterBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);

	void BrushBegin(const Point source, const Point target);
	void BrushMove(const Point source, const Point target);
	void BrushEnd(const Point source, const Point target);
	char* BrushName(void);
};

#endif#pragma once#pragma once
