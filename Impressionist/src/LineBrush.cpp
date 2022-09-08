//
// LineBrush.cpp
//
// The implementation of Line Brush.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "linebrush.h"
#include <math.h>

#define PI 3.14159265

int* LineBrush::c_pDirections = NULL;
int	LineBrush::c_nDirectionCount = 0;
extern float frand();

LineBrush::LineBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

void LineBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;


	BrushMove(source, target);
}

void LineBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("LineBrush::BrushMove  document is NULL\n");
		return;
	}

	int size = pDoc->getSize();
	if (dlg->getSizeBool() == TRUE) {
		size = pDoc->getSize() + rand() % 20 + 1;
	}

	int width = pDoc->getWidth();
	int angle = 0;
	
	if (pDoc->m_pCurrentDirection == 0) {
		angle = pDoc->getAngle();
	}

	if (pDoc->m_pCurrentDirection == 1) {
		if (getSobelX(source) == 0)
			angle = 90;
		else
			angle = (int) (atan((getSobelY(source) / getSobelX(source))) * 180 / PI);
	}

	if (pDoc->m_pCurrentDirection == 2) {
		angle = pDoc->getLineAngle();
	}

	
	double sinParameter = sin(angle * PI / 180);
	double cosParameter = cos(angle * PI / 180);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	if (size == 1) {
		glPointSize((float)size);
		glBegin(GL_POINTS);
		SetColor(source);

		glVertex2d(target.x, target.y);

		glEnd();
	}


	glLineWidth((float)width);

	if (pDoc->getCustomEdgeClip() == TRUE)
	{
		Point* points = new Point[size + 1];
	
		int peakPoint = 0;

		for (int i = 0; i <= size; i++) {
			points[i] = Point(target.x - (int)(i / 2 * cosParameter), target.y - (int)(i / 2 * sinParameter));
		}
	

		for (int i = 0; i <= size; i++)
		{
			if (getSobelMagnitude(points[i]) > getSobelMagnitude(points[peakPoint]))
			{
				peakPoint = i;
			}
		}

		if (getSobelMagnitude(points[peakPoint]) > pDoc->getThreshold())
			size = peakPoint;
	
		delete[] points;
	}

	if (pDoc->getEdgeClip() == TRUE)
	{
		Point* points = new Point[size + 1];

		int peakPoint = 0;

		for (int i = 0; i <= size; i++) {
			points[i] = Point(target.x - (int)(i / 2 * cosParameter), target.y - (int)(i / 2 * sinParameter));
		}


		for (int i = 0; i <= size; i++)
		{
			if (pDoc->GetEdgeMapPixel(points[i])[0] > 0)
			{
				peakPoint = i;
			}
		}

		size = peakPoint;

		delete[] points;
	}


	if (size > 1) {
		glBegin(GL_LINES);
		SetColor(source);

		glVertex2d(target.x - (int)(size / 2 * cosParameter), target.y - (int)(size / 2 * sinParameter));
		BrushEnd(source, target);
	}
}

void LineBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;
	int size = pDoc->getSize();
	if (dlg->getSizeBool() == TRUE) {
		size = pDoc->getSize() + rand() % 20 + 1;
	}
	int angle = 0;

	if (size > 1) {
		if (pDoc->m_pCurrentDirection == 0) {
			angle = pDoc->getAngle();
		}

		if (pDoc->m_pCurrentDirection == 1) {
			if (getSobelX(source) == 0)
				angle = 90;
			else
				angle = (int)(atan((getSobelY(source) / getSobelX(source))) * 180 / PI);
		}

		if (pDoc->m_pCurrentDirection == 2) {
			angle = pDoc->getLineAngle();
		}



		double sinParameter = sin(angle * PI / 180);
		double cosParameter = cos(angle * PI / 180);
		
		if (pDoc->getCustomEdgeClip() == TRUE)
		{
			int peakPoint = 0;

			Point* points = NULL;
			points = new Point[size];
			for (int i = 0; i < (int)(size);) {
				points[i] = Point(target.x + (int)(i / 2 * cosParameter), target.y + (int)(i / 2 * sinParameter));
				i += 1;
			}

			for (int i = 0; i <= size; i++)
			{
				if (getSobelMagnitude(points[i]) > getSobelMagnitude(points[peakPoint]))
				{
					peakPoint = i;
				}
			}

			if (getSobelMagnitude(points[peakPoint]) > pDoc->getThreshold())
				size = peakPoint;

			delete[] points;
		}

		if (pDoc->getEdgeClip() == TRUE)
		{
			Point* points = new Point[size + 1];

			int peakPoint = 0;

			for (int i = 0; i <= size; i++) {
				points[i] = Point(target.x + (int)(i / 2 * cosParameter), target.y + (int)(i / 2 * sinParameter));
			}


			for (int i = 0; i <= size; i++)
			{
				if (pDoc->GetEdgeMapPixel(points[i])[0] > 128)
				{
					peakPoint = i;
				}
			}

			size = peakPoint;

			delete[] points;
		}

		glVertex2d(target.x + (int)(size / 2 * cosParameter), target.y + (int)(size / 2 * sinParameter));

		glEnd();
	}
}

