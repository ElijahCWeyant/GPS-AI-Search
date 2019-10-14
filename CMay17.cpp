#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <fstream>
#include <limits>
#include <queue>
#include <stack>

using namespace std;

struct point {
	double x;
	double y;
	bool friend operator == (point a, point b) {
		return (a.x == b.x && a.y == b.y);
	}
};

struct line {
	point begin;
	point end;
	double m;
	double b;
	string road_name;
};

void fixline(line &a) {
	if (a.end.x == a.begin.x) {
		a.m == numeric_limits<double>::max();
		a.b == numeric_limits<double>::max();
	}
	else {
		a.m = (a.end.y - a.begin.y) / (a.end.x - a.begin.x);
		a.b = a.begin.y - (a.m * a.begin.x);
	}
}


struct node {
	int a; //index of vector of roads for road that intersects this node
	int b;
	int me;
	point intersect;
};

struct djnode {
	int indexNode; //which node in inters
	int parent;
	int me; //which index
	double cost;
	friend bool operator > (djnode a, djnode b) {
		return a.cost < b.cost;
	}
};

vector<node> inters;
vector<line> roads;
vector<djnode> nodeList;

bool onLine(point x, line y) {
	bool on = false;
	double calc;
	if (y.m > 99999999) { //if the line is verticle
		if (x.x + 1 > y.begin.x && x.x - 1 < y.begin.x) { //and it could be on the line at place x with a little leaway
			double high = y.begin.y; //correct non-standard entry of csv (sorry)
			double low = y.end.y;
			if (y.begin.y < y.end.y) {
				low = y.begin.y;
				high = y.end.y;
			}
			if (x.y > low - 1 && x.y < high + 1) //if it is between the y values with a little leaway, it must be on the line, probably
				on = true;
		}
	}
	calc = (y.m * x.x) + y.b;
	if ((x.y + 1) > calc && (x.y - 1) < calc) //give it a little leaway
		on = true;
	return on;
}

double distance(point a, point b) {
	double distance;
	double dx = (a.x - b.x);
	double dy = (a.y - b.y);
	distance = sqrt((dx*dx) + (dy*dy));
	return distance;
}


//function that finds next node to go to
node go(node here, node en, point end) {
	int size = inters.size();
	vector<node> possa;
	vector<node> possb;
	vector<node> possnext;
	node next;
	double mina1 = 1000;
	double mina1Ind;
	double mina2 = 1000;
	double mina2Ind;
	double minb1 = 1000;
	double minb1Ind;
	double minb2 = 1000;
	double minb2Ind;
	double minnext = 1000;
	double minnextInd;


	for (int i = 0; i < size; i++) {
		if (onLine(inters[i].intersect, roads[here.a]))
			possa.push_back(inters[i]);
		else if (onLine(inters[i].intersect, roads[here.b]))
			possb.push_back(inters[i]);
	}

	for (int i = 0; i < possa.size(); i++)
	{
		if (possa[i].intersect.x - here.intersect.x < 0)
			if (distance(possa[i].intersect, here.intersect) < mina1) {
				mina1 = distance(possa[i].intersect, here.intersect);
				mina1Ind = i;
			}
			else if (possa[i].intersect.x - here.intersect.x > 0)
				if (distance(possa[i].intersect, here.intersect) < mina2) {
					mina2 = distance(possa[i].intersect, here.intersect);
					mina2Ind = i;
				}
	}

	for (int i = 0; i < possb.size(); i++)
	{
		if (possb[i].intersect.x - here.intersect.x < 0)
			if (distance(possb[i].intersect, here.intersect) < minb1) {
				minb1 = distance(possb[i].intersect, here.intersect);
				minb1Ind = i;
			}
			else if (possb[i].intersect.x - here.intersect.x > 0)
				if (distance(possb[i].intersect, here.intersect) < minb2) {
					minb2 = distance(possb[i].intersect, here.intersect);
					minb2Ind = i;
				}
	}

	possnext.push_back(possa[mina1Ind]);
	possnext.push_back(possa[mina2Ind]);
	possnext.push_back(possb[minb1Ind]);
	possnext.push_back(possb[minb2Ind]);

	for (int i = 0; i < possnext.size(); i++) {
		if ((distance(here.intersect, possnext[i].intersect) + distance(possnext[i].intersect, end) /*+ (distance(possnext[i].intersect, next.intersect)*distance(possnext[i].intersect, next.intersect))*/) < minnext)
		{
			minnext = (distance(here.intersect, possnext[i].intersect) + distance(possnext[i].intersect, end) /*+ (distance(possnext[i].intersect, next.intersect)*distance(possnext[i].intersect, next.intersect))*/);
			minnextInd = i;
		}
	}

	next = possnext[minnextInd];




	possa.clear();
	possb.clear();
	possnext.clear();
	return next;
}

