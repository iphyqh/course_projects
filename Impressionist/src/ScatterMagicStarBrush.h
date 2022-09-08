//
// PointBrush.h
//
// The header file for Point Brush. 
//

#ifndef SCATTERMAGICSTARBRUSH_H
#define SCATTERMAGICSTARBRUSH_H

#include "ImpBrush.h"

class ScatterMagicStarBrush : public ImpBrush
{
public:
	ScatterMagicStarBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);

	void BrushBegin(const Point source, const Point target);
	void BrushMove(const Point source, const Point target);
	void BrushEnd(const Point source, const Point target);
	char* BrushName(void);
};

#endif#pragma once
