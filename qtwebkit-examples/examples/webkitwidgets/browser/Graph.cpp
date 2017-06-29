#include "Graph.h"
#include<string.h> 

#define MAX_NODE 1500
//#define HIDDEN_SIZE 1

Graph::Graph(void)
{
	//boost_graph g; 
	head=NULL;
	  
	memset(&arr_url[0],NULL, sizeof(Node*)*(MAX_NODE+2));
	a_index=0;
	temp_node=NULL; 
    G_queue = new linkedqueue;
}
Graph::~Graph(void)
{    
}

unsigned long Graph::sgetGSize() {
	Node getSizeNode;
	getSizeNode.SetGSize();

	graph_size = sizeof(Node*) + sizeof(Node*) + sizeof(linkedqueue*) + sizeof(int) + sizeof(Node*) + sizeof(void) + sizeof(void) + sizeof(int) + sizeof(void) + sizeof(int) + sizeof(Node*) + sizeof(void) + sizeof(Node*); // + (sizeof(Node*)*20000)

	graph_size += getSizeNode.getGSize();

	return graph_size;
}
  
Node* Graph::count(Node* i_prior,char* i_url, long msecs)
{
		char* sad;
	if(!head)   
	{
		Node* New = new Node(i_url, i_prior, msecs);
		head = New;
		arr_url[a_index]=New;
		a_index++;

		New->calLoadTime(msecs); //{H} calulate loading time

		return New;
	}
 
	//{H} here
	int id = search(a_index,i_url);  // find same url in graph
	Node* val = NULL;
	if(id>=0)
		val = arr_url[id];

	if(val!=NULL)
	{
		my_queue* new_queue = new my_queue;
	        new_queue->set_node(i_prior);
		
		val->v_count++;
		//{H} here 
		i_prior->insert_edge(val);  // add edge
		//val->set_Prior(i_prior);
		
		val->queue->Enqueue(val->queue, new_queue); //  for log  when  end of browser 
	
		val->calLoadTime(msecs); //{H} calulate loading time

		//////{H} fill empty Nodes.
		fill_edge_g(val);
		//val->fill_edge();

		///////////{H} LRU algorithm
		LRU(val);
		////////////////////////////
		return val;
	}
	else    // new node
	{
		my_queue* new_queue = new my_queue;
		new_queue->set_node(i_prior);
		Node* New = new Node(i_url,i_prior, msecs);
		sad = i_prior->insert_edge(New);
		arr_url[a_index]=New;

		fill_edge_g(New);
		//New->fill_edge();
		///////////{H} LRU algorithm
		LRU(New);

		/*if(sad != 0) {
			Graph::discount(sad);
		}
		else if(sad == 0){
 			a_index++;
			QuickSort(0,a_index-1);
		}*/

		New->queue->Enqueue(New->queue, new_queue);

		New->calLoadTime(msecs); //{H} calulate loading time
		
		return New;
	}
	return 0;
}

void Graph::fill_edge_g(Node* P){
	////////////////////////////////////////////////////////////////////
	////			{H} Analysis of log			////
	////////////////////////////////////////////////////////////////////
	Node* store[1500];
	Node* sort[1500];
	int preNodeRank[1500]={0,};
	int sortRank[1500]={0,};
	int b,c,d,e;	//{H} the function paratmeter FOR()
	int a2=0;
	int aa, bb, cc, dd, ee;
	for(int m=0; m<=1499; m++){
		store[m]=NULL;
		sort[m]=NULL;
	}

	Node* keyNode;
//cout << "arr_url[0] : " << arr_url[0]->url << endl;
	////////////////////////////////////////////////////////////////////
	while(arr_url[a2]!=NULL){
		preNodeRank[a2] = arr_url[a2]->v_count;
		a2++;
		if(a2>1499){
			a2 = 1500;
			break;
		}
	}			
//cout << "a2 : " <<a2<<endl;
	/*for(c=0; c<a2; c++){
		preNodeRank[c] = arr_url[c]->v_count;
	}*/
//cout << "11"<<endl;
	e=0;
	if(arr_url[e]!=NULL) {
		sort[e] = arr_url[e];
		sortRank[e]= preNodeRank[e];
	}
	e++;
	int sortKey=0;
//cout << "22"<<endl;
	for(aa=0; aa<a2; aa++){
		for(int a1=0; a1<e; a1++){
			if(strcmp(arr_url[aa]->url, sort[a1]->url)==0){
				sortKey=1;
			}
		}
		if(sortKey==0){
			sort[e]=arr_url[aa];
			sortRank[e]= preNodeRank[aa];
			e++;
		}
		sortKey=0;
	}
//cout << "33"<<endl;

	//{H}insertionSort
	for(bb=1; bb<e; bb++){
		memcpy(&keyNode, &sort[bb], sizeof(Node*));
		sortKey = sortRank[bb];
		for(cc=bb-1; cc>=0; cc--){
			if(sortKey>sortRank[cc]){
				memcpy(&sort[cc+1], &sort[cc], sizeof(Node*));
				sortRank[cc+1] = sortRank[cc];
			}
			else{
				break;
			}
		}
		memcpy(&sort[cc+1], &keyNode, sizeof(Node*));
		sortRank[cc+1] = sortKey;
		memset(&keyNode, NULL, sizeof(Node*));
	}
	sortKey=0;
//cout << "44"<<endl;

///////////////////////////////////////////////////
	int edgeCon=0;
	int edgeKey=0;
	int Pcon=P->e_index;
	while(sort[edgeCon]!=NULL){
		if(P->e_index >= HIDDEN_SIZE)
			break;

		for(int con=0; con<P->e_index; con++){	
			if(strcmp(P->edge[con]->url, sort[edgeCon]->url)==0){
				edgeKey=1;
				break;
			}
		}					

		if(edgeKey==0 && Pcon<HIDDEN_SIZE) {
			P->edge[Pcon] = sort[edgeCon];
//cout<<"P->edge[Pcon] : " << P->edge[Pcon]->url << endl;
			Pcon++;
		}
		edgeCon++;
		if(edgeCon>=HIDDEN_SIZE){
			P->edge[HIDDEN_SIZE]=NULL;
			break;
		}
	}
///////////////////////////////////////////////////

	for(ee=0; ee<a2; ee++){
		preNodeRank[ee]=0;
		sortRank[ee]=0;
		sort[ee]=NULL;
		//memset(&sort[ee], NULL, sizeof(Node*));
	}

	return;
}