node come(node here, node beg, point begin) {
	int size = inters.size();
	vector<node> possa;
	vector<node> possb;
	vector<node> possnext;
	node next;
	double mina1 = 1000;
	double mina1Ind;
	double mina2 = 1000;
	double mina2Ind;
	double minb1 = 1000;
	double minb1Ind;
	double minb2 = 1000;
	double minb2Ind;
	double minnext = 1000;
	double minnextInd;


	for (int i = 0; i < size; i++) {
		if (onLine(inters[i].intersect, roads[here.a]))
			possa.push_back(inters[i]);
		else if (onLine(inters[i].intersect, roads[here.b]))
			possb.push_back(inters[i]);
	}

	for (int i = 0; i < possa.size(); i++)
	{
		if (possa[i].intersect.x - here.intersect.x < 0)
			if (distance(possa[i].intersect, here.intersect) < mina1) {
				mina1 = distance(possa[i].intersect, here.intersect);
				mina1Ind = i;
			}
			else if (possa[i].intersect.x - here.intersect.x > 0)
				if (distance(possa[i].intersect, here.intersect) < mina2) {
					mina2 = distance(possa[i].intersect, here.intersect);
					mina2Ind = i;
				}
	}

	for (int i = 0; i < possb.size(); i++)
	{
		if (possb[i].intersect.x - here.intersect.x < 0)
			if (distance(possb[i].intersect, here.intersect) < minb1) {
				minb1 = distance(possb[i].intersect, here.intersect);
				minb1Ind = i;
			}
			else if (possb[i].intersect.x - here.intersect.x > 0)
				if (distance(possb[i].intersect, here.intersect) < minb2) {
					minb2 = distance(possb[i].intersect, here.intersect);
					minb2Ind = i;
				}
	}

	possnext.push_back(possa[mina1Ind]);
	possnext.push_back(possa[mina2Ind]);
	possnext.push_back(possb[minb1Ind]);
	possnext.push_back(possb[minb2Ind]);

	for (int i = 0; i < possnext.size(); i++) {
		if ((distance(here.intersect, possnext[i].intersect) + distance(possnext[i].intersect, begin) /*+ (distance(possnext[i].intersect, next.intersect)*distance(possnext[i].intersect, next.intersect))*/) < minnext)
		{
			minnext = (distance(here.intersect, possnext[i].intersect) + distance(possnext[i].intersect, begin) /*+ (distance(possnext[i].intersect, next.intersect)*distance(possnext[i].intersect, next.intersect))*/);
			minnextInd = i;
		}
	}

	next = possnext[minnextInd];




	possa.clear();
	possb.clear();
	possnext.clear();
	return next;
}

//function that searches if end comes before next intersection
bool search(node here, node next, point end) {
	bool closer = (distance(here.intersect, end) <= distance(here.intersect, next.intersect));
	bool on = (onLine(end, roads[here.a]) || onLine(end, roads[here.b]));

	if (closer && on)
		return true;
	else
		return false;
}

line whichLine(node here, point end) {
	line which;
	if (onLine(end, roads[here.a]))
		which = roads[here.a];
	else if (onLine(end, roads[here.b]))
		which = roads[here.b];
	return which;
}


//for determining which street is being taken to the next node
string take(node here, node next) {
	string take = "";
	if (here.a == next.a)
		take = roads[here.a].road_name;
	if (here.a == next.b)
		take = roads[here.a].road_name;
	if (here.b == next.a)
		take = roads[here.b].road_name;
	if (here.b == next.b)
		take = roads[here.b].road_name;
	return take;

}

