#include <iostream>
#include <cmath>
#include <set>

using namespace std;

#define noise -1
#define unclassified 0

class Point
{
	double x, y;
	int label; // what cluster it belongs to

public:

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

	// getters and seters
	int getLabel() { return label; } 
	void setLabel(int label) { this->label = label; }

	// operators
	friend bool operator == (const Point& left, const Point& right)
	{
		return left.x == right.x && left.y == right.y;
	}

	friend bool operator < (const Point& left, const Point& right)
	{
		if (left.x < right.x)
			return true;
		if (left.x > right.x)
			return false;
		return left.y < right.y;
	}
};

class DBSCAN
{
	set<Point> setOfPoints;
	double epsilon;
	unsigned int minimumAmountOfPoints;
	unsigned int amountOfClusters;

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
	~DBSCAN() {}

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

	set<Point> getSetOfPoints() { return this->setOfPoints; }
};

int main()
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

	for (Point point : cluster.getSetOfPoints())
		cout << point.getLabel() << " ";

	system("pause");

	return 0;
}