double LineBrush::BlurredPoint(const Point source)
{
	ImpressionistDoc* pDoc = GetDocument();

	Point* points = NULL;
	GLubyte** pixels = NULL;
	double* gs = NULL; // gs stands for grayscale

	points = new Point[9];
	pixels = new GLubyte * [9];
	gs = new double[9]();


	points[0] = Point(source.x - 1, source.y - 1);
	points[1] = Point(source.x, source.y - 1);
	points[2] = Point(source.x, source.y - 1);
	points[3] = Point(source.x - 1, source.y);
	points[4] = source;
	points[5] = Point(source.x + 1, source.y);
	points[6] = Point(source.x - 1, source.y + 1);
	points[7] = Point(source.x, source.y + 1);
	points[8] = Point(source.x + 1, source.y + 1);



	for (int i = 0; i <= 8; i++)
		pixels[i] = pDoc->GetOriginalPixel(points[i]);

	for (int i = 0; i <= 8; i++) {
		gs[i] = 0.299 * pixels[i][0];
	}

	for (int i = 0; i <= 8; i++) {
		for (int channel = 1; channel < 3; channel++) {
			if (channel == 1)
			{
				gs[i] += 0.587 * pixels[i][channel];
			}
			if (channel == 2)
			{
				gs[i] += 0.114 * pixels[i][channel];
			}
		}
	}

	double BlurredSource = 0;


	BlurredSource = 1 * gs[0] + 2 * gs[1] + 1 * gs[2] + 2 * gs[3] + 4 * gs[4] + 2 * gs[5] + 1 * gs[6] + 2 * gs[7] + 1 * gs[8];

	BlurredSource = BlurredSource / 16;

	delete[] points;
	delete[] pixels;
	delete[] gs;

	return BlurredSource;

}


double LineBrush::getSobelX(const Point source)
{
	ImpressionistDoc* pDoc = GetDocument();

	Point* points = NULL;
	GLubyte** pixels = NULL;
	double * gs = NULL; // gs stands for grayscale

	points = new Point[9];
	pixels = new GLubyte * [9];
	gs = new double[9]();

	points[0] = Point(source.x - 1, source.y - 1);
	points[1] = Point(source.x, source.y - 1);
	points[2] = Point(source.x, source.y - 1);
	points[3] = Point(source.x - 1, source.y);
	points[4] = source;
	points[5] = Point(source.x + 1, source.y);
	points[6] = Point(source.x - 1, source.y + 1);
	points[7] = Point(source.x, source.y + 1);
	points[8] = Point(source.x + 1, source.y + 1);



	for (int i = 0; i <= 8; i++)
		gs[i] = BlurredPoint(points[i]);


	double Gy = (gs[0] * (1) + gs[1] * 2 + gs[2] * 1 + gs[6] * (-1) + gs[7] * (-2) + gs[8] * (-1));

	delete[] points;
	delete[] pixels;
	delete[] gs;

	return Gy;


}

double LineBrush::getSobelY(const Point source)
{
	ImpressionistDoc* pDoc = GetDocument();

	Point* points = NULL;
	GLubyte** pixels = NULL;
	double* gs = NULL; // gs stands for grayscale

	points = new Point [9];
	pixels = new GLubyte * [9];
	gs = new double [9]();

	points[0] = Point(source.x - 1, source.y - 1);
	points[1] = Point(source.x, source.y - 1);
	points[2] = Point(source.x, source.y - 1);
	points[3] = Point(source.x - 1, source.y);
	points[4] = source;
	points[5] = Point(source.x + 1, source.y);
	points[6] = Point(source.x - 1, source.y + 1);
	points[7] = Point(source.x, source.y + 1);
	points[8] = Point(source.x + 1, source.y + 1);



	for (int i = 0; i <= 8; i++)
		gs[i] = BlurredPoint(points[i]);


	double Gx = (gs[0] * (-1) + gs[2] * 1 + gs[3] * (-2) + gs[5] * 2 + gs[6] * (-1) + gs[8] * 1);

	delete[] points;
	delete[] pixels;
	delete[] gs;

	return Gx;
}

double LineBrush::getSobelMagnitude(const Point source) {
	return sqrt(getSobelX(source) * getSobelX(source) + getSobelY(source) * getSobelY(source));
}

