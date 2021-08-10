#include <iostream>
#include <vector>
#include <fstream>
#include <set>
#include <algorithm>
#include <queue>
#include <utility>
#include <ctime>
#include <stdlib.h>

using namespace std;

/* QUESTION 1 */

class Point {
public:
	int x, y;
	int dir;
};

class Line {
public:
	Point a, b;
};

class FindIntersect {
	Line curr;
public:
	FindIntersect(Line curr) : curr(curr) {}

	bool operator () (const Line& line2) {
		return curr.a.y <= line2.a.y && curr.b.y >= line2.a.y && curr.a.x >= line2.a.x && curr.a.x <= line2.b.x;
	}
};

class RangeSort {
public:
	bool operator () (const Line& line1, const Line& line2) const {
		return line1.a.y > line2.a.y;
	}
};

class Comparator {
public:
	bool operator () (pair<Point, Point>& p1, pair<Point, Point>& p2) {
		return p1.first.x > p2.first.x;
	}
};

void generateRandLines(priority_queue<pair<Point, Point>, vector<pair<Point, Point>>, Comparator>& pq) {
	int n = 10, upper = 10, lower = 0;
	int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
	int hv;
	Line line;
	srand((unsigned)time(0));

	// Creates n random lines with x/y values between lower and upper.
	for (int i = 0; i < n; i++) {
		x1 = (rand() % (upper - lower + 1)) + lower;
		y1 = (rand() % (upper - lower + 1)) + lower;
		while (y1 == x1) {
			y1 = (rand() % (upper - lower + 1)) + lower;
		}

		hv = (rand() % (10 - 1 + 1)) + 1;
		if (hv < 5) {
			x2 = x1;
			y2 = y1 = (rand() % (upper - lower + 1)) + lower;
			while (y2 == x2 || y2 == y1) {
				y2 = (rand() % (upper - lower + 1)) + lower;
			}
		}

		if (hv >= 5) {
			y2 = y1;
			x2 = x2 = (rand() % (upper - lower + 1)) + lower;
			while (x2 == y2 || x2 == x1) {
				x2 = (rand() % (upper - lower + 1)) + lower;
			}
		}

		cout << "{" << x1 << ", " << y1 << "}, {" << x2 << ", " << y2 << "}" << endl;

		line.a = { x1, y1 };
		line.b = { x2, y2 };
		if (x1 < x2) {
			line.a.dir = 0;
			line.b.dir = 1;
		}
		else {
			line.a.dir = 1;
			line.b.dir = 0;
		}

		pq.push(make_pair(line.a, line.b));
		pq.push(make_pair(line.b, line.a));
	}
}

void createMinHeap(priority_queue<pair<Point, Point>, vector<pair<Point, Point>>, Comparator>& pq) {
	// Generate random lines (doesn't always generate intersections).
	generateRandLines(pq);

}

bool isVertical(Line line) {
	return line.a.x == line.b.x;
}

void sweepLine(priority_queue<pair<Point, Point>, vector<pair<Point, Point>>, Comparator>& pq) {
	int n = pq.size();
	int intersections = 0;
	cout << n << endl;

	// Create a set of active lines, or lines that the sweep line has reached so far.
	set<Line, RangeSort> lines;

	for (int i = 0; i < n; i++) {
		// Grab the left-most line (lowest x-coordinate)
		Line curr = { pq.top().first, pq.top().second };


		// Check for its direction.
		// If it is vertical check for intersecting lines already in the set.
		// Then pop it from the queue.
		if (isVertical(curr)) {
			set<Line>::iterator it = find_if(lines.begin(), lines.end(), FindIntersect(curr));
			while (it != lines.end()) {
				it = find_if(next(it), lines.end(), FindIntersect(curr));
				//cout << "{" << curr.a.x << ", " << curr.a.y << "}" << endl;
				intersections++;
			}
			lines.insert(lines.begin(), curr);
			pq.pop();
		}
		// If it is horizontal and the leftmost point of the line, insert it into the active lines set.
		// Then pop it from the queue.
		else if (curr.a.dir == 0) {
			set<Line>::iterator it = find_if(lines.begin(), lines.end(), FindIntersect(curr));
			while (it != lines.end()) {
				it = find_if(next(it), lines.end(), FindIntersect(curr));
				//cout << "{" << curr.a.x << ", " << curr.a.y << "}" << endl;
				intersections++;
			}
			lines.insert(lines.begin(), curr);
			pq.pop();
		}
		else {
			pq.pop();
			lines.erase(curr);
		}
		// Else it is the rightmost point of a horizontal line and can be popped from the queue
	}

	cout << "Intersections: " << intersections << endl;
}

/* Driver code */
int main() {
	priority_queue<pair<Point, Point>, vector<pair<Point, Point>>, Comparator> pq;

	/* Creates random horizontal and vertical lines and
	   pushes them to the priority queue. */
	createMinHeap(pq);

	/* Counts number of intersections in lines. */
	sweepLine(pq);
}
