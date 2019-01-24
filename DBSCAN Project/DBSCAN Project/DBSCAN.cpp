#include <iostream>
#include <cmath>
#include <unordered_set>
#include <fstream>

using namespace std;

#define noise -1
#define unclassified 0

class Point
{
	double x, y;
	int label; // what cluster it belongs to

public:
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

	// those getters are not necessary for the algorithm, they exist for the printings in the main 
	double getX() { return x; }
	double getY() { return y; }

	// operators
	friend bool operator == (const Point& left, const Point& right)
	{
		return left.x == right.x && left.y == right.y;
	}

	// methods
	double distance(Point& point)
	{
		double xDistance = x - point.x;
		double yDistance = y - point.y;
		return sqrt(xDistance * xDistance + yDistance * yDistance);
	}
};

class DBSCAN
{
	unordered_set<Point*> setOfPoints;
	double epsilon;
	unsigned int minimumAmountOfPoints;
	unsigned int amountOfClusters;

public:
	// constructors
	DBSCAN(unordered_set<Point*> setOfPoints, double epsilon, int minimumAmountOfPoints)
	{
		this->setOfPoints = setOfPoints;
		this->epsilon = epsilon;
		this->minimumAmountOfPoints = minimumAmountOfPoints;
		amountOfClusters = 0;
	}

	// destructors
	~DBSCAN() {}

	// methods
	void clusterAlgorithm()
	{
		for (Point* point : setOfPoints)
		{
			if (point->getLabel() != unclassified)
				continue;
			unordered_set<Point*> neighbors = rangeQuery(point);
			if (neighbors.size() < minimumAmountOfPoints)
			{
				point->setLabel(noise);
				continue;
			}
			amountOfClusters++;
			point->setLabel(amountOfClusters);
			unordered_set<Point*> seed = neighbors;
			seed.erase(point);
			for (Point* point : seed)
			{
				if (point->getLabel() == noise)
					point->setLabel(amountOfClusters);
				if (point->getLabel() != unclassified)
					continue;
				point->setLabel(amountOfClusters);
				neighbors = rangeQuery(point);
				if (neighbors.size() >= minimumAmountOfPoints)
					for (Point* point : neighbors)
						seed.insert(point);
			}
		}
	}

	unordered_set<Point*> rangeQuery(Point* currentPoint)
	{
		unordered_set<Point*> neighbors;
		for (Point* point : setOfPoints)
			if (currentPoint->distance(*point) <= epsilon)
				neighbors.insert(point);
		return neighbors;
	}
};

int main(void)
{
	unordered_set<Point*> dataBase;
	ifstream inFile;
	double x, y;

	// read the coordinateds from the file
	inFile.open("Set of Points.txt");
	while (inFile >> x >> y)
		dataBase.insert(new Point(x, y));
	inFile.close();

	// run the clustering 
	DBSCAN cluster(dataBase, 2.0, 4);
	cluster.clusterAlgorithm();

	// (x, y, cluster the point belongs)
	for (Point* point : dataBase)
	{
		cout << "(" << point->getX() << ", " << point->getY() << ", " << point->getLabel() << ") ";
		delete point;
		point = nullptr;
	}
	cout << endl;

	system("pause");
	return 0;
}