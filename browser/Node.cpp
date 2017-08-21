#include "Node.h"
#include <math.h>

#define EDGE_SIZE 300

Node::Node(void)
{
	url = NULL;
	v_count =0;
	memset(e_count,0,EDGE_SIZE*sizeof(int));
	memset(edge,NULL,EDGE_SIZE*sizeof(Node*));
	memset(Prior,NULL,50*sizeof(Node*));
	e_index = 0;
	p_index = 0;
	queue = new linkedqueue;

	msecs=0;
	memset(priorN,0,EDGE_SIZE*sizeof(double));
}
Node::~Node(void) 
{
} 

Node::Node(char* i_url,Node* pre, long msecs_v)
{
	url = new char[strlen(i_url)+1];
	strcpy(url,i_url);
	v_count = 1;
	memset(e_count,0,EDGE_SIZE*sizeof(int));
	memset(edge,NULL,EDGE_SIZE*sizeof(Node*));
	memset(Prior,NULL,50*sizeof(Node*));
	e_index = 0;
	p_index = 1;
	queue = new linkedqueue;

	if(pre !=NULL)
		Prior[0]=pre;

	msecs=msecs_v;
	memset(priorN,0,EDGE_SIZE*sizeof(double));
}

void Node::setPrior(int j)
{
	long deno = 0;

	for (int i = 0; i < e_index; i++){
		if(edge[i]!=NULL){
			deno += e_count[i];
		}
	}

	priorN[j] = ((double)e_count[j])*((double)(edge[j]->msecs));	

	return;
}

int Node::get_e_count(int i){
	return e_count[i];
}

Node** Node::get_edge(){
	return edge;
}
int Node::get_e_index()
{
	return e_index;
}
char* Node::insert_edge(Node* New)
{
	int temp=0;
	int temp1=0;
	int flag=0;

	Node* temp_node;
	if (e_index > 0)
	{
		if(New->url != NULL){
			for (int i = 0; i < e_index; i++) 
			{
				if (strcmp(edge[i]->url, New->url) == 0) 
				{
					e_count[i]++;
					flag=1;
					break;
				}
			}
		}
		else { 
			cout << "!!!NULL!!! Node::insert_edge(Node* New)" << endl;
			return 0;
		}
	}
	if(flag == 0){
		if(e_index < EDGE_SIZE) {
			edge[e_index]=New;
			e_count[e_index]=1;
			e_index++;
		}
	}
	for(int T=0; T<e_index; T++){
		setPrior(T);
	}
	for (int i = 0; i < e_index; i++) 
	{
		for(int j=i+1;j<e_index;j++)
		{
			if(priorN[i]< priorN[j])
			{
				temp = priorN[i];
				priorN[i]=priorN[j];
				priorN[j]= temp;

				temp_node = edge[i];
				edge[i]=edge[j];
				edge[j]= temp_node;

				temp1 = e_count[i];
				e_count[i]=e_count[j];
				e_count[j]= temp1;	
			}

		}	
	}

	return 0;
}
double Node::get_priorN(int In)
{
	return priorN[In];
}

int Node::comVCount(Node* a, Node* b, int allVCount){
	if(allVCount!=0){
		double ratioA = ((double)(a->v_count))*((double)(a->msecs));
		double ratioB = ((double)(b->v_count))*((double)(b->msecs));

		if(ratioA >= ratioB)
			return 1;
		else
			return -1;
	}
	return 0;
}

void Node::set_Prior(Node* pre)
{
	for(int i=0;i<=p_index;i++)
	{
		if(Prior[i] == NULL) {
			Prior[p_index]=pre;
			p_index++;
			break;
		}
	}
	return;
}

void Node::delete_edge(Node* New)
{
	int i=0;

	while (edge[i]!=0 && i<EDGE_SIZE)
	{
		if(strcmp(edge[i]->url, New->url)==0) 
		{
			for(int j=i; j<(EDGE_SIZE-1); j++) {
				edge[j] = edge[j+1];
				e_count[j] = e_count[j+1];
			}

			if(e_index!=0){
				edge[e_index-1] = NULL;
				e_count[e_index-1] = NULL;
				e_index--;
			}
			edge[EDGE_SIZE-1] = 0;
			e_count[EDGE_SIZE-1] = 0;
		}
		i++;
		if(i==EDGE_SIZE)
			break;
	}

	return;
}

int Node::get_p_index()
{
	return p_index;
}
Node* Node::get_Prior(int index)
{
	Node* back = Prior[index];
	Prior[index] = NULL;
	return back;
}

long Node::calLoadTime(long ms)
{
	msecs = ((msecs*(v_count-1))+ms)/v_count;

	return msecs;
}

