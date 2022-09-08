//
// PointBrush.h
//
// The header file for Point Brush. 
//

#ifndef MAGICSTARBRUSH_H
#define MAGICSTARBRUSH_H

#include "ImpBrush.h"

class MagicStarBrush : public ImpBrush
{
public:
	MagicStarBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);

	void BrushBegin(const Point source, const Point target);
	void BrushMove(const Point source, const Point target);
	void BrushEnd(const Point source, const Point target);
	char* BrushName(void);
};

#endif#pragma once
