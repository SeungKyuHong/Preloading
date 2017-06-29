#include <iostream>
#include <cstring>
#include <fstream>
#include <string>
#include "Graph.h"
#include "linkedqueue.h"
#include "my_queue.h"
#include "Node.h"
#include "browserapplication.h"
#include "settings.h"

#include <QtCore/QSettings>

using namespace std;

//////////////////////////////////////////////
//			\BDùķ\B9\C0̼\C7\C0\BB \C0\A7\C7\D1 \C4ڵ\E5			//
//////////////////////////////////////////////
class manage
{
private:
	//ifstream history;
	//QSettings settings;
	float hitRatio;
	float loadTime;
	int HIDDEN_SIZE;
public:
	manage();
	~manage();

	ifstream history;
	ofstream result;
	ofstream com;

	int hitCount;
	int totalCount;
	int j;
	void makeGraph();

	//\BE\B0\D4 \BE\F7\B5\A5\C0\CCƮ\C7ؾ\DF \C7ϳ\AA 1) \BF\A7\C1\F6\B8\A6 \B5\FB\B6\F3 \BF켱\BC\F8\C0\A7 \C1\A4\B7\C4 -> \C8\FC \C0̿\EB 2)qt\BF\A1\BC\AD \BB\E7\BF\EB -> qt \BAм\AE\BF\A1 \BDð\A3\C0\CC \B4\F5 \B0ɸ\B1\B0\CD \B0\B0\C0\BA\C1\A6 \C4ڵ\E5\B4\C2 \B0\A3\B4\DC
	void updateGraph();
	void comHit(int HI);
	void close();
	void open();

	//static Graph* g1();
	Graph *s_g_head;

	//static char* url;
	char* url;
	char* hUrl;

	unsigned long totalTime;
	unsigned long preLoadTime;

	int lessEdgeNodeCount;
	int m_reloadCount;
	char* initialUrl;

	int comLog();
	void comHit2();
	void comHit_comNode();
	
	void accesstime();
};
