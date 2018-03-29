#pragma once
#include <vector>
#include "Point.hpp"
#include "CQuaternion.h"

#ifndef PATCH_HPP
#define PATCH_HPP

extern int precision;
extern RotateMode rm;

float COS(float angle)
{
	return cos(angle * 2 * 0.00872664625);
}
float SIN(float angle)
{
	return sin(angle * 2 * 0.00872664625);
}

struct Patch
{
	int n, m;
	bool subdivised;
	std::vector<Point> controlPoints;
	std::vector<Point> gridPoints;
	//std::vector<Point> subdivisionPoints;
	float rotationAngle;
	SquareMatrice *rotationMatrix;
	CQuaternion *quatRotation;

	Patch()
	{
		n = 0;
		m = 0;
		subdivised = false;
		this->rotationAngle = 0.f;

		gridPoints.reserve(pow(precision + 1, 2));
		quatRotation = new CQuaternion(rotationAngle, new CVecteur(1, 0, 0));
		rotationMatrix = quatRotation->QuaternionToMatrice();
	}

	Patch(int _n, int _m)
	{
		n = _n;
		m = _m;
		subdivised = false;

		for (int i = 0; i <= n; ++i)
		{
			for (int j = 0; j <= m; ++j)
			{
				Point p = Point(i, .0f, j);
				controlPoints.push_back(p);
			}
		}
		this->rotationAngle = 0.f;
		quatRotation = new CQuaternion(rotationAngle, new CVecteur(1, 0, 0));
		rotationMatrix = quatRotation->QuaternionToMatrice();
		gridPoints.reserve(pow(precision + 1, 2));
	}

	void ChangeColor(float col1,float col2,float col3)
	{
		for (Point &p : gridPoints)
		{
			p.c1 = col1;
			p.c2 = col2;
			p.c3 = col3;
		}
	}
	void MovePoint(int i, float step)
	{
		controlPoints.at(i).y += step;
	}

	CVecteur* rotate_vector_by_quaternion(CVecteur* v, CQuaternion* q)
	{
		CVecteur u(q->GetY(), q->GetZ(), q->GetW());

		float s = q->GetX();

		return CVecteur::AdditionVecteur(CVecteur::AdditionVecteur(CVecteur::Scalar(&u, 2.0f * CVecteur::DotProduct(&u, v))
			, CVecteur::Scalar(v, (s*s - CVecteur::DotProduct(&u, &u)))),
			CVecteur::Scalar(CVecteur::CrossProduct(&u, v), 2.0f * s));
	}

	void Rotate(int a, float step)
	{
		float x = 0.f, y = 0.f, z = 0.f;

		if (rm == quaternion)
		{
			if (a == 0)
			{
				if(step < 0)
					rotationAngle -= 0.25;
				else
					rotationAngle += 0.25;

				if (rotationAngle == 360)
					rotationAngle = 0;

				this->quatRotation->reinit(this->rotationAngle, new CVecteur(1, 0, 0, true));

				for (Point &p : gridPoints)
				{
					CVecteur* v = new CVecteur(p.x, p.y, p.z);
					CVecteur* res = rotate_vector_by_quaternion(v, this->quatRotation);

					p.x = res->getX();
					p.y = res->getY();
					p.z = res->getZ();

					delete v;
					delete res;
				}
			}
			else if (a == 1)
			{
				rotationAngle += 0.25;
				if (rotationAngle == 360)
					rotationAngle = 0;

				this->quatRotation->reinit(this->rotationAngle, new CVecteur(0, 1, 0, true));

				for (Point &p : gridPoints)
				{
					CVecteur* v = new CVecteur(p.x, p.y, p.z);
					CVecteur* res = rotate_vector_by_quaternion(v, this->quatRotation);

					p.x = res->getX();
					p.y = res->getY();
					p.z = res->getZ();

					delete v;
					delete res;
				}
			}
			else if (a == 2)
			{
				rotationAngle += 0.25;
				if (rotationAngle == 360)
					rotationAngle = 0;

				this->quatRotation->reinit(this->rotationAngle, new CVecteur(0, 0, 1, true));

				for (Point &p : gridPoints)
				{
					CVecteur* v = new CVecteur(p.x, p.y, p.z);
					CVecteur* res = rotate_vector_by_quaternion(v, this->quatRotation);

					p.x = res->getX();
					p.y = res->getY();
					p.z = res->getZ();

					delete v;
					delete res;
				}
			}
		}
		else if (rm == matrix)
		{
			if (a == 0)
			{
				for (Point &p : gridPoints)
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
				for (Point &p : gridPoints)
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
				for (Point &p : gridPoints)
				{
					x = p.x;
					y = p.y;
					z = p.z;

					p.x = x * COS(step) - y * SIN(step);
					p.y = x * SIN(step) + y * COS(step);
				}
			}
		}
	}

