#include "BSpline.h"
#include "Beziercurveevaluator.h"
#include "vec.h"
#include "mat.h"
void BSpline::evaluateCurve(
	const std::vector<Point>& controlPoints,
	std::vector<Point>& evaluatedPoints,
	const float& animationLength,
	const bool& beWrap) const
{
	evaluatedPoints.clear();

	if (!beWrap)
	{
		evaluatedPoints.push_back(Point(0, controlPoints.front().y));
		evaluatedPoints.push_back(Point(animationLength, controlPoints.back().y));
	}

	// a hack to make the endpoints controllable
	vector <Point> controlPointsCopy;
	if (beWrap)
	{
		Point start_p1 = Point((controlPoints.end() - 2)->x - animationLength,
			(controlPoints.end() - 2)->y);
		Point start_p2 = Point((controlPoints.end() - 1)->x - animationLength,
			(controlPoints.end() - 1)->y);
		Point end_p1 = Point((controlPoints.begin())->x + animationLength,
			(controlPoints.begin())->y);
		Point end_p2 = Point((controlPoints.begin() + 1)->x + animationLength,
			(controlPoints.begin() + 1)->y);
		controlPointsCopy.push_back(start_p1);
		controlPointsCopy.push_back(start_p2);
		controlPointsCopy.insert(controlPointsCopy.end(), controlPoints.begin(), controlPoints.end());
		controlPointsCopy.push_back(end_p1);
		controlPointsCopy.push_back(end_p2);
	}
	else
	{
		controlPointsCopy.push_back(controlPoints.front());
		controlPointsCopy.push_back(controlPoints.front());
		controlPointsCopy.insert(controlPointsCopy.end(), controlPoints.begin(), controlPoints.end());
		controlPointsCopy.push_back(controlPoints.back());
		controlPointsCopy.push_back(controlPoints.back());
	}
	const Mat4d basis = Mat4d(
		1, 4, 1, 0,
		0, 4, 2, 0,
		0, 2, 4, 0,
		0, 1, 4, 1) / 6.0;

	BezierCurveEvaluator bezierCurveEvaluator;
	for (size_t cnt = 0; cnt + 3 < controlPointsCopy.size(); ++cnt)
	{
		Vec4d param_x(controlPointsCopy[cnt].x, controlPointsCopy[cnt + 1].x,
			controlPointsCopy[cnt + 2].x, controlPointsCopy[cnt + 3].x);
		Vec4d param_y(controlPointsCopy[cnt].y, controlPointsCopy[cnt + 1].y,
			controlPointsCopy[cnt + 2].y, controlPointsCopy[cnt + 3].y);
		param_x = basis * param_x;
		param_y = basis * param_y;
		vector<Point> param_control;
		for (int i = 0; i < 4; ++i)
		{
			param_control.push_back(Point(param_x[i], param_y[i]));
		}
		vector<Point> param_evaluated;
		bezierCurveEvaluator.evaluateCurve(param_control, param_evaluated, animationLength, false);
		evaluatedPoints.insert(evaluatedPoints.end(), param_evaluated.begin(), param_evaluated.end() - 2);
	}

}