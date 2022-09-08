#include "CatmullCurveEvaluator.h"
#include <assert.h>
#include "vec.h"
#include "mat.h"
#define SEGMENT 500

void CatmullCurveEvaluator::evaluateCurve(const std::vector<Point>& ptvCtrlPts,
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

	const  Mat4d m(
		-1, 3, -3, 1,
		2, -5, 4, -1,
		-1, 0, 1, 0,
		0, 2, 0, 0);

	int i = 0;

	// Special Case: Deals with wrapping when there is only two points
	if (iCtrlPtCount == 2 && bWrap) {
		
		const Vec4d px(controlPointsCopy[ptvCtrlPts.size() - 2].x - fAniLength, controlPointsCopy[ptvCtrlPts.size() - 1].x - fAniLength,
			controlPointsCopy[0].x, controlPointsCopy[1].x);
		const Vec4d py(controlPointsCopy[ptvCtrlPts.size() - 2].y, controlPointsCopy[ptvCtrlPts.size() - 1].y,
			controlPointsCopy[0].y, controlPointsCopy[1].y);
		for (int j = 0; j < segNum; j++) {
			double t = j / (double)segNum;
			const 	Vec4d param_time(t * t * t, t * t, t, 1);

			Point eval_point(double(param_time * (m * px) / 2), double(param_time * (m * py) / 2));

			if (eval_point.x < 0)
				continue;

			if (ptvEvaluatedCurvePts.size() == 0 && eval_point.x >= 0)
				ptvEvaluatedCurvePts.push_back(eval_point);

			else if (eval_point.x > ptvEvaluatedCurvePts.back().x && eval_point.x > 0 && eval_point.x < controlPointsCopy[0].x)
				ptvEvaluatedCurvePts.push_back(eval_point);

		}

		ptvEvaluatedCurvePts.push_back(controlPointsCopy[0]);
		const Vec4d px2(controlPointsCopy[ptvCtrlPts.size() - 1].x - fAniLength, controlPointsCopy[0].x,
			controlPointsCopy[1].x, controlPointsCopy[0].x + fAniLength);
		const Vec4d py2(controlPointsCopy[ptvCtrlPts.size() - 1].y, controlPointsCopy[0].y,
			controlPointsCopy[1].y, controlPointsCopy[0].y);
		for (int j = 0; j < segNum; j++) {
			double t = j / (double)segNum;
			const 	Vec4d param_time(t * t * t, t * t, t, 1);

			Point eval_point(double(param_time * (m * px2) / 2), double(param_time * (m * py2) / 2));

			if (eval_point.x > ptvEvaluatedCurvePts.back().x && eval_point.x > controlPointsCopy[0].x && eval_point.x < controlPointsCopy[1].x)
				ptvEvaluatedCurvePts.push_back(eval_point);

		}

		ptvEvaluatedCurvePts.push_back(controlPointsCopy[1]);
		const Vec4d px3(controlPointsCopy[0].x - fAniLength, controlPointsCopy[1].x,
			controlPointsCopy[0].x + fAniLength, controlPointsCopy[1].x + fAniLength);
		const Vec4d py3(controlPointsCopy[0].y, controlPointsCopy[1].y,
			controlPointsCopy[0].y, controlPointsCopy[1].y);
		for (int j = 0; j < segNum; j++) {
			double t = j / (double)segNum;
			const 	Vec4d param_time(t * t * t, t * t, t, 1);

			Point eval_point(double(param_time * (m * px3) / 2), double(param_time * (m * py3) / 2));

			if (eval_point.x > ptvEvaluatedCurvePts.back().x && eval_point.x > controlPointsCopy[0].x && eval_point.x < fAniLength)
				ptvEvaluatedCurvePts.push_back(eval_point);
		}
	}

	// General case
	else {
		for (; i + 2 < iCtrlPtCount; i += 1) {

			// The curve between first ctrl point and second ctrl point, repeat the first control point at x = 0 when wrap is not activated to 
			// behave like sample
			if (i == 0 && !bWrap) {
				ptvEvaluatedCurvePts.push_back(controlPointsCopy[0]);
				const Vec4d px(0, controlPointsCopy[0].x,
					controlPointsCopy[1].x, controlPointsCopy[2].x);
				const Vec4d py(controlPointsCopy[0].y, controlPointsCopy[0].y,
					controlPointsCopy[1].y, controlPointsCopy[2].y);
				for (int j = 0; j < segNum; j++) {
					double t = j / (double)segNum;
					const 	Vec4d param_time(t * t * t, t * t, t, 1);

					Point eval_point(double(param_time * (m * px) / 2), double(param_time * (m * py) / 2));

					if (eval_point.x > ptvEvaluatedCurvePts.back().x && eval_point.x > controlPointsCopy[0].x && eval_point.x < controlPointsCopy[1].x)
						ptvEvaluatedCurvePts.push_back(eval_point);

				}
			}

			// When wrap is activated, need to take care of two consecutive curves at the start, before and after i = 0 (first ctrl point)
			else if (i == 0 && bWrap) {
				const Vec4d pxW(controlPointsCopy[ptvCtrlPts.size() - 2].x - fAniLength, controlPointsCopy[ptvCtrlPts.size() - 1].x - fAniLength,
					controlPointsCopy[0].x, controlPointsCopy[1].x);
				const Vec4d pyW(controlPointsCopy[ptvCtrlPts.size() - 2].y, controlPointsCopy[ptvCtrlPts.size() - 1].y,
					controlPointsCopy[0].y, controlPointsCopy[1].y);
				for (int j = 0; j < segNum; j++) {
					double t = j / (double)segNum;
					const 	Vec4d param_time(t * t * t, t * t, t, 1);

					Point eval_point(double(param_time * (m * pxW) / 2), double(param_time * (m * pyW) / 2));

					if (eval_point.x < 0)
						continue;

					if (ptvEvaluatedCurvePts.size() == 0 && eval_point.x >= 0)
						ptvEvaluatedCurvePts.push_back(eval_point);

					else if (eval_point.x > ptvEvaluatedCurvePts.back().x && eval_point.x > 0 && eval_point.x < controlPointsCopy[0].x)
						ptvEvaluatedCurvePts.push_back(eval_point);

				}

				ptvEvaluatedCurvePts.push_back(controlPointsCopy[0]);
				const Vec4d px(controlPointsCopy[ptvCtrlPts.size() - 1].x - fAniLength, controlPointsCopy[0].x,
					controlPointsCopy[1].x, controlPointsCopy[2].x);
				const Vec4d py(controlPointsCopy[ptvCtrlPts.size() - 1].y, controlPointsCopy[0].y,
					controlPointsCopy[1].y, controlPointsCopy[2].y);
				for (int j = 0; j < segNum; j++) {
					double t = j / (double)segNum;
					const 	Vec4d param_time(t * t * t, t * t, t, 1);

					Point eval_point(double(param_time * (m * px) / 2), double(param_time * (m * py) / 2));

					if (eval_point.x > ptvEvaluatedCurvePts.back().x && eval_point.x > controlPointsCopy[0].x && eval_point.x < controlPointsCopy[1].x)
						ptvEvaluatedCurvePts.push_back(eval_point);

				}
			}

			// The curve between second last ctrl point and last ctrl point, repeat the last control point at x = fAniLength when wrap is not activated to 
			// behave like sample
			if (i == iCtrlPtCount - 3 && !bWrap) {
				ptvEvaluatedCurvePts.push_back(controlPointsCopy[i + 1]);
				const Vec4d px(controlPointsCopy[i].x, controlPointsCopy[i + 1].x,
					controlPointsCopy[i + 2].x, fAniLength);
				const Vec4d py(controlPointsCopy[i].y, controlPointsCopy[i + 1].y,
					controlPointsCopy[i + 2].y, controlPointsCopy[i + 2].y);
				for (int j = 0; j < segNum; j++) {
					double t = j / (double)segNum;
					const 	Vec4d param_time(t * t * t, t * t, t, 1);

					Point eval_point(double(param_time * (m * px) / 2), double(param_time * (m * py) / 2));

					if (eval_point.x > ptvEvaluatedCurvePts.back().x && eval_point.x > controlPointsCopy[i + 1].x && eval_point.x < controlPointsCopy[i + 2].x)
						ptvEvaluatedCurvePts.push_back(eval_point);

				}
				ptvEvaluatedCurvePts.push_back(controlPointsCopy[i + 2]);

				break;
			}

			// When wrap is activated, need to take care of two consecutive curves at the end, before and after i = iCtrlPtCount - 1 (last ctrl point)  
			else if (i == iCtrlPtCount - 3 && bWrap) {

				ptvEvaluatedCurvePts.push_back(controlPointsCopy[i + 1]);
				const Vec4d px(controlPointsCopy[i].x, controlPointsCopy[i + 1].x,
					controlPointsCopy[i + 2].x, fAniLength + controlPointsCopy[0].x);
				const Vec4d py(controlPointsCopy[i].y, controlPointsCopy[i + 1].y,
					controlPointsCopy[i + 2].y, controlPointsCopy[0].y);
				for (int j = 0; j < segNum; j++) {
					double t = j / (double)segNum;
					const 	Vec4d param_time(t * t * t, t * t, t, 1);

					Point eval_point(double(param_time * (m * px) / 2), double(param_time * (m * py) / 2));

					if (eval_point.x > ptvEvaluatedCurvePts.back().x && eval_point.x > controlPointsCopy[i + 1].x && eval_point.x < controlPointsCopy[i + 2].x)
						ptvEvaluatedCurvePts.push_back(eval_point);

				}
				ptvEvaluatedCurvePts.push_back(controlPointsCopy[i + 2]);


				const Vec4d pxW(controlPointsCopy[i + 1].x, controlPointsCopy[i + 2].x,
					fAniLength + controlPointsCopy[0].x, fAniLength + controlPointsCopy[1].x);
				const Vec4d pyW(controlPointsCopy[i + 1].y, controlPointsCopy[i + 2].y,
					controlPointsCopy[0].y, controlPointsCopy[1].y);
				for (int j = 0; j < segNum; j++) {
					double t = j / (double)segNum;
					const 	Vec4d param_time(t * t * t, t * t, t, 1);

					Point eval_point(double(param_time * (m * pxW) / 2), double(param_time * (m * pyW) / 2));

					if (eval_point.x > ptvEvaluatedCurvePts.back().x && eval_point.x > controlPointsCopy[i + 2].x && eval_point.x < fAniLength)
						ptvEvaluatedCurvePts.push_back(eval_point);

				}

				break;
			}

			// Ordinary in-between curves with i >= 0 && i <= iCtrlPtCount - 4
			ptvEvaluatedCurvePts.push_back(controlPointsCopy[i + 1]);
			const Vec4d px(controlPointsCopy[i].x, controlPointsCopy[i + 1].x,
				controlPointsCopy[i + 2].x, controlPointsCopy[i + 3].x);
			const Vec4d py(controlPointsCopy[i].y, controlPointsCopy[i + 1].y,
				controlPointsCopy[i + 2].y, controlPointsCopy[i + 3].y);
			for (int j = 0; j < segNum; j++) {
				double t = j / (double)segNum;
				const 	Vec4d param_time(t * t * t, t * t, t, 1);

				Point eval_point(double(param_time * (m * px) / 2), double(param_time * (m * py) / 2));

				if (eval_point.x > ptvEvaluatedCurvePts.back().x && eval_point.x > controlPointsCopy[i + 1].x && eval_point.x < controlPointsCopy[i + 2].x)
					ptvEvaluatedCurvePts.push_back(eval_point);

			}
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
}
