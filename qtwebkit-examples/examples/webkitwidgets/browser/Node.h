#pragma once
#include<iostream>
#include<fstream>
#include <cstring>
#include <ctime>
#include "linkedqueue.h"
using namespace std;

#define EDGE 100

class linkedqueue;
class my_queue;
class Node
{ 
private:
	//char* time;
	int e_count[EDGE];
	double priorN[EDGE];
	//temporlatory variable
	unsigned long node_size;
public:
	Node* Prior[50];
	int e_index, p_index;
	char* url;
	int v_count;	//{HONG} the number of count visiting the current node.
	linkedqueue* queue;
	Node* edge[EDGE];
	
	~Node(void);
	Node(void);
	Node(char* i_url,Node* pre, long msecs_v);//{H}
	
	char* insert_edge(Node* New);
	void delete_edge(Node* New);
	void set_Prior(Node* pre);
	Node* get_Prior(int);
	int get_p_index();
Node** get_edge();
int get_e_index();

	//char* title;
	long msecs;

	void setPrior(int j);
	//void renew_time(char* time);
	//char* get_time(void);

	long calLoadTime(long ms);

	int fill_edge();
	int comVCount(Node* a, Node* b, int allVCount);
	double get_priorN(int In);

	//temporlatory function
	void SetGSize();
	unsigned long getGSize();
};

