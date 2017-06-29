#include "Node.h"
#include <math.h>

#define EDGE_SIZE 100

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
//{H}
void Node::setPrior(int j)
{
	long deno = 0;
	float PEIndex = 0;

	for (int i = 0; i < e_index; i++){
		if(edge[i]!=NULL){
			deno += e_count[i];
		}
	}
	//priorN[j] = ((double)e_count[j]/deno)*((double)edge[j]->msecs-0.06);	
	priorN[j] = e_count[j];
//cout << "edge[j]->msecs/1000 : " << (double)edge[j]->msecs/1000 << endl;
	return;
}

Node** Node:: get_edge(){
	return edge;
}
int Node::get_e_index()
{
	return e_index;
}
char* Node::insert_edge(Node* New)
{
	int temp=0;
	int flag =0;

	Node* temp_node;
	Node* temp2;
	if (e_index > 0)
	{
		if(New->url != NULL){
			for (int i = 0; i < e_index; i++) 
			{
//cout << "0. e_index : " << e_index << " " << url<< endl;
				if (strcmp(edge[i]->url, New->url) == 0) 
				{
					e_count[i]++;
					//return;
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
//cout << "1. e_index : " << e_index << " " << url<< endl;
	if(flag == 0){
		if(e_index < EDGE_SIZE) {
			edge[e_index]=New;
			e_count[e_index]=1;
			e_index++;
		}
	}
//cout << "2. e_index : " << e_index << " " << url<< endl;
	//priorN[e_index-1] = e_count[e_index-1];
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
			}
			/*else if(priorN[i] == priorN[j])	//로딩시간 고려
			{
				if(edge[i]->msecs < edge[j]->msecs) {
					temp = priorN[i];
					priorN[i]=priorN[j];
					priorN[j]= temp;

					temp_node = edge[i];
					edge[i]=edge[j];
					edge[j]= temp_node;	
				}
			}*/
		}	
	}

	for (int ii = 0; ii < e_index; ii++){
		//cout << "priorN["<<ii<<"] : " << priorN[ii] << endl;
	}
//cout << "3. e_index : " << e_index << " " << url<< endl;
	return 0;
}
double Node::get_priorN(int In)
{
	return priorN[In];
}
int Node::fill_edge() {
	int curIndex = e_index;
	int WSize = 10;
	int WCount=0;

	int initIndex = e_index;
	int endIndex = 4;
	int sw=0;
/////////////////////////////////////////////////////
	int allVCount=0;

	for(int ii=0; ii < p_index; ii++){
		if(p_index >= EDGE_SIZE)
			break;
		if(Prior[ii] != NULL) {
			allVCount += Prior[ii]->v_count;
		}
		else {
			continue;
		}
	}
	for(int gg=0; gg < p_index; gg++){
		if(gg >= EDGE_SIZE)
			break;
		if(Prior[gg] != NULL) {
			for(int jj=0; jj < Prior[gg]->p_index; jj++){
				if(jj >= EDGE_SIZE)
					break;
				if(Prior[gg]->Prior[jj] != NULL) {
					allVCount += Prior[gg]->Prior[jj]->v_count;
				}
				else {
					continue;
				}
			}
		}
	}
	//cout << "allVCount : " << allVCount << endl;
/////////////////////////////////////////////////////
	if(e_index < 5) {
		while(WCount < WSize) {
			for(int i=0; i < p_index; i++){
				if(i >= EDGE_SIZE)
					break;
				if(Prior[i] != NULL) {
					if(edge[initIndex] == 0){
//cout << "11\n";
						edge[initIndex] = Prior[i];
						e_count[initIndex] = 0;

						if(initIndex >= endIndex){
							initIndex = e_index;
						}
						else {
							initIndex++;
						}
					}
					else {
						if(comVCount(edge[initIndex], Prior[i], allVCount) < 0)	//{H} here
						{
//cout << "22\n";
							edge[initIndex] = Prior[i];
							e_count[initIndex] = 0;	
						}
						
						if(initIndex >= endIndex){
							initIndex = e_index;
						}
						else {
							initIndex++;
						}
					}
				}
				else {
					continue;
				}
				WCount++;
				if(WCount >= WSize)
					break;	
			}
//cout << "33\n";
			for(int g=0; g < p_index; g++){
				if(g >= EDGE_SIZE)
					break;
				if(Prior[g] != NULL) {
					for(int j=0; j < Prior[g]->p_index; j++){
						if(j >= EDGE_SIZE)
							break;
						if(Prior[g]->Prior[j] != NULL) {
							if(edge[initIndex] == 0){
//cout << "44\n";
								edge[initIndex] = Prior[g]->Prior[j];
								e_count[initIndex] = 0;

								if(initIndex >= endIndex){
								initIndex = e_index;
								}
								else {
									initIndex++;
								}
							}
							else {
								//if(edge[initIndex]->v_count < Prior[g]->Prior[j]->v_count)	//{H} here
								if(comVCount(edge[initIndex], Prior[g]->Prior[j], allVCount) < 0)	//{H} here
								{
//cout << "55\n";
									edge[initIndex] = Prior[g]->Prior[j];
									e_count[initIndex] = 0;	
								}
						
								if(initIndex >= endIndex){
									initIndex = e_index;
								}
								else {
									initIndex++;
								}
							}
						}
						else {
							continue;
						}
					}
				}
				WCount++;
				if(WCount >= WSize)
					break;
			}
			break;
		}
	}
	/*initIndex=0;
	while(edge[initIndex]!=NULL){
		cout<< "edge["<<initIndex<<"] : " << e_count[initIndex]<<endl;
		initIndex++;
	}*/
//cout << "66\n";
	return 0;
}

int Node::comVCount(Node* a, Node* b, int allVCount){
	if(allVCount!=0){
		double ratioA = ((double)a->v_count/allVCount)*((double)a->msecs-0.06);
		double ratioB = ((double)b->v_count/allVCount)*((double)b->msecs-0.06);
	
		if(ratioA >= ratioB)
			return 1;
		else
			return -1;
	}
	return 0;
}

//{H} here
void Node::set_Prior(Node* pre)
{
	bool valid=true;
	for(int i=0;i<=p_index;i++)
	{
		if(Prior[i] == NULL) {
			Prior[p_index]=pre;
			p_index++;
			break;
		}

		if(strcmp(Prior[i]->url,pre->url)==0)
		{
			valid = false;
			break;
		}
	}
	return;
}

void Node::delete_edge(Node* New)
{
	int sw=0;
	int i=0;

	while (edge[i]!=NULL && i<EDGE_SIZE)
	{
		if(strcmp(edge[i]->url, New->url)==0) 
		{
			for(int j=i; j<(e_index-1); j++) {
				edge[j] = edge[j+1];
			}
			edge[e_index-1] = NULL;
			edge[EDGE_SIZE-1] = 0;
			e_count[EDGE_SIZE-1] = 0;
			e_index--;	
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

///////
//{H}//
///////
long Node::calLoadTime(long ms)
{
	msecs = ((msecs*(v_count-1))+ms)/v_count;

	return msecs;
}

void Node::SetGSize() {
	node_size = (sizeof(Node*)*EDGE_SIZE) + (sizeof(int)*EDGE_SIZE) + (sizeof(int)*2) + sizeof(int) + sizeof(char*) + sizeof(int) + sizeof(linkedqueue*) + (sizeof(Node*)*50) + sizeof(char*) + sizeof(void) + sizeof(void) + sizeof(Node*) + sizeof(int) + sizeof(Node**) + sizeof(int) + sizeof(long) + sizeof(int) + sizeof(void) + sizeof(long);
}

unsigned long Node::getGSize() {
	return node_size;
}