bool loadMap() {
	ifstream inRoads;
	ifstream inInters;
	string find1 = "";
	string find2 = "";
	inRoads.open("roads.csv");
	line temp;
	node ntemp;
	if (!inRoads.is_open()) {
		cout << "\nError, roads.csv cannot be opened." << endl;
		return false;
	}
	while (!inRoads.eof()) {
		inRoads >> temp.begin.x;
		inRoads >> temp.begin.y;
		inRoads >> temp.end.x;
		inRoads >> temp.end.y;
		inRoads.ignore(10, ' ');
		getline(inRoads, temp.road_name);
		fixline(temp);
		roads.push_back(temp);
	}
	inRoads.close();
	inInters.open("inters.csv");
	if (!inInters.is_open()) {
		cout << "\nError, inters.csv cannot be opened." << endl;
		return false;
	}
	int count = 0;
	while (!inInters.eof()) {
		getline(inInters, find1, ',');
		inInters.ignore(10, ' ');
		getline(inInters, find2, ',');
		for (int i = 0; i < roads.size(); i++) {
			if (roads[i].road_name == find1)
				ntemp.a = i;
			if (roads[i].road_name == find2)
				ntemp.b = i;
		}
		inInters >> ntemp.intersect.x;
		inInters >> ntemp.intersect.y;
		inInters.ignore(1);
		ntemp.me = count;
		count++;
		inters.push_back(ntemp);
	}
	inInters.close();
	return true;
}

int closeToRoad(point x) {
	double minDistance = abs((roads[0].end.y - roads[0].begin.y)*x.x - (roads[0].end.x - roads[0].begin.x)*x.y
		+ roads[0].end.x*roads[0].begin.y - roads[0].end.y*roads[0].begin.x)
		/ sqrt((roads[0].end.y - roads[0].begin.y)*(roads[0].end.y - roads[0].begin.y)
			+ (roads[0].end.x - roads[0].begin.x)*(roads[0].end.x - roads[0].begin.x));
	double calcDist = 0;
	int saveMe = 0;
	for (int i = 1; i < roads.size(); i++)
	{
		calcDist = abs((roads[i].end.y - roads[i].begin.y)*x.x - (roads[i].end.x - roads[i].begin.x)*x.y
			+ roads[i].end.x*roads[i].begin.y - roads[i].end.y*roads[i].begin.x)
			/ sqrt((roads[i].end.y - roads[i].begin.y)*(roads[i].end.y - roads[i].begin.y)
				+ (roads[i].end.x - roads[i].begin.x)*(roads[i].end.x - roads[i].begin.x));
		if (calcDist < minDistance) {
			minDistance = calcDist;
			saveMe = i;
		}
	}
	return saveMe;
}

point closestPointOnRoad(line b, double x, double y) {
	point retMe;
	if (b.m > 999999999) {
		retMe.x = b.begin.x;
		retMe.y = y;
	}
	else {
		retMe.x = x / 2;
		retMe.y = b.m * retMe.x + b.b;
	}
	return retMe;
}

double absolute(double x) {
	if (x < 0)
		return x * -1;
}

vector<djnode> getSucc(int nodeIndex, int vectIndex) {
	//see if node is shared
	vector<node> checkFromMe;
	for (int i = 0; i < inters.size(); i++) {
		if (inters[nodeIndex].intersect == inters[i].intersect)
			checkFromMe.push_back(inters[i]);
	}
	//gt all lines from node
	vector<int> myLines;
	for (int i = 0; i < checkFromMe.size(); i++) {
		for (int j = 0; j < 2; j++) {
			if (j % 2 == 0)
				myLines.push_back(inters[checkFromMe[i].me].a);
			else
				myLines.push_back(inters[checkFromMe[i].me].b);
		}
	}

	//find all nodes connectd to each line
	vector<int> possNodes;
	for (int i = 0; i < inters.size(); i++) {
		/*if (i == nodeIndex) {*/
			for (int j = 0; j < myLines.size(); j++) {
				if (inters[i].a == myLines[j] || inters[i].b == myLines[j])
					possNodes.push_back(i);
			}
		/*}*/
	}
	djnode temp;
	vector<djnode> children;
	temp.parent = vectIndex;
	for (int i = 0; i < possNodes.size(); i++) {
		temp.indexNode = possNodes[i];
		temp.cost = absolute(distance(inters[possNodes[i]].intersect, inters[nodeIndex].intersect));
		children.push_back(temp);
	}
	return children;
}

