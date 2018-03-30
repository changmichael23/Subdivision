#pragma once

#include "Curve.hpp"
#include <vector>
#include "Point.hpp"

extern int precision;

float COS(float angle)
{
	return cos(angle * 2 * 0.00872664625);
}
float SIN(float angle)
{
	return sin(angle * 2 * 0.00872664625);
}

float ACOS(float x)
{
	return acos(x) * 2 * 0.00872664625;
}

struct PatchCoons
{
	int n, m;

	Curve* curves[4];
	std::vector<Point> points;

	std::vector<int> triangles;
	std::vector<Point> uPoints, vPoints, bPoints;
	
	PatchCoons()
	{
		n = 0;
		m = 0;

		points.reserve(pow(precision + 1, 2));
	}

	PatchCoons(int _n, int _m, Curve* _a, Curve* _b, Curve* _c, Curve* _d)
	{
		n = _n;
		m = _m;

		curves[0] = _a;
		curves[1] = _b;
		curves[2] = _c;
		curves[3] = _d;

		points.reserve(pow(precision + 1, 2));
	}

	std::vector<int> generateTriangles()
	{
		triangles.clear();
		int height = curves[1]->currPoints.size();
		int width = curves[0]->currPoints.size();
		for (int i = 0; i < width*(height - 1); i++)
		{
			triangles.push_back(i);
			if (i % 2 == 0)
			{
				triangles.push_back(i+width);
				triangles.push_back(i+1);
			}
			else
			{
				triangles.push_back(i + width-1);
				triangles.push_back(i + width);
			}
		}
		return triangles;
	}

	void ChangeColor(float col1, float col2, float col3)
	{
		for (Point &p : points)
		{
			p.c1 = col1;
			p.c2 = col2;
			p.c3 = col3;
		}
	}

	void Rotate(int a, float step)
	{
		float x = 0.f, y = 0.f, z = 0.f;
		if (a == 0)
		{
			for (Point &p : points)
			{
				x = p.x;
				y = p.y;
				z = p.z;

				p.y = y * COS(step) - z * SIN(step);
				p.z = y * SIN(step) + z * COS(step);
			}
		}
		else if (a == 1)
		{
			for (Point &p : points)
			{
				x = p.x;
				y = p.y;
				z = p.z;

				p.x = x * COS(step) + z * SIN(step);
				p.z = -x * SIN(step) + z * COS(step);
			}
		}
		else if (a == 2)
		{
			for (Point &p : points)
			{
				x = p.x;
				y = p.y;
				z = p.z;

				p.x = x * COS(step) - y * SIN(step);
				p.y = x * SIN(step) + y * COS(step);
			}
		}
	}

	void Translate(int i, float step)
	{
		if (i == 0)
		{
			for (Point &p : points)
			{
				p.x += step;
			}
		}
		else if (i == 1)
		{
			for (Point &p : points)
			{
				p.y += step;
			}
		}
		else if (i == 2)
		{
			for (Point &p : points)
			{
				p.z += step;
			}
		}
	}

	void Translate(Point step)
	{
		for (Point &p : points)
		{
			p.x += step.x;
			p.y += step.y;
			p.z += step.z;
		}
	}

	void GeneratePatch()
	{
		points.clear();

		uPoints = GenerateRuledSurface(curves[0], curves[2], m);
		vPoints = GenerateRuledSurface(curves[1], curves[3], n);

		bPoints = GenerateBilinearPatch();

		for (int j = 0; j < m; ++j)
		{
			for (int i = 0; i < n; ++i)
			{
				float x = uPoints.at(j * m + i).x + vPoints.at((i + 1) * n - (j + 1)).x - bPoints.at(i * n + j).x;
				float y = uPoints.at(j * m + i).y + vPoints.at((i + 1) * n - (j + 1)).y - bPoints.at(i * n + j).y;
				float z = uPoints.at(j * m + i).z + vPoints.at((i + 1) * n - (j + 1)).z - bPoints.at(i * n + j).z;
				//Point p = uPoints.at(j * m + i) + vPoints.at((j + 1) * n - (i + 1)) - bPoints.at(j * n + i);
				points.push_back(Point(x, y, z));
			}
		}
	}

	std::vector<Point> GenerateRuledSurface(Curve* c1, Curve* c2, int v)
	{
		std::vector<Point> surface;

		for (int i = 0; i < c1->n; ++i)
		{
			for (int j = 0; j < v; ++j)
			{
				Point p1 = c1->currPoints[i] * (1 - j / ((float)(v - 1)));
				Point p2 = c2->currPoints[c1->n - 1 - i] * j / ((float)(v - 1));
				surface.push_back(Point(p1.x + p2.x, p1.y + p2.y, p1.z + p2.z));
			}
		}

		return surface;
	}

	std::vector<Point> GenerateBilinearPatch()
	{
		std::vector<Point> patch;

		for (int j = 0; j < m; ++j)
		{
			for (int i = 0; i < n; ++i)
			{
				patch.push_back(BezierPatch(i / ((float) (n - 1)), j / ((float) (m - 1))));
			}
		}

		return patch;
	}

	Point BezierPatch(float u, float v)
	{
		Point Suv = Point(), Si = Point();
		
		Si = curves[0]->currPoints.at(0) * BernsteinPoly(0, v, 1);
		Si += curves[2]->currPoints.at(curves[2]->n - 1) * BernsteinPoly(1, v, 1);

		Suv += Si * BernsteinPoly(0, u, 1);

		Si = curves[0]->currPoints.at(curves[0]->n - 1) * BernsteinPoly(0, v, 1);
		Si += curves[2]->currPoints.at(0) * BernsteinPoly(1, v, 1);

		Suv += Si * BernsteinPoly(1, u, 1);

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