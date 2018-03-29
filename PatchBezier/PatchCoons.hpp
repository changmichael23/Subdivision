#pragma once

#include "Curve.hpp"
#include <vector>
#include "Point.hpp"

struct PatchCoons
{
	Curve* a;
	Curve* b;
	Curve* c;
	Curve* d;

	std::vector<Point*> points;



};