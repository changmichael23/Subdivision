#pragma once

#include "PatchCoons.hpp"
#include "CVecteur.h"

//profondeur z
//haut y

PatchCoons* makeRacordement(PatchCoons patch1, int side1, PatchCoons patch2, int side2,int complexity, bool& succeed)
{
	PatchCoons *patchFinal = new PatchCoons();
	if ((side1 >= 0 && side1 < 4 && side2 >= 0 && side2 < 4) || patch1.curves[side1]->currPoints.size() != patch2.curves[side2]->currPoints.size() || complexity<0)
	{
		succeed = false;
		return patchFinal;
	}
	Curve *c1 = patch1.curves[side1], *c2 = patch2.curves[side2];
	Point p1 = c1->currPoints[0], p2 = c1->currPoints[c1->currPoints.size() - 1], p3 = c2->currPoints[0], p4 = c2->currPoints[c2->currPoints.size() - 1];
	CVecteur v1(p2.x-p1.x, p2.y - p1.y, p2.z - p1.z), v2(p4.x - p3.x, p4.y - p3.y, p4.z - p3.z);

	float angle = ACOS(CVecteur::DotProduct(&v1,&v2) / (v1.getNorme()*v2.getNorme()));
	
	
	//a verifier
	patch2.Rotate(1, -angle);

	Point center1((p1.x+p2.x)/2, (p1.y + p2.y) / 2, (p1.z + p2.z) / 2), center2((p3.x + p4.x) / 2, (p3.y + p4.y) / 2, (p3.z + p4.z) / 2);

	//a verifier
	patch2.Translate(Point(-(center2.x - center1.x), -(center2.y - center1.y), -(center2.z - center1.z)));

	
	PatchCoons patchTmp2;
	
	int width2 = patch2.curves[0]->n;
	int height2 = patch2.curves[1]->n;

	//faire tourner les points du patch2 pour avoir des lignes en face 
	int rotation = 0;
	if (side1 == 0)
	{
		if (side2 == 0)
		{
			patchTmp2.curves[0] = patch2.curves[2]->invCurve();
			patchTmp2.curves[1] = patch2.curves[3]->invCurve();
			patchTmp2.curves[2] = patch2.curves[0]->invCurve();
			patchTmp2.curves[3] = patch2.curves[1]->invCurve();

			rotation = 180;
		}
		else if (side2 == 1)
		{
			patchTmp2.curves[0] = patch2.curves[3]->copy();
			patchTmp2.curves[1] = patch2.curves[0]->invCurve();
			patchTmp2.curves[2] = patch2.curves[1]->copy();
			patchTmp2.curves[3] = patch2.curves[2]->invCurve();

			rotation = 90;
		}
		else if (side2 == 3)
		{
			patchTmp2.curves[0] = patch2.curves[1]->invCurve();
			patchTmp2.curves[1] = patch2.curves[2]->copy();
			patchTmp2.curves[2] = patch2.curves[3]->invCurve();
			patchTmp2.curves[3] = patch2.curves[0]->copy();

			rotation=-90;
		}
		else
		{
			patchTmp2.curves[0] = patch2.curves[0]->copy();
			patchTmp2.curves[1] = patch2.curves[1]->copy();
			patchTmp2.curves[2] = patch2.curves[2]->copy();
			patchTmp2.curves[3] = patch2.curves[3]->copy();
		}
	}
	else if (side1 == 1)
	{
		if (side2 == 0)
		{
			patchTmp2.curves[0] = patch2.curves[1]->invCurve();
			patchTmp2.curves[1] = patch2.curves[2]->copy();
			patchTmp2.curves[2] = patch2.curves[3]->invCurve();
			patchTmp2.curves[3] = patch2.curves[0]->copy();

			rotation = -90;
		}
		else if (side2 == 1)
		{
			patchTmp2.curves[0] = patch2.curves[2]->invCurve();
			patchTmp2.curves[1] = patch2.curves[3]->invCurve();
			patchTmp2.curves[2] = patch2.curves[0]->invCurve();
			patchTmp2.curves[3] = patch2.curves[1]->invCurve();

			rotation = 180;
		}
		else if (side2 == 2)
		{
			patchTmp2.curves[0] = patch2.curves[3]->copy();
			patchTmp2.curves[1] = patch2.curves[0]->invCurve();
			patchTmp2.curves[2] = patch2.curves[1]->copy();
			patchTmp2.curves[3] = patch2.curves[2]->invCurve();

			rotation = 90;
		}
		else
		{
			patchTmp2.curves[0] = patch2.curves[0]->copy();
			patchTmp2.curves[1] = patch2.curves[1]->copy();
			patchTmp2.curves[2] = patch2.curves[2]->copy();
			patchTmp2.curves[3] = patch2.curves[3]->copy();
		}
	}
	else if (side1 == 2)
	{
		if (side2 == 1)
		{
			patchTmp2.curves[0] = patch2.curves[1]->invCurve();
			patchTmp2.curves[1] = patch2.curves[2]->copy();
			patchTmp2.curves[2] = patch2.curves[3]->invCurve();
			patchTmp2.curves[3] = patch2.curves[0]->copy();

			rotation = -90;
		}
		else if (side2 == 2)
		{
			patchTmp2.curves[0] = patch2.curves[2]->invCurve();
			patchTmp2.curves[1] = patch2.curves[3]->invCurve();
			patchTmp2.curves[2] = patch2.curves[0]->invCurve();
			patchTmp2.curves[3] = patch2.curves[1]->invCurve();

			rotation = 180;
		}
		else if (side2 == 3)
		{
			patchTmp2.curves[0] = patch2.curves[3]->copy();
			patchTmp2.curves[1] = patch2.curves[0]->invCurve();
			patchTmp2.curves[2] = patch2.curves[1]->copy();
			patchTmp2.curves[3] = patch2.curves[2]->invCurve();

			rotation = 90;
		}
		else
		{
			patchTmp2.curves[0] = patch2.curves[0]->copy();
			patchTmp2.curves[1] = patch2.curves[1]->copy();
			patchTmp2.curves[2] = patch2.curves[2]->copy();
			patchTmp2.curves[3] = patch2.curves[3]->copy();
		}
	}
	else if (side1 == 3)
	{
		if (side2 == 0)
		{
			patchTmp2.curves[0] = patch2.curves[3]->copy();
			patchTmp2.curves[1] = patch2.curves[0]->invCurve();
			patchTmp2.curves[2] = patch2.curves[1]->copy();
			patchTmp2.curves[3] = patch2.curves[2]->invCurve();

			rotation = 90;
		}
		else if (side2 == 2)
		{
			patchTmp2.curves[0] = patch2.curves[1]->invCurve();
			patchTmp2.curves[1] = patch2.curves[2]->copy();
			patchTmp2.curves[2] = patch2.curves[3]->invCurve();
			patchTmp2.curves[3] = patch2.curves[0]->copy();

			rotation = -90;
		}
		else if (side2 == 3)
		{
			patchTmp2.curves[0] = patch2.curves[2]->invCurve();
			patchTmp2.curves[1] = patch2.curves[3]->invCurve();
			patchTmp2.curves[2] = patch2.curves[0]->invCurve();
			patchTmp2.curves[3] = patch2.curves[1]->invCurve();

			rotation = 180;
		}
		else
		{
			patchTmp2.curves[0] = patch2.curves[0]->copy();
			patchTmp2.curves[1] = patch2.curves[1]->copy();
			patchTmp2.curves[2] = patch2.curves[2]->copy();
			patchTmp2.curves[3] = patch2.curves[3]->copy();
		}
	}

	int width1 = patch1.curves[0]->n;
	int height1 = patch1.curves[1]->n;

	

	if (rotation == 180)
	{
		for (int i = patch2.points.size()-1; i>=0 ; i--)
		{
			patchTmp2.points.push_back(patch2.points[i]);
		}
	}
	else if (rotation == 90)
	{
		for (int j = width2 - 1; j >= 0; j--)
		{
			for (int i = 0; i <height2; i++)
			{
				patchTmp2.points.push_back(patch2.points[i*width2+j]);
			}
		}
	}
	else if (rotation == -90)
	{
		for (int j = 0; j < width2; j++)
		{
			for (int i = height2-1; i >=0; i--)
			{
				patchTmp2.points.push_back(patch2.points[i*width2 + j]);
			}
		}
	}

	width2 = patchTmp2.curves[0]->n;
	height2 = patchTmp2.curves[1]->n;
	

	std::vector<Point> selectedPoints;
	complexity += 1;

	if (side1 == 0)
	{
		int limit = patchTmp2.n-width2;
		for (int i = patchTmp2.n - (width2*complexity); i < limit; i++)
		{
			selectedPoints.push_back(patchTmp2.points[i]);
		}
		for (int i = 0; i < width1; i++)
		{
			selectedPoints.push_back(((patch1.curves[0]->currPoints[i])+(patchTmp2.curves[2]->currPoints[i]))/2);
		}
		for (int i = width1; i < width1*complexity; i++)
		{
			selectedPoints.push_back(patch1.points[i]);
		}
	}
	else if (side2 == 1)
	{
		for (int i = 0; i < height1; i++)
		{
			for (int j = width2 - complexity; j < width2 - 1; j++)
			{
				selectedPoints.push_back(patchTmp2.points[i*width2+j]);
			}
			selectedPoints.push_back((patchTmp2.points[i*width2+width2-1]+patch1.points[i*width1])/2);
			for (int j = 1; j < complexity; j++)
			{
				selectedPoints.push_back(patch1.points[i*width1 + j]);
			}
		}
	}
	if (side1 == 2)
	{
		int limit = patch1.n - width1;
		for (int i = patch1.n - (width1*complexity); i < limit; i++)
		{
			selectedPoints.push_back(patch1.points[i]);
		}
		for (int i = 0; i < width1; i++)
		{
			selectedPoints.push_back(((patchTmp2.curves[0]->currPoints[i]) + (patch1.curves[2]->currPoints[i])) / 2);
		}
		for (int i = width2; i < width2*complexity; i++)
		{
			selectedPoints.push_back(patchTmp2.points[i]);
		}
	}
	else if (side2 == 3)
	{
		for (int i = 0; i < height1; i++)
		{
			for (int j = width1 - complexity; j < width1 - 1; j++)
			{
				selectedPoints.push_back(patch1.points[i*width1 + j]);
			}
			selectedPoints.push_back((patch1.points[i*width1 + width1 - 1] + patchTmp2.points[i*width2]) / 2);
			for (int j = 1; j < complexity; j++)
			{
				selectedPoints.push_back(patchTmp2.points[i*width2 + j]);
			}
		}
	}


	PatchCoons patchIntermediaire;
	for (int i = 0; i < selectedPoints.size(); i++)
		patchIntermediaire.points.push_back(selectedPoints[i]);
	patchIntermediaire.n = selectedPoints.size();
	//mettre les point en triangles
	//faire la subdivision des selected points

	//reconstitution d'un patch entier
	
	if (side1 == 0)
	{
		for (int i = 0; i < patchTmp2.n - complexity*width2; i++)
		{
			patchFinal->points.push_back(patchTmp2.points[i]);
		}
		for (int i = 0; i < patchIntermediaire.points.size(); i++)
		{
			patchFinal->points.push_back(patchIntermediaire.points[i]);
		}
		for (int i = complexity*width1; i < patch1.n; i++)
		{
			patchFinal->points.push_back(patch1.points[i]);
		}

		//pour l'instant on remet les courbes à jour mais à changer après
		patchFinal->curves[0] = patchTmp2.curves[0]->copy();
		patchFinal->curves[1] = patchTmp2.curves[1]->addConnectedCurve(patch1.curves[1]);
		patchFinal->curves[2] = patch1.curves[2]->copy();
		patchFinal->curves[3] = patchTmp2.curves[3]->addConnectedCurve(patch1.curves[3]);
	}
	else if (side1 ==1)
	{
		int lineSize = 1 + (complexity - 1) * 2;
		for (int i = 0; i < height1; i++)
		{
			for (int j = 0; j < width2 - complexity; j++)
			{
				patchFinal->points.push_back(patchTmp2.points[width2*i+j]);
			}
			for (int j = 0; j < lineSize; j++) 
			{
				patchFinal->points.push_back(patchIntermediaire.points[lineSize*i + j]);
			}
			for (int j = complexity; j < width1; j++)
			{
				patchFinal->points.push_back(patch1.points[width1*i + j]);
			}
		}

		//pour l'instant on remet les courbes à jour mais à changer après
		patchFinal->curves[0] = patchTmp2.curves[0]->addConnectedCurve(patch1.curves[0]);
		patchFinal->curves[1] = patchTmp2.curves[1]->copy();
		patchFinal->curves[2] = patchTmp2.curves[2]->addConnectedCurve(patch1.curves[2]);
		patchFinal->curves[3] = patchTmp2.curves[3]->copy();
	}
	else if (side1 == 2)
	{
		for (int i = 0; i < patch1.n - complexity*width1; i++)
		{
			patchFinal->points.push_back(patch1.points[i]);
		}
		for (int i = 0; i < patchIntermediaire.points.size(); i++)
		{
			patchFinal->points.push_back(patchIntermediaire.points[i]);
		}
		for (int i = complexity*width2; i < patchTmp2.n; i++)
		{
			patchFinal->points.push_back(patchTmp2.points[i]);
		}

		//pour l'instant on remet les courbes à jour mais à changer après
		patchFinal->curves[0] = patch1.curves[0]->copy();
		patchFinal->curves[1] = patch1.curves[1]->addConnectedCurve(patchTmp2.curves[1]);
		patchFinal->curves[2] = patchTmp2.curves[2]->copy();
		patchFinal->curves[3] = patch1.curves[3]->addConnectedCurve(patchTmp2.curves[3]);
	}
	else if (side1 == 3)
	{
		int lineSize = 1 + (complexity - 1) * 2;
		for (int i = 0; i < height1; i++)
		{
			for (int j = 0; j < width1 - complexity; j++)
			{
				patchFinal->points.push_back(patch1.points[width1*i + j]);
			}
			for (int j = 0; j < lineSize; j++)
			{
				patchFinal->points.push_back(patchIntermediaire.points[lineSize*i + j]);
			}
			for (int j = complexity; j < width2; j++)
			{
				patchFinal->points.push_back(patchTmp2.points[width2*i + j]);
			}
		}

		//pour l'instant on remet les courbes à jour mais à changer après
		patchFinal->curves[0] = patch1.curves[0]->addConnectedCurve(patchTmp2.curves[0]);
		patchFinal->curves[1] = patch1.curves[1]->copy();
		patchFinal->curves[2] = patch1.curves[2]->addConnectedCurve(patchTmp2.curves[2]);
		patchFinal->curves[3] = patch1.curves[3]->copy();
	}


	succeed = true;
	return patchFinal;
}
