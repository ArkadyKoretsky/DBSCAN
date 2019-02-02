#include <iostream>
#include <cmath>
#include <unordered_set>
#include <fstream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <random>

using namespace std;

#define noise -1
#define unclassified 0
#define amountOfPoints 100
#define range 50

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

void createPointsFromFile(unordered_set<Point*>& dataBase)
{
	ifstream inFile;
	double x, y;
	inFile.open("Set of Points.txt");
	while (inFile >> x >> y)
		dataBase.insert(new Point(x, y));
	inFile.close();
}

void generateRandomPoints(unordered_set<Point*>& dataBase)
{
	double x, y;
	srand(time(NULL));
	for (int i = 0; i < amountOfPoints; i++)
	{
		x = rand() % range;
		y = rand() % range;
		dataBase.insert(new Point(x, y));
	}
}

void printElapsedTime(clock_t begin, clock_t end)
{
	double elapsedTime = double(end - begin) / CLOCKS_PER_SEC;
	cout << "Time in Seconds: " << elapsedTime << endl;
}

void printPointsAndClusters(unordered_set<Point*>& dataBase)
{
	// the sorting is just to show you the points in each cluster (in the printing)
	vector<Point*> sortedPointsByLabel(dataBase.begin(), dataBase.end());
	sort(sortedPointsByLabel.begin(), sortedPointsByLabel.end(), [](Point* A, Point* B) {return A->getLabel() < B->getLabel(); });

	// print the clusters
	int label = sortedPointsByLabel[0]->getLabel();
	cout << "Noise Points: ";
	for (Point* point : sortedPointsByLabel)
	{
		if (point->getLabel() != label)
		{
			cout << endl;
			label = point->getLabel();
			cout << "Cluster Number " << label << ": ";
		}
		cout << "(" << point->getX() << " , " << point->getY() << ") ";
		delete point;
		point = nullptr;
	}
	cout << endl;
}

int main(void)
{
	unordered_set<Point*> dataBase;
	double epsilon = 2;
	unsigned int minimumPoints = 4;

	createPointsFromFile(dataBase);

	//generateRandomPoints(dataBase);

	// run the clustering 
	DBSCAN dbscan(dataBase, epsilon, minimumPoints);
	//clock_t begin = clock();
	dbscan.clusterAlgorithm();
	//clock_t end = clock();
	//printElapsedTime(begin, end);

	printPointsAndClusters(dataBase);

	system("pause");
	return 0;
}