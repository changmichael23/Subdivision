#ifndef POINT_HPP
#define POINT_HPP

#include <string>
#include <vector>
extern float sizeCube;

struct Point {
	float x;
	float y;
	float z;
	float n1;
	float n2;
	float n3;
	float c1=0.0f;
	float c2 = 1.0f;
	float c3 = 0.0f;
	Point()
	{
		x = 0;
		y = 0;
		z = 0;
		n1 = 0;
		n2 = 0;
		n3 = 0;
	}

	Point(float a, float b, float c, float d = 0.0f, float e = 0.0f, float f = 0.0f,float col1=0.0f, float col2 = 1.0f, float col3 = 0.0f)
	{
		x = a;
		y = b;
		z = c;
		n1 = d;
		n2 = e;
		n3 = f;
		c1 = col1;
		c2 = col2;
		c3 = col3;

	}

	bool operator==(const Point &p)
	{
		if (x == p.x && y == p.y && z == p.z)
		{
			return true;
		}
		return false;
	}

	void operator+=(const Point &p)
	{
		x += p.x;
		y += p.y;
		z += p.z;
	}

	Point operator*(const float &f)
	{
		Point p = Point();
		p.x = x * f;
		p.y = y * f;
		p.z = z * f;

		return p;
	}

	void operator=(const float &f)
	{
		x = f;
		y = f;
		z = f;
	}

	std::string toString()
	{
		return "x = " + std::to_string(x) + ", y = " + std::to_string(y) + ", z =" + std::to_string(z);
	}
};

float * structToTab(std::vector<Point> newPoints)
{
	float* tabP = new float[newPoints.size() * 6];
	int j = 0;
	for (int i = 0; i < newPoints.size() * 6; i += 6)
	{
		tabP[i] = newPoints[j].x;
		tabP[i + 1] = newPoints[j].y;
		tabP[i + 2] = newPoints[j].z;

		tabP[i + 3] = newPoints[j].n1;
		tabP[i + 4] = newPoints[j].n2;
		tabP[i + 5] = newPoints[j].n3;
		j++;
	}

	return tabP;
}
float RandomFloat(float a, float b) {
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}
std::vector<Point> createRandomPoints(int n)
{
	std::vector<Point> tmp;

	for (int i = 0; i < 50; i++)
	{
		tmp.push_back(Point(RandomFloat(-500,500), RandomFloat(-500, 500), RandomFloat(-500, 500), 0, 0, 0));
		
	}
	return tmp;
}


