#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

#define noise -1;
#define unclassified 0;

class Point
{
	double x, y;

public:
	int clusterID;

	double distance(Point& point)
	{
		double xDistance = this->x - point.x;
		double yDistance = this->y - point.y;
		return sqrt(xDistance*xDistance + yDistance * yDistance);
	}

	// constructors
	Point(double x, double y)
	{
		this->x = x;
		this->y = y;
		this->clusterID = 0;
	}

	// destructors
	~Point(){}
};