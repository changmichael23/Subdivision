#pragma once
#include <vector>
#include "Point.hpp"

#ifndef CURVE_HPP
#define CURVE_HPP

struct Curve
{
	float u = .25, v = 0.25;
	int n;
	bool closed;
	std::vector<Point> initPoints, currPoints, newPoints;

	Curve(std::vector<Point> p, bool c)
	{
		n = p.size();
		initPoints = p;
		currPoints = p;
		closed = c;
	}

	void SubdiviseCurve()
	{
		Point p1, p2, np1, np2;

		for (int i = 0; i < n - 1; ++i)
		{
			if (i == 0)
			{
				p1 = currPoints.at(i);
				p2 = currPoints.at(i + 1);

				if (closed)
				{
					np1 = Point(((1 - u) * p1.x + u * p2.x) / 2, ((1 - u) * p1.y + u * p2.y) / 2, ((1 - u) * p1.z + u * p2.z) / 2);
					newPoints.push_back(np1);
				}
				np2 = Point((v * p1.x + (1 - v) * p2.x) / 2, (v * p1.y + (1 - v) * p2.y) / 2, (v * p1.z + (1 - v) * p2.z) / 2);
				newPoints.push_back(np2);
			}
			else if (i == n - 2)
			{
				p1 = currPoints.at(i);
				p2 = currPoints.at(i + 1);

				np1 = Point(((1 - u) * p1.x + u * p2.x) / 2, ((1 - u) * p1.y + u * p2.y) / 2, ((1 - u) * p1.z + u * p2.z) / 2);
				newPoints.push_back(np1);

				if (closed)
				{
					np2 = Point((v * p1.x + (1 - v) * p2.x) / 2, (v * p1.y + (1 - v) * p2.y) / 2, (v * p1.z + (1 - v) * p2.z) / 2);
					newPoints.push_back(np2);
				}
			}

			if (closed)
			{
				p1 = currPoints.at(n - 1);
				p2 = currPoints.at(0);

				np1 = Point(((1 - u) * p1.x + u * p2.x) / 2, ((1 - u) * p1.y + u * p2.y) / 2, ((1 - u) * p1.z + u * p2.z) / 2);
				newPoints.push_back(np1);

				np2 = Point((v * p1.x + (1 - v) * p2.x) / 2, (v * p1.y + (1 - v) * p2.y) / 2, (v * p1.z + (1 - v) * p2.z) / 2);
				newPoints.push_back(np2);
			}
		}

		currPoints = newPoints;
		n = newPoints.size();
	}

	void ReinitializeCurve()
	{
		currPoints = initPoints;
		n = initPoints.size();
	}
};

#endif // !CURVE_HPP