	void Translate(int i, float step)
	{
		if (i == 0)
		{
			for (Point &p : gridPoints)
			{
				p.x += step;
			}
		}
		else if (i == 1)
		{
			for (Point &p : gridPoints)
			{
				p.y += step;
			}
		}
		else if (i == 2)
		{
			for (Point &p : gridPoints)
			{
				p.z += step;
			}
		}
	}

	void Resize(float step)
	{
		for (Point &p : gridPoints)
		{
			p.x *= step;
			p.y *= step;
			p.z *= step;
		}
	}

	void GenerateCurve()
	{
		gridPoints.clear();

		for (int j = 0; j <= precision; ++j)
		{
			for (int i = 0; i <= precision; ++i)
			{
				gridPoints.push_back(BezierPatch(i / (float)precision, j / (float)precision));
				CalculateNormale(i / (float)precision, j / (float)precision);
			}
		}
	}

	Point BezierPatch(float u, float v)
	{
		Point Suv = Point(), Si = Point();

		for (int i = 0; i <= n; ++i)
		{
			Si = .0f;
			for (int j = 0; j <= m; ++j)
			{
				Si += controlPoints[i * (m + 1) + j] * BernsteinPoly(j, v, m);
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

	void CalculateNormale(float u, float v)
	{
		Point Du = Point(), Dv = Point(), Si = Point();

		for (int i = 0; i <= n; ++i)
		{
			Si = .0f;
			for (int j = 0; j <= m; ++j)
			{
				Si += controlPoints[i * (m + 1) + j] * BernsteinPoly(j, v, m);
			}
			Du += Si * DerivateBernsteinPoly(i, u, n);
		}

		for (int i = 0; i <= n; ++i)
		{
			Si = .0f;
			for (int j = 0; j <= m; ++j)
			{
				Si += controlPoints[i * (m + 1) + j] * DerivateBernsteinPoly(j, v, m);
			}
			Dv += Si * BernsteinPoly(i, u, n);
		}

		Point cp = CrossProduct(Du, Dv);

		gridPoints.back().n1 = cp.x;
		gridPoints.back().n2 = cp.y;
		gridPoints.back().n3 = cp.z;
	}

	float DerivateBernsteinPoly(int i, float t, int dim)
	{
		if (i == 0)
		{
			return -dim * pow(1 - t, dim - 1);
		}
		else if (i == dim)
		{
			return dim * pow(t, dim - 1);
		}
		else
		{
			return i * Binomial(i, dim) * pow(t, i - 1) * pow(1 - t, dim - i)
				- (dim - i) * Binomial(i, dim) * pow(t, i) * pow(1 - t, dim - i - 1);
		}
	}

	Point CrossProduct(Point p1, Point p2)
	{
		Point cp = Point();

		cp.x = p1.y * p2.z - p1.z * p2.y;
		cp.y = p1.z * p2.x - p1.x * p2.z;
		cp.z = p1.x * p2.y - p1.y * p2.x;

		return cp;
	}

	void Subdivise()
	{
		int sm;

		std::vector<Point> centrePoints;
		std::vector<Point> uEdgePoints;
		std::vector<Point> vEdgePoints;
		std::vector<Point> vertexPoints;

		std::cout << "Saisir la \"smoothness\" de la subdivision (standard = 4) : ";
		std::cin >> sm;

		for (int i = 0; i < precision; ++i)
		{
			for (int j = 0; j < precision; ++j)
			{
				Point centre = Point();

				centre.x = (gridPoints.at(i * precision + j).x + gridPoints.at((i + 1) * precision + j).x + gridPoints.at(i * precision + j + 1).x + gridPoints.at((i + 1) * precision + j + 1).x) / 4;
				centre.y = (gridPoints.at(i * precision + j).y + gridPoints.at((i + 1) * precision + j).y + gridPoints.at(i * precision + j + 1).y + gridPoints.at((i + 1) * precision + j + 1).y) / 4;
				centre.z = (gridPoints.at(i * precision + j).z + gridPoints.at((i + 1) * precision + j).z + gridPoints.at(i * precision + j + 1).z + gridPoints.at((i + 1) * precision + j + 1).z) / 4;

				centrePoints.push_back(centre);
			}
		}

		for (int i = 0; i < precision; ++i)
		{
			for (int j = 0; j < precision; ++j)
			{
				Point edge = Point();

				if (j != precision - 1)
				{
					edge.x = (centrePoints.at(i * precision + j).x + centrePoints.at(i * precision + j + 1).x) / 2;
					edge.y = (centrePoints.at(i * precision + j).y + centrePoints.at(i * precision + j + 1).y) / 2;
					edge.z = (centrePoints.at(i * precision + j).z + centrePoints.at(i * precision + j + 1).z) / 2;

					vEdgePoints.push_back(edge);
	}

				if (i != precision - 1)
				{
					edge.x = (centrePoints.at(i * precision + j).x + centrePoints.at((i + 1) * precision + j).x) / 2;
					edge.y = (centrePoints.at(i * precision + j).y + centrePoints.at((i + 1) * precision + j).y) / 2;
					edge.z = (centrePoints.at(i * precision + j).z + centrePoints.at((i + 1) * precision + j).z) / 2;

					uEdgePoints.push_back(edge);
				}
			}
		}

		for (int i = 1; i < precision; ++i)
		{
			for (int j = 1; j < precision; ++j)
			{
				Point p = Point();

				p.x = (sm * gridPoints.at(i * (precision + 1) + j).x
					+ centrePoints.at((i - 1) * precision + (j - 1)).x
					+ centrePoints.at((i - 1) * precision + j).x
					+ centrePoints.at(i * precision + (j - 1)).x
					+ centrePoints.at(i * precision + j).x
					+ vEdgePoints.at((i - 1) * (precision - 1) + (j - 1)).x
					+ vEdgePoints.at(i * (precision - 1) + (j - 1)).x
					+ uEdgePoints.at((i - 1) * (precision - 1) + (j - 1)).x
					+ uEdgePoints.at((i - 1) * (precision - 1) + j).x)
					/ (8 + sm);
				p.y = (sm * gridPoints.at(i * (precision + 1) + j).y
					+ centrePoints.at((i - 1) * precision + (j - 1)).y
					+ centrePoints.at((i - 1) * precision + j).y
					+ centrePoints.at(i * precision + (j - 1)).y
					+ centrePoints.at(i * precision + j).y
					+ vEdgePoints.at((i - 1) * (precision - 1) + (j - 1)).y
					+ vEdgePoints.at(i * (precision - 1) + (j - 1)).y
					+ uEdgePoints.at((i - 1) * (precision - 1) + (j - 1)).y
					+ uEdgePoints.at((i - 1) * (precision - 1) + j).y)
					/ (8 + sm);
				p.z = (sm * gridPoints.at(i * (precision + 1) + j).z
					+ centrePoints.at((i - 1) * precision + (j - 1)).z
					+ centrePoints.at((i - 1) * precision + j).z
					+ centrePoints.at(i * precision + (j - 1)).z
					+ centrePoints.at(i * precision + j).z
					+ vEdgePoints.at((i - 1) * (precision - 1) + (j - 1)).z
					+ vEdgePoints.at(i * (precision - 1) + (j - 1)).z
					+ uEdgePoints.at((i - 1) * (precision - 1) + (j - 1)).z
					+ uEdgePoints.at((i - 1) * (precision - 1) + j).z)
					/ (8 + sm);

				vertexPoints.push_back(p);
			}
		}

		gridPoints.clear();
		gridPoints.reserve(4 * precision * precision);
		int cI = 0, uI = 0, vI = 0, vtI = 0;
		for (int i = 0; i < precision * 2 - 1; ++i)
		{
			for (int j = 0; j < precision * 2 - 1; ++j)
			{
				if (i % 2 == 0)
				{
					if (j % 2 == 0)
					{
						gridPoints.push_back(centrePoints.at(cI++));
					}
					else
					{
						gridPoints.push_back(vEdgePoints.at(vI++));
					}
				}
				else
				{
					if (j % 2 == 0)
					{
						gridPoints.push_back(uEdgePoints.at(uI++));
					}
					else
					{
						gridPoints.push_back(vertexPoints.at(vtI++));
					}
				}
			}
		}
	}
};

#endif