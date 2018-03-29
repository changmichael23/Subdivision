#pragma once

#include "PatchCoons.hpp"
#include "CVecteur.h"

//profondeur z
//haut y

PatchCoons* makeRacordement(PatchCoons patch1, int side1, PatchCoons patch2, int side2,int complexity, bool& succeed)
{
	PatchCoons *p = new PatchCoons();
	if ((side1 >= 0 && side1 < 4 && side2 >= 0 && side2 < 4) || patch1.curves[side1].currPoints.size() != patch2.curves[side2].currPoints.size())
	{
		succeed = false;
		return p;
	}
	Curve c1 = patch1.curves[side1], c2 = patch2.curves[side2];
	Point p1 = c1.currPoints[0], p2 = c1.currPoints[c1.currPoints.size() - 1], p3 = c2.currPoints[0], p4 = c2.currPoints[c2.currPoints.size() - 1];
	CVecteur v1(p2.x-p1.x, p2.y - p1.y, p2.z - p1.z), v2(p4.x - p3.x, p4.y - p3.y, p4.z - p3.z);

	float angle = acos(CVecteur::DotProduct(&v1,&v2) / (v1.getNorme()*v2.getNorme()));
	
	//a verifier
	patch2.Rotate(-angle, 0);

	Point center1((p1.x+p2.x)/2, (p1.y + p2.y) / 2, (p1.z + p2.z) / 2), center2((p3.x + p4.x) / 2, (p3.y + p4.y) / 2, (p3.z + p4.z) / 2);

	//a verifier
	patch2.Translate(Point(-(center2.x - center1.x), -(center2.y - center1.y), -(center2.z - center1.z)));


	std::vector<Point> selectedPoints;
	switch (side1)
	{
	case 0:
		for (int i = 0; i < patch1.curves[0].n * complexity; i++)
		{
			selectedPoints.push_back(patch1.points[i]);
		}
		break;
	case 1:
		for (int i = 0; i < patch1.points.size(); i += patch1.curves[0].n)
		{
			for (int j = 0; j < complexity; j++)
			{
				selectedPoints.push_back(patch1.points[i + j]);
			}

		}
		break;
	case 2:
		for (int i = patch1.curves[0].n-complexity; i < patch1.points.size(); i += patch1.curves[0].n)
		{
			for (int j = 0; j < complexity; j++)
			{
				selectedPoints.push_back(patch1.points[i + j]);
			}
		}
		break;
	case 3:
		for (int i = patch1.curves[0].n * (patch1.curves[1].n- complexity); i < patch1.points.size(); i++)
		{
			selectedPoints.push_back(patch1.points[i]);
		}
		break;
	}

	switch (side2)
	{
	case 0:
		for (int i = 0; i < patch2.curves[0].n * complexity; i++)
		{
			selectedPoints.push_back(patch2.points[i]);
		}
		break;
	case 1:
		for (int i = 0; i < patch2.points.size(); i += patch2.curves[0].n)
		{
			for (int j = 0; j < complexity; j++)
			{
				selectedPoints.push_back(patch2.points[i + j]);
			}

		}
		break;
	case 2:
		for (int i = patch2.curves[0].n - complexity; i < patch2.points.size(); i += patch2.curves[0].n)
		{
			for (int j = 0; j < complexity; j++)
			{
				selectedPoints.push_back(patch2.points[i + j]);
			}
		}
		break;
	case 3:
		for (int i = patch2.curves[0].n * (patch2.curves[1].n - complexity); i < patch2.points.size(); i++)
		{
			selectedPoints.push_back(patch2.points[i]);
		}
		break;
	}





	succeed = true;
	return p;
}
