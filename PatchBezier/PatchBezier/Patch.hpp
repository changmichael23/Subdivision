#pragma once
#include <vector>
#include "Point.hpp"

#ifndef PATCH_HPP
#define PATCH_HPP

extern int precision;
extern RotateMode rm;

struct Patch
{
	int n, m;
	std::vector<Point> controlPoints;
	std::vector<Point> gridPoints;

	Patch()
	{
		n = 0;
		m = 0;
	}

	Patch(int _n, int _m)
	{
		n = _n;
		m = _m;

		for (int i = 0; i <= n; ++i)
		{
			for (int j = 0; j <= m; ++j)
			{
				Point p = Point(i, .0f, j);
				controlPoints.push_back(p);
			}
		}
	}

	void MovePoint(int i, float step)
	{
		controlPoints.at(i).y += step;
	}

	void Rotate(int i, float step)
	{
		if (rm == quaternion)
		{

		}
		else if (rm == matrix)
		{
			
		}
	}

	void Translate(int i, float step)
	{
		if (i == 0)
		{
			for (Point p : controlPoints)
			{
				p.x += step;
			}
		}
		else if (i == 1)
		{
			for (Point p : controlPoints)
			{
				p.y += step;
			}
		}
		else if (i == 2)
		{
			for (Point p : controlPoints)
			{
				p.z += step;
			}
		}
	}

	void Resize(float step)
	{
		for (Point p : controlPoints)
		{
			p.x *= step;
			p.y *= step;
			p.z *= step;
		}
	}

	void generateCurve()
	{
		gridPoints.reserve(pow(precision + 1, 2));

		for (int j = 0; j <= precision; ++j)
		{
			for (int i = 0; i <= precision; ++i)
			{
				gridPoints.push_back(bezierPatch(i / (float)precision, j / (float)precision));
				std::cout << gridPoints.back().x << " - " << gridPoints.back().y << " - " << gridPoints.back().z << std::endl;
			}
		}
	}

	Point bezierPatch(float u, float v)
	{
		Point Suv = Point(), Si = Point();

		for (int i = 0; i <= n; ++i)
		{
			Si = .0f;
			for (int j = 0; j <= m; ++j)
			{
				Si +=  controlPoints[i * (m + 1) + j] * BernsteinPoly(j, v, m);
			}
			Suv += Si * BernsteinPoly(i, u, n);
		}

		return Suv;
	}

	float BernsteinPoly(int i, float t, int dim)
	{
		return Binomial(i, dim) * pow(t, i) * pow(1 - t, dim - i);
	}

	int Binomial(int x, int n)
	{
		return Factorial(n) / (Factorial(x) * Factorial(n - x));
	}

	int Factorial(int x)
	{
		return (x == 1 || x == 0) ? 1 : x * Factorial(x - 1);
	}
};

#endif