#include <iostream>
#include <cmath>
#include <set>

using namespace std;


#define noise -1
#define unclassified 0

/*
const int noise = -1;
const int unclassified = 0;
*/

class Point
{
	double x, y;
	int label; // what cluster in belongs to

public:

	bool operator < (Point& A) 
	{
		if (x < A.x) return true;
		if (x > A.x) return false;
		return y < A.y;
	}

	double distance(Point& point)
	{
		double xDistance = this->x - point.x;
		double yDistance = this->y - point.y;
		return sqrt(xDistance * xDistance + yDistance * yDistance);
	}

	// constructors
	Point(double x, double y)
	{
		this->x = x;
		this->y = y;
		label = unclassified;
	}

	// destructors
	~Point() {}

	// getters and setters
	int getLabel() { return label; }
	void setLabel(int label) { this->label = label; }
};

class DBSCAN
{
	set<Point> setOfPoints;
	double epsilon;
	int minimumAmountOfPoints;
	int amountOfClusters;

public:
	// constructors
	DBSCAN(set<Point> setOfPoints, double epsilon, int minimumAmountOfPoints)
	{
		this->setOfPoints = setOfPoints;
		this->epsilon = epsilon;
		this->minimumAmountOfPoints = minimumAmountOfPoints;
		amountOfClusters = 0;
	}

	// destructors
	~DBSCAN()
	{

	}

	void clusterAlgorithm()
	{
		for (Point point : setOfPoints)
		{
			if (point.getLabel() != unclassified)
				continue;
			set<Point> neighbors = rangeQuery(point);
			if (neighbors.size() < minimumAmountOfPoints)
			{
				point.setLabel(noise);
				continue;
			}
			amountOfClusters++;
			point.setLabel(amountOfClusters);
			set<Point> seed = neighbors;
			seed.erase(point);
			for (Point point : seed)
			{
				if (point.getLabel() == noise)
					point.setLabel(amountOfClusters);
				if (point.getLabel() != unclassified)
					continue;
				point.setLabel(amountOfClusters);
				neighbors = rangeQuery(point);
				if (neighbors.size() >= minimumAmountOfPoints)
					for (Point point : neighbors)
						seed.insert(point);
			}
		}
	}

	set<Point> rangeQuery(Point& currentPoint)
	{
		set<Point> neighbors;
		for (Point point : setOfPoints)
			if (currentPoint.distance(point) <= epsilon)
				neighbors.insert(point);
		return neighbors;
	}
};

void main()
{
	set<Point> dataBase;
	dataBase.insert(Point(0.0, 0.0));
	dataBase.insert(Point(1.0, 1.0));
	dataBase.insert(Point(1.0, 2.0));
	dataBase.insert(Point(1.0, 3.0));
	dataBase.insert(Point(1.0, 6.0));
	dataBase.insert(Point(2.0, 6.0));
	dataBase.insert(Point(2.0, 2.0));
	dataBase.insert(Point(5.0, 1.0));
	dataBase.insert(Point(5.0, 2.0));
	dataBase.insert(Point(6.0, 1.0));
	dataBase.insert(Point(6.0, 2.0));

	DBSCAN cluster(dataBase, 2.0, 4);

	cluster.clusterAlgorithm();

	for (Point point : dataBase)
		cout << point.getLabel() << " ";
}