int Graph::LRU(Node* P) {
	int id = -1;
	int l=0;
	Node* val = NULL;
	Node* New;
	
	int c=0;

	id = search(a_index, P->url);

	if(id >= 0) {
		if(id != (a_index-1))	{
			arr_url[MAX_NODE+1]=arr_url[id];
			for(int i=id; i<a_index; i++) {
				arr_url[i]=arr_url[i+1];
			}
			arr_url[a_index-1]=arr_url[MAX_NODE+1];
			arr_url[MAX_NODE+1]=0;				
		}
	}
	else if(a_index >= MAX_NODE) {
		l=0;
		while(arr_url[0]->Prior[l] != NULL && l < 50) {
			arr_url[0]->Prior[l]->delete_edge(arr_url[0]);
			l++;
		}
//cout<< "arr_url[0] : " << arr_url[0]->url<<endl;
		/*while(arr_url[c]!=NULL){
			if(c>=MAX_NODE)
				break;

			arr_url[c]->delete_edge(arr_url[0]);
			c++;
		}*/
		l=0;
		for(int i=0; i<a_index-1; i++) {
			arr_url[i]=arr_url[i+1];
		}
		arr_url[MAX_NODE-1] = P;
	}
	else {
		arr_url[a_index] = P;
		a_index++;
		QuickSort(0,a_index-1);
	}

	return 0;
}

void Graph::discount(char* i_url)
{
	if(!head)
		return;
	int id = BinarySearch(a_index,i_url);

	if (id < 0) return;  

	Node* val = arr_url[id];
	if(val!=0)  
	{
		Node* New = new Node;
		val->v_count--;
		if (val != head){
			New = val->queue->Dequeue(val->queue)->get_node();
			New->delete_edge(val);
		}

		if(val->v_count==0)
		{
			memmove(arr_url+id,arr_url+id+1,(a_index-id-1)*sizeof(arr_url[0]));
			arr_url[a_index-1]=NULL;
			a_index--;
		}
	}

	return;
}

int Graph::search(int Size,char* i_url)
{
	for(int i=0; i < Size; i++) {
		if (strcmp(i_url,arr_url[i]->url)==0) {
			return i;
		}
	}

	return -1;
}

int Graph::BinarySearch(int Size,char* i_url)
{
    int Left, Right, Mid;

    Left = 0;
    Right = Size-1;
    while (Left <= Right)
    {
        Mid = (Left + Right) / 2;
	if (strcmp(i_url,arr_url[Mid]->url)==0)//Target == arr_url[Mid])
            return Mid;
        else if (strcmp(i_url,arr_url[Mid]->url)>0)
            Left = Mid + 1;
        else
            Right = Mid - 1;
    }
    return -1;
}

Node* Graph::gethead()
{
	return head;
}

//////////////////////////////////////////////start_ sorting function 
void Graph::Swap(int l, int r)
{
    Node* temp = arr_url[l];
	arr_url[l] = arr_url[r];
	arr_url[r] = temp;
}


int Graph::Partition(int Left,int Right)
{
    int First = Left;
    Node* Pivot = arr_url[First];

    ++Left;

    while( Left <= Right )
    {
		while((strcmp(arr_url[Left]->url,Pivot->url)<=0) /*DataSet[Left] <= Pivot*/ && Left < Right )
            ++Left;
        while((strcmp(arr_url[Right]->url,Pivot->url)>=0) /*DataSet[Right] >= Pivot*/ && Left <= Right )
            --Right;

        if ( Left < Right )
            Swap(Left,Right);
        else
            break;
    }

    Swap(First, Right);

    return Right;
}

void Graph::QuickSort(int Left, int Right)
{
    if ( Left < Right )
    {
        int Index = Partition(Left,Right);

        QuickSort(Left,Index - 1 );
        QuickSort(Index + 1,Right);
    }
}
/////////////////////////////////////////////////////////end sorting function

void Graph:: set_temp_node(Node* temp_n)
{
	temp_node = temp_n;
}
Node* Graph:: get_temp_node()
{
	return temp_node;
}
