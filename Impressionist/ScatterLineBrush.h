//
// ScatterLineBrush.h
//
// The header file for Scatter Line Brush. 
//

#ifndef SCATTELINERBRUSH_H
#define SCATTELINERBRUSH_H

#include "ImpBrush.h"

class ScatterLineBrush : public ImpBrush
{
public:
	ScatterLineBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);

	void BrushBegin(const Point source, const Point target);
	void BrushMove(const Point source, const Point target);
	void BrushEnd(const Point source, const Point target);
	char* BrushName(void);
};

#endif#pragma once#pragma once