vector<node> djik(point start, point end) {
	//create new nodes for beginning and end
	node begin;
	node goal;
	begin.a = closeToRoad(start);
	begin.b = begin.a;
	begin.intersect = closestPointOnRoad(roads[begin.a], start.x, start.y);
	goal.a = closeToRoad(end);
	goal.intersect = closestPointOnRoad(roads[goal.a], end.x, end.y);
	goal.b = goal.a;
	int theEnd = inters.back().me;
	begin.me = theEnd+1;
	theEnd++;
	goal.me = theEnd + 1;
	inters.push_back(begin);
	inters.push_back(goal);

	djnode temp;
	temp.parent = -1;
	temp.indexNode = begin.me;
	temp.me = 0;
	temp.cost = 0;
	nodeList.push_back(temp);
	priority_queue<djnode> frontier;
	frontier.push(temp);	
	double minDist = 99999999;
	djnode favChild;
	bool in = false;
	while (inters[frontier.top().indexNode].me != goal.me) {
		minDist = 99999999;
		favChild.indexNode = 0;
		favChild.parent = 0;
		in = false;

		temp = frontier.top();
		frontier.pop();
		vector<djnode> children = getSucc(temp.indexNode, temp.me);

		for (int i = 0; i < children.size(); i++) {
			for (int j = 0; j < nodeList.size(); j++) {//check if node is on the list
				if (nodeList[j].indexNode == children[i].indexNode) {
					if (children[i].cost < nodeList[j].cost) { //if it is, check if the cost is lower
						nodeList[j].cost = children[i].cost; //if so, this is a quicker route
						nodeList[j].parent = children[i].parent;
					}
					in = true;
				}
			}
			if (!in) {
				children[i].me = nodeList.back().me + 1;
				nodeList.push_back(children[i]);
				frontier.push(children[i]);
			}
		}
	}


	//think on this
	stack<node> revPath;
	revPath.push(inters[frontier.top().indexNode]);
	vector<int> path;
	node ntemp;
	int ptemp;
	while (revPath.top().me != begin.me) {
		ptemp = nodeList[revPath.top().me].parent;
		ntemp = inters[nodeList[revPath.top().me].me];
		revPath.push(ntemp);
		
	}


}

int main() {
	if (!loadMap()) {
		system("pause");
		return 1;
	}
	system("pause");
	return 0;

}

int main1() {
	point start = { 0,0 };
	point end = { 10,10 };
	node next1;
	next1.intersect = start;
	node next2;
	next2.intersect = end;
	vector<node> path;
	bool found = false;
	int foundID;
	line endline;
	string thisroad = "";
	//function that finds nearest road
	//	needs to narrow down search
	//		calculate distance between point and each line
	int travelled;
	//line a;
	//a.begin.x = 12;
	//a.begin.y = 15;
	//a.end.x = 121;
	//a.end.y = 151;
	//fixline(a);
	//cout << "m is : " << a.m << endl;
	path.push_back(next1);
	path.push_back(next2);
	auto it = path.begin() + 1;
	auto notit = path.end() - 1;

	while (!found) {
		path.insert(it, next1);
		path.insert(notit, next2);
		next1 = go(next1, next2, end);
		next2 = come(next2, next1, start);
		found = (next1.intersect.x == next2.intersect.x) && (next1.intersect.y == next2.intersect.y);
		if (found)
			foundID = path.size() - 1;
		path.insert(it, next1);
		path.insert(notit, next2);
		it++;
		notit--;
	}

	for (int i = 0; i < path.size(); i++) {
		if (i < path.size() - 1) {
			thisroad = take(path[i], path[i + 1]);
			cout << "at (" << path[i].intersect.x << "," << path[i].intersect.y << "),";
			cout << " take " << thisroad << " to";
			cout << "at (" << path[i + 1].intersect.x << "," << path[i + 1].intersect.y << ")" << endl;
		}
		else {
			thisroad = whichLine(path[i], end).road_name;
			cout << "at (" << path[i].intersect.x << "," << path[i].intersect.y << "),";
			cout << " take " << thisroad << " to";
			cout << "at (" << end.x << "," << end.y << ")" << endl;
		}

	}

	cout << "Congratulations! You've made it to your destination!";

	path.clear();
	system("pause");
	return 0;
}