#pragma once
#include "Node.h"
	   
#define MAX_NODE 1000

class Graph
{
private:
	Node* head;
	Node* temp_node; //temp node for store in Q

	unsigned long graph_size;
	int HIDDEN_SIZE;
public:
	Node* arr_url[MAX_NODE+2];
    	linkedqueue* G_queue;
	
	int a_index;
	Graph(void);
	~Graph(void);

	Node* count(Node* i_prior,char* i_url, long msecs);	//add a Node
	void discount(char* i_url);
 
	void QuickSort(int Left,int Right);
	int Partition(int Left,int Right);
	void Swap(int l, int r);
	int BinarySearch(int Size,char* i_url);
	Node* gethead();

        void set_temp_node(Node* temp_n);
	Node* get_temp_node();

	int LRU(Node* P);
	int search(int Size,char* i_url);

	void fill_edge_g(Node* P);
	void setHIDDEN_SIZE(int k) {HIDDEN_SIZE=k;}
};
