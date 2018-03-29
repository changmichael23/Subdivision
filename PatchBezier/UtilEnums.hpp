#pragma once

enum State
{
	first = 0,
	idle = 1,
	drawing = 2,
	editing = 3,
	deleting = 4,
	subdivising = 5
};
enum EditMode
{
	rotateX = 0,
	rotateY = 1,
	rotateZ = 2,
	translateX = 3,
	translateY = 4,
	translateZ = 5,
	resize = 6
};
enum RotateMode
{
	matrix = 0,
	quaternion = 1
};