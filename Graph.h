#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <stdlib.h>
#include <time.h>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::fstream;
using std::ios;
using std::map;
using std::vector;

const int MAXN = 100;

typedef struct ArcNode {
	ArcNode() :adjvex(0), weight(0), nextArc(NULL) {};
	int adjvex;
	int weight;
	struct ArcNode *nextArc;
}ArcNode;

typedef struct VertexNode {
	VertexNode() :data(0), forceX(0.0), forceY(0.0), firstArc(NULL) {};
	int data;
	double x, y;
	double forceX, forceY;
	ArcNode *firstArc;
}VertexNode, AdjList[MAXN];

class Graph {
public:
	Graph();
	~Graph();
public:
	AdjList adjList;
	map<int, int> verNum;
	int arcNum;
public:
	void CreateGraph(string filePath);    // 从文件读取图
	void InsertEdge(int v, int u, int w); // 插入边
};

Graph::Graph() {
	srand(time(NULL));
	verNum.clear();
	arcNum = 0;
	for (int i = 0; i < MAXN; i++) {
		adjList[i].data = -1;
		adjList[i].firstArc = NULL;
		adjList[i].x = 500 + rand() % 100 - 100;
		adjList[i].y = 500 + rand() % 100 - 100;
	}
}

Graph::~Graph() {
}

void Graph::CreateGraph(string filePath) {
	fstream graphFile;
	graphFile.open(filePath, ios::in | ios::out);
	string line;
	while (getline(graphFile, line)) {
		string tmpNum = "";
		int recode[3] = { 0 }, cnt = 0;
		for (int i = 0; i < line.length(); i++) {
			if (line[i] == ' ') {
				recode[cnt++] = atoi(tmpNum.c_str());
				tmpNum = "";
			}
			if (isdigit(line[i]))
				tmpNum += line[i];
		}
		recode[2] = atoi(tmpNum.c_str());
		InsertEdge(recode[0], recode[1], recode[2]);
		verNum[recode[0]] = 1, verNum[recode[1]] = 1;
		arcNum++;
	}
	cout << verNum.size() << "---" << arcNum << endl;
	for (int i = 0; i < verNum.size(); i++) {
		ArcNode *tmpNode = adjList[i].firstArc;
		cout << i;
		while (tmpNode) {
			cout << "->" << tmpNode->adjvex;
			tmpNode = tmpNode->nextArc;
		}
		cout << endl;
	}
}

void Graph::InsertEdge(int v, int u, int w) {
	ArcNode *newNode = new ArcNode();
	newNode->adjvex = u;
	newNode->weight = w;
	newNode->nextArc = adjList[v].firstArc;
	adjList[v].firstArc = newNode;
}