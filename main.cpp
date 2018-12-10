#include "Draw.h"
#include <iostream>

using namespace std;

int main() {
	Graph graph;
	graph.CreateGraph("data/data.txt");
	Draw draw(graph);
	draw.Start();
	system("pause");
}
