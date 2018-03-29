#pragma once
#include "Patch.hpp"

//extern Patch* patches[10];
extern int nbPatches;
extern std::vector<Patch> patches;
extern Patch tmpPatch;
extern int nbPoints;
extern int pointIdx, patchIdx;
extern State gs;
extern EditMode em;

void StartNewPatch()
{
	gs = drawing;
	int n, m;

	std::cout << "Saisir l'ordre n du patch : ";
	std::cin >> n;
	std::cout << "Saisir l'ordre m du patch : ";
	std::cin >> m;

	tmpPatch = Patch(n, m);
	nbPoints = (n + 1) * (m + 1);
	pointIdx = 0;
	
}

void CancelPatch()
{
	if (nbPatches == 0)
	{
		gs = first;
	}
	else
	{
		gs = idle;
	}
}

void ConfirmPatch()
{
	gs = idle;
	patches.push_back(tmpPatch);
	patches.back().GenerateCurve();
	++nbPatches;
}

void RotatePatch(int i)
{
	gs = editing;
	patchIdx = 0;

	if (i == 0)
	{
		em = rotateX;
	}
	else if (i == 1)
	{
		em = rotateY;
	}
	else if (i == 2)
	{
		em = rotateZ;
	}
}

void TranslatePatch(int i)
{
	gs = editing;
	patchIdx = 0;

	if (i == 0)
	{
		em = translateX;
	}
	else if (i == 1)
	{
		em = translateY;
	}
	else if (i == 2)
	{
		em = translateZ;
	}
}

void ResizePatch()
{
	gs = editing;
	patchIdx = 0;
	em = resize;
}

void CancelEdit()
{
	gs = idle;
}

void ChangeRotationMethod()
{
	if (rm == matrix)
	{
		rm = quaternion;
	}
	else if (rm == quaternion)
	{
		rm = matrix;
	}
}

void DeletePatch()
{
	gs = deleting;
	patchIdx = 0;
}

void QuitDelete()
{
	if (nbPatches == 0)
	{
		gs = first;
	}
	else
	{
		gs = idle;
	}
}

void ConfirmDelete()
{
	patches.erase(patches.begin() + patchIdx);
	--nbPatches;

	if (nbPatches == 0)
	{
		QuitDelete();
	}
}

void SubdivisePatch()
{
	gs = subdivising;
	patchIdx = 0;
}

void CancelSubdivise()
{
	gs = idle;
}

void ConfirmSubdivise()
{
	patches.at(patchIdx).Subdivise();
}