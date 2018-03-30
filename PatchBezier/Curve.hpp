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

	Curve()
	{

	}

	Curve(std::vector<Point> p, bool c)
	{
		n = p.size();
		initPoints = p;
		currPoints = p;
		closed = c;
	}

	Curve* invCurve()
	{
		Curve *c = new Curve();
		c->u = u;
		c->v = v;
		c->n = n;
		for (int i = initPoints.size() - 1; i >= 0; i--)
		{
			c->initPoints.push_back(initPoints[i]);
		}
		for (int i = currPoints.size() - 1; i >= 0; i--)
		{
			c->currPoints.push_back(currPoints[i]);
		}
		for (int i = newPoints.size() - 1; i >= 0; i--)
		{
			c->newPoints.push_back(newPoints[i]);
		}
		return c;
	}

	Curve* copy()
	{
		Curve *c = new Curve();
		c->u = u;
		c->v = v;
		c->n = n;
		for (int i = 0; i<initPoints.size(); i++)
		{
			c->initPoints.push_back(initPoints[i]);
		}
		for (int i = 0; i<currPoints.size(); i++)
		{
			c->currPoints.push_back(currPoints[i]);
		}
		for (int i = 0; i<newPoints.size(); i++)
		{
			c->newPoints.push_back(newPoints[i]);
		}
		return c;
	}

	Curve* addConnectedCurve(Curve *c2)
	{
		Curve *newCourbe = new Curve();
		newCourbe->u = u;
		newCourbe->v = v;
		newCourbe->n = n+ c2->n -1;
		for (int i = 0; i<initPoints.size()-1; i++)
		{
			newCourbe->initPoints.push_back(initPoints[i]);
		}
		for (int i = 0; i<c2->initPoints.size(); i++)
		{
			newCourbe->initPoints.push_back(c2->initPoints[i]);
		}

		for (int i = 0; i<currPoints.size() - 1; i++)
		{
			newCourbe->currPoints.push_back(currPoints[i]);
		}
		for (int i = 0; i<c2->currPoints.size(); i++)
		{
			newCourbe->currPoints.push_back(c2->currPoints[i]);
		}

		for (int i = 0; i<newPoints.size() - 1; i++)
		{
			newCourbe->newPoints.push_back(newPoints[i]);
		}
		for (int i = 0; i<c2->newPoints.size(); i++)
		{
			newCourbe->newPoints.push_back(c2->newPoints[i]);
		}
		return newCourbe;
	}

	void SubdiviseCurve()
	{
		Point p1, p2, np1, np2;

		for (int i = 0; i < n - 1 ; ++i)
		{
			if (i == 0)
			{
				p1 = currPoints.at(i);
				p2 = currPoints.at(i + 1);

				if (closed)
				{
					np1 = Point(((1 - u) * p1.x + u * p2.x)  , ((1 - u) * p1.y + u * p2.y)  , ((1 - u) * p1.z + u * p2.z)  );
					newPoints.push_back(np1);
				}
				else
				{
					newPoints.push_back(currPoints[0]);
				}

				np2 = Point((v * p1.x + (1 - v) * p2.x)  , (v * p1.y + (1 - v) * p2.y)  , (v * p1.z + (1 - v) * p2.z)  );
				newPoints.push_back(np2);
			}
			else if (i == n - 2)
			{
				p1 = currPoints.at(i);
				p2 = currPoints.at(i + 1);

				np1 = Point(((1 - u) * p1.x + u * p2.x)  , ((1 - u) * p1.y + u * p2.y)  , ((1 - u) * p1.z + u * p2.z)  );
				newPoints.push_back(np1);

				if (closed)
				{
					np2 = Point((v * p1.x + (1 - v) * p2.x)  , (v * p1.y + (1 - v) * p2.y)  , (v * p1.z + (1 - v) * p2.z)  );
					newPoints.push_back(np2);
				}
				else
				{
					newPoints.push_back(currPoints.back());
				}
			}
			else
			{
				p1 = currPoints.at(i);
				p2 = currPoints.at(i + 1);

				np1 = Point(((1 - u) * p1.x + u * p2.x)  , ((1 - u) * p1.y + u * p2.y)  , ((1 - u) * p1.z + u * p2.z)  );
				newPoints.push_back(np1);
				np2 = Point((v * p1.x + (1 - v) * p2.x)  , (v * p1.y + (1 - v) * p2.y)  , (v * p1.z + (1 - v) * p2.z)  );
				newPoints.push_back(np2);
			}
			
		}

		if (closed)
		{
			p1 = currPoints.at(n - 1);
			p2 = currPoints.at(0);

			np1 = Point(((1 - u) * p1.x + u * p2.x)  , ((1 - u) * p1.y + u * p2.y)  , ((1 - u) * p1.z + u * p2.z)  );
			newPoints.push_back(np1);

			np2 = Point((v * p1.x + (1 - v) * p2.x)  , (v * p1.y + (1 - v) * p2.y)  , (v * p1.z + (1 - v) * p2.z)  );
			newPoints.push_back(np2);
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
