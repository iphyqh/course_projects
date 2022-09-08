#include "BezierCurveEvaluator.h"
#include <assert.h>
#include "vec.h"
#include "mat.h"
#define SEGMENT 30

void BezierCurveEvaluator::evaluateCurve(const std::vector<Point>& ptvCtrlPts,
	std::vector<Point>& ptvEvaluatedCurvePts,
	const float& fAniLength,
	const bool& bWrap) const
{
	ptvEvaluatedCurvePts.clear();
	float x_start = 0.0;
	float y_start = ptvCtrlPts[0].y;
	int iCtrlPtCount = ptvCtrlPts.size();
	int segNum = 30;

	vector<Point> controlPointsCopy(ptvCtrlPts);
	if (bWrap)
	{
		controlPointsCopy.push_back(Point(ptvCtrlPts.front().x + fAniLength, ptvCtrlPts.front().y));

	}
	const  Mat4d m(
		-1, 3, -3, 1,
		3, -6, 3, 0,
		-3, 3, 0, 0,
		1, 0, 0, 0);

	bool is_wrapped = false;
	int i = 0;
	for (; i + 3 < iCtrlPtCount; i += 3) {
		ptvEvaluatedCurvePts.push_back(controlPointsCopy[i]);
		ptvEvaluatedCurvePts.push_back(controlPointsCopy[i + 3]);
		const Vec4d px(controlPointsCopy[i].x, controlPointsCopy[i + 1].x,
			controlPointsCopy[i + 2].x, controlPointsCopy[i + 3].x);
		const Vec4d py(controlPointsCopy[i].y, controlPointsCopy[i + 1].y,
			controlPointsCopy[i + 2].y, controlPointsCopy[i + 3].y);
		for (int j = 0; j < segNum; j++) {
			double t = j / (double)segNum;
			const 	Vec4d param_time(t * t * t, t * t, t, 1);

			Point eval_point(param_time * (m * px), param_time * (m * py));
			ptvEvaluatedCurvePts.push_back(eval_point);
		}
	}



	if (!bWrap) {
		for (; i < ptvCtrlPts.size(); i++)
		{
			ptvEvaluatedCurvePts.push_back(ptvCtrlPts[i]);
		}
		ptvEvaluatedCurvePts.push_back(Point(0, ptvCtrlPts.front().y));
		ptvEvaluatedCurvePts.push_back(Point(fAniLength, ptvCtrlPts.back().y));
	}
	else if (ptvCtrlPts.size() - i < 3) {
		for (; i < ptvCtrlPts.size(); i++)
		{
			ptvEvaluatedCurvePts.push_back(ptvCtrlPts[i]);
		}
		const float interval_length = ptvCtrlPts.front().x + fAniLength - ptvCtrlPts.back().x;
		const float percent = 1.0f - ptvCtrlPts.front().x / interval_length;
		const float y = ptvCtrlPts.back().y + (ptvCtrlPts.front().y - ptvCtrlPts.back().y) * percent;
		ptvEvaluatedCurvePts.push_back(Point(fAniLength, y));
		ptvEvaluatedCurvePts.push_back(Point(0.0f, y));
	}
	else {
		const Vec4d px(controlPointsCopy[i].x, controlPointsCopy[i + 1].x,
			controlPointsCopy[i + 2].x, controlPointsCopy[i + 3].x);
		const Vec4d py(controlPointsCopy[i].y, controlPointsCopy[i + 1].y,
			controlPointsCopy[i + 2].y, controlPointsCopy[i + 3].y);
		for (int j = 0; j < SEGMENT; j++) {
			const double t = j / (double)SEGMENT;
			const Vec4d param_time(t * t * t, t * t, t, 1);

			Point eval_point(param_time * (m * px), param_time * (m * py));

			if (eval_point.x > fAniLength)
			{
				const float x_mod = fmod(eval_point.x, fAniLength);
				/*if (!is_wrapped)
				{
					const Point prev_point(ptvEvaluatedCurvePts.back());
					const float x_neg1 = prev_point.x - fAniLength;
					ptvEvaluatedCurvePts.push_back(Point(x_neg1, prev_point.y));
					ptvEvaluatedCurvePts.push_back(eval_point);
					is_wrapped = true;
				}*/
				eval_point.x = x_mod;
			}
			ptvEvaluatedCurvePts.push_back(eval_point);

		}
	}
}