std::vector<Point> transformPointsToCube(std::vector<Point> p)
{
	std::vector<Point> tmp;
	int j = 0;
	for (int i = 0; i < p.size(); i++)
	{
		tmp.push_back(Point(p[i].x + sizeCube, p[i].y + sizeCube, p[i].z + sizeCube, +1.0f, 0.0f, 0.0f,p[i].c1, p[i].c2, p[i].c3));
		tmp.push_back(Point(p[i].x + sizeCube, p[i].y + sizeCube, p[i].z - sizeCube, +1.0f, 0.0f, 0.0f, p[i].c1, p[i].c2, p[i].c3));
		tmp.push_back(Point(p[i].x + sizeCube, p[i].y - sizeCube, p[i].z - sizeCube, +1.0f, 0.0f, 0.0f, p[i].c1, p[i].c2, p[i].c3));
		tmp.push_back(Point(p[i].x + sizeCube, p[i].y - sizeCube, p[i].z + sizeCube, +1.0f, 0.0f, 0.0f, p[i].c1, p[i].c2, p[i].c3));
		tmp.push_back(Point(p[i].x - sizeCube, p[i].y + sizeCube, p[i].z + sizeCube, -1.0f, 0.0f, 0.0f, p[i].c1, p[i].c2, p[i].c3));
		tmp.push_back(Point(p[i].x - sizeCube, p[i].y + sizeCube, p[i].z - sizeCube, -1.0f, 0.0f, 0.0f, p[i].c1, p[i].c2, p[i].c3));
		tmp.push_back(Point(p[i].x - sizeCube, p[i].y - sizeCube, p[i].z - sizeCube, -1.0f, 0.0f, 0.0f, p[i].c1, p[i].c2, p[i].c3));
		tmp.push_back(Point(p[i].x - sizeCube, p[i].y - sizeCube, p[i].z + sizeCube, -1.0f, 0.0f, 0.0f, p[i].c1, p[i].c2, p[i].c3));

		tmp.push_back(Point(p[i].x + sizeCube, p[i].y + sizeCube, p[i].z + sizeCube, 0.0f, +1.0f, 0.0f, p[i].c1, p[i].c2, p[i].c3));
		tmp.push_back(Point(p[i].x + sizeCube, p[i].y + sizeCube, p[i].z - sizeCube, 0.0f, +1.0f, 0.0f, p[i].c1, p[i].c2, p[i].c3));
		tmp.push_back(Point(p[i].x - sizeCube, p[i].y + sizeCube, p[i].z - sizeCube, 0.0f, +1.0f, 0.0f, p[i].c1, p[i].c2, p[i].c3));
		tmp.push_back(Point(p[i].x - sizeCube, p[i].y + sizeCube, p[i].z + sizeCube, 0.0f, +1.0f, 0.0f, p[i].c1, p[i].c2, p[i].c3));
		tmp.push_back(Point(p[i].x + sizeCube, p[i].y - sizeCube, p[i].z + sizeCube, 0.0f, -1.0f, 0.0f, p[i].c1, p[i].c2, p[i].c3));
		tmp.push_back(Point(p[i].x + sizeCube, p[i].y - sizeCube, p[i].z - sizeCube, 0.0f, -1.0f, 0.0f, p[i].c1, p[i].c2, p[i].c3));
		tmp.push_back(Point(p[i].x - sizeCube, p[i].y - sizeCube, p[i].z - sizeCube, 0.0f, -1.0f, 0.0f, p[i].c1, p[i].c2, p[i].c3));
		tmp.push_back(Point(p[i].x - sizeCube, p[i].y - sizeCube, p[i].z + sizeCube, 0.0f, -1.0f, 0.0f, p[i].c1, p[i].c2, p[i].c3));

		tmp.push_back(Point(p[i].x + sizeCube, p[i].y + sizeCube, p[i].z - sizeCube, 0.0f, 0.0f, -1.0f, p[i].c1, p[i].c2, p[i].c3));
		tmp.push_back(Point(p[i].x + sizeCube, p[i].y - sizeCube, p[i].z - sizeCube, 0.0f, 0.0f, -1.0f, p[i].c1, p[i].c2, p[i].c3));
		tmp.push_back(Point(p[i].x - sizeCube, p[i].y - sizeCube, p[i].z - sizeCube, 0.0f, 0.0f, -1.0f, p[i].c1, p[i].c2, p[i].c3));
		tmp.push_back(Point(p[i].x - sizeCube, p[i].y + sizeCube, p[i].z - sizeCube, 0.0f, 0.0f, -1.0f, p[i].c1, p[i].c2, p[i].c3));
		tmp.push_back(Point(p[i].x + sizeCube, p[i].y + sizeCube, p[i].z + sizeCube, 0.0f, 0.0f, +1.0f, p[i].c1, p[i].c2, p[i].c3));
		tmp.push_back(Point(p[i].x + sizeCube, p[i].y - sizeCube, p[i].z + sizeCube, 0.0f, 0.0f, +1.0f, p[i].c1, p[i].c2, p[i].c3));
		tmp.push_back(Point(p[i].x - sizeCube, p[i].y - sizeCube, p[i].z + sizeCube, 0.0f, 0.0f, +1.0f, p[i].c1, p[i].c2, p[i].c3));
		tmp.push_back(Point(p[i].x - sizeCube, p[i].y + sizeCube, p[i].z + sizeCube, 0.0f, 0.0f, +1.0f, p[i].c1, p[i].c2, p[i].c3));
		
		j += 8;
	}

	return tmp;

}

#endif

