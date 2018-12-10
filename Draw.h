#include "Graph.h"
#include <graphics.h>
#include <math.h>

struct parameter { // ����
	double KR, KS; // Fr = Kr / D^2, Fs = Ks * (D - L), Kr >> Ks
	double L;	   // ���ɳ���
	double DELTAT; // x = fx * DELTAT, y = fy * DELTAT
};

class Draw {
public:
	Draw(Graph _graph);		// ��ȡ�ڽӱ�
	void Start();			// ��ʼ
	void DrawNode();		// ��ʾ�ڵ������
	void UpdateReplusion(); // ���¿��׳���
	void UpdateSpring();	// ���µ�������
	int Update();			// ���½ڵ�λ��
private:
	Graph graph;			// �ڽӱ�
	parameter par;			// ����
	vector<COLORREF> color;
};

Draw::Draw(Graph _graph) {
	srand(time(NULL));
	graph = _graph;
	color.resize(graph.verNum.size());
	for (int i = 0; i < graph.verNum.size(); i++) {
		ArcNode *tmpNode = graph.adjList[i].firstArc;
		if (tmpNode) {
			if (color[i] == NULL)
				color[i] = RGB(rand() % 255, rand() % 255, rand() % 255);
			COLORREF childColor RGB(rand() % 255, rand() % 255, rand() % 255);
			while (tmpNode) {
				color[tmpNode->adjvex] = childColor;
				tmpNode = tmpNode->nextArc;
			}
		}
	}
	par.L = 80;
	par.KR = 1500;
	par.KS = 0.01;
	par.DELTAT = 10;
}

void Draw::Start() {
	initgraph(1000, 1000, SHOWCONSOLE);
	setbkcolor(WHITE);
	cleardevice();
	for (int i = 0; ; i++) { // ����n��
		for (int j = 0; j < graph.verNum.size(); j++) // ��ʼ��λ��
			graph.adjList[j].forceX = 0.0, graph.adjList[j].forceY = 0.0;
		UpdateReplusion();
		UpdateSpring();
		// ����һǧ��
		if (i > 1000)
			return;
		// ������нڵ�����û�иı��˳�ѭ��
		if (double(Update()) / double(graph.verNum.size()) == 1.0) {
			cout << i << endl;
			return;
		}
		cleardevice();
		DrawNode();
	}
}

void Draw::DrawNode() {
	for (int i = 0; i < graph.verNum.size(); i++) {
		ArcNode *tmpNode = graph.adjList[i].firstArc;
		while (tmpNode) {
			setlinestyle(PS_SOLID, 3);
			setlinecolor(BLACK);
			line(graph.adjList[i].x, graph.adjList[i].y, graph.adjList[tmpNode->adjvex].x, graph.adjList[tmpNode->adjvex].y);
			setfillcolor(color[tmpNode->adjvex]);
			solidcircle(graph.adjList[tmpNode->adjvex].x, graph.adjList[tmpNode->adjvex].y, 15);
			tmpNode = tmpNode->nextArc;
		}
		setfillcolor(color[i]);
		solidcircle(graph.adjList[i].x, graph.adjList[i].y, 15);
	}
}

void Draw::UpdateReplusion() {
	double dx, dy, f, fx, fy, d, dsq;
	for (int i = 0; i < graph.verNum.size(); i++) {
		for (int j = i + 1; j < graph.verNum.size(); j++) {
			dx = graph.adjList[i].x - graph.adjList[j].x;
			dy = graph.adjList[i].y - graph.adjList[j].y;
			dsq = dx * dx + dy * dy;
			d = sqrt(dsq);
			f = par.KR / dsq; // Fr = Kr / D^2
			fx = f * dx / d;
			fy = f * dy / d;
			graph.adjList[i].forceX += fx;
			graph.adjList[i].forceY += fy;
			graph.adjList[j].forceX -= fx;
			graph.adjList[j].forceY -= fy;
		}
	}
}

void Draw::UpdateSpring() {
	double dx, dy, f, fx, fy, d;
	for (int i = 0; i < graph.verNum.size(); i++) {
		ArcNode *tmpNode = graph.adjList[i].firstArc;
		while (tmpNode) {
			int i2 = tmpNode->adjvex;
			dx = graph.adjList[i2].x - graph.adjList[i].x;
			dy = graph.adjList[i2].y - graph.adjList[i].y;
			d = sqrt(dx * dx + dy * dy);
			f = par.KS * (d - par.L); // Fs = Ks * (D - L)
			fx = f * dx / d;
			fy = f * dy / d;
			graph.adjList[i].forceX += fx;
			graph.adjList[i].forceY += fy;
			graph.adjList[i2].forceX -= fx;
			graph.adjList[i2].forceY -= fy;
			tmpNode = tmpNode->nextArc;
		}
	}
}

int Draw::Update() {
	double dx, dy, dsq, s;
	int cnt = 0;
	for (int i = 0; i < graph.verNum.size(); i++) {
		dx = par.DELTAT * graph.adjList[i].forceX;
		dy = par.DELTAT * graph.adjList[i].forceY;
		dsq = dx * dx + dy * dy;
		if (dsq > 50) { // �������λ�Ƴ���
			s = sqrt(50 / dsq);
			dx *= s;
			dy *= s;
		}
		graph.adjList[i].x += dx;
		graph.adjList[i].y += dy;
		// ͳ�����겻��Ľڵ����
		if (fabs(dx) <= 0.1 && fabs(dy) <= 0.1)
			cnt++;
	}
	cout << cnt << endl;
	return cnt;
}

