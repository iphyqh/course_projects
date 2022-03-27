//
// CircleBrush.h
//
// The header file for Circle Brush. 
// The method of drawing a circle comes from the answers of this webpage: https://stackoverflow.com/questions/22444450/drawing-circle-with-opengl
//

#ifndef CIRCLEBRUSH_H
#define CIRCLEBRUSH_H

#include "ImpBrush.h"

class CircleBrush : public ImpBrush
{
public:
	CircleBrush( ImpressionistDoc* pDoc = NULL, char* name = NULL );

	void BrushBegin( const Point source, const Point target );
	void BrushMove( const Point source, const Point target );
	void BrushEnd( const Point source, const Point target );
	char* BrushName( void );
};

#endif