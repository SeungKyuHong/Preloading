#include "Manage.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
using namespace std;

//#define HIDDEN_SIZE 1

manage::manage() {
	hitCount = 0;
	totalCount = 0;
	totalTime = 0;
	preLoadTime = 0;

	j = 0;
	hUrl = "www.naver.com";

	lessEdgeNodeCount = 0;
	m_reloadCount = 0;
	initialUrl = "https://www.google.co.kr/";
}

manage::~manage() {
	//history.close();
}

void manage::open() {
	history.open("history/6_ogt1.txt");
	result.open("history/6_ogt1_re_4_lt.csv", ios::app);
	result << "# Node, all count, hit count, without preloading, with preloading, Hit ratio, Improved loading time\n";
}

void manage::close() {
	history.close();

	Graph* graph_size = new Graph;
	Node* node_size = new Node;
	unsigned long getGSize=0;
	unsigned long c_getGSize=0;
	unsigned long getNSize=0;

	node_size -> SetGSize();
	getNSize = node_size->getGSize();
	getGSize = graph_size->sgetGSize(); + (sizeof(Node*)*(s_g_head->a_index));

	for(int g=0; g < s_g_head->a_index; g++) {
		getGSize += getNSize;
		if(getGSize >= 1048576) {
			c_getGSize++;
			getGSize = getGSize - 1048576;
		}
	}

cout << "s_g_head->a_index : " << s_g_head->a_index << endl;
	cout << "# Node, all count, hit count, without preloading, with preloading\n";
	cout << s_g_head->a_index << ", " << totalCount << ", "<< hitCount << ", "<< totalTime << ", " << preLoadTime<< ", " << hitRatio << ", " << loadTime << endl;
	result << s_g_head->a_index << ", " << totalCount << ", "<< hitCount << ", "<< totalTime << ", " << preLoadTime << ", " << hitRatio << ", " << loadTime << endl;

	cout<<"getGSize : " << c_getGSize << ", " << getGSize << endl;
	result<<"getGSize : " << c_getGSize << ", "  << getGSize << endl;
	cout<<"getNSize : " << getNSize << endl;
	result<<"getNSize : " << getNSize << endl;

	cout<<"m_reloadCount : " << m_reloadCount << endl;
	result<<"m_reloadCount : " << m_reloadCount << endl;
	cout<<"lessEdgeNodeCount : " << lessEdgeNodeCount << endl;
	result<<"lessEdgeNodeCount : " << lessEdgeNodeCount << endl;

	result.close();
}

void manage::makeGraph()
{
	Node* temp_node = NULL;
	int i = 0;

	char readStr[1200] = { 0, };
	char temp_token[1200]={0,};
	char temp_token_2[1200]={0,}; //{H}
	int sw =0;

	s_g_head = new Graph;

	if(history.fail() == false){
		while (i <= 0) {
			if(history.eof())
				break;

			history.getline(readStr, 1200);

			if (strlen(readStr) == 0)
				continue;
			////////////////////////////////	
			if(sw == 0)
				strcpy(temp_token,strtok(readStr,"* \n"));
			else
				strcpy(temp_token_2,strtok(readStr,"* \n"));	

			if (strcmp(temp_token, "url") == 0 && sw == 0)
			{
				strcpy(temp_token, strtok(NULL, " "));
				sw=1;
			}
			else if (strcmp(temp_token_2, "loading_time") == 0 && sw == 1)
			{
				strcpy(temp_token_2, strtok(NULL, " "));

				s_g_head->set_temp_node(s_g_head->count(temp_node, temp_token, atol(temp_token_2)));
				temp_node = s_g_head->get_temp_node();
				//temp_node2= BrowserApplication::gethead()->get_temp_node();
				cout <<i << " " << temp_token <<endl;
				totalCount++;
				sw=0;
				memset(temp_token, 0, 1200);
				memset(temp_token_2, 0, 1200);
				i++;
				break;
			}
			////////////////////////////////
		}
	}
	j=1;

	return;
}


/*
Graph* manage::g1() {
	if (!s_g_head) {
		s_g_head = new Graph;
	}
	return s_g_head;
}
*/
void manage::updateGraph()
{

}

void manage::accesstime(){
	char url_data[1200] = { 0, };
	char temp_token[1200]={0,};
	
	int buf = 0;
	int count = 0;
	float ans = 0;
//////////////
ifstream hsk;
ofstream hsk2("history/1_hsk1_time.csv");
hsk.open("history/log2.txt");
hsk2 << "AccessTime"<<endl;
/////////////
	//int j = 0;

	if(hsk.fail() == false){
		while(!hsk.eof())
		{
			hsk.getline(url_data,1200);
			if (strlen(url_data) == 0) continue;	
		
			if (strstr(url_data, "Visit Time")!=0) {

			for(int i=0; i< strlen(url_data); i++) {
				if(url_data[i] == ' ') {
					if(url_data[i+1] == ':' && url_data[i+2] == ' ') {
						memmove(&url_data[0], &url_data[i+3], strlen(url_data)-(i+3));
						memset(&url_data[strlen(url_data)-(i+3)], 0, strlen(url_data)-(i+3)+1);
					} 
				}
			}

			if (strstr(url_data, "오후")!=0) {	
				//if(strstr(url_data, "Length") == 0) {
				/*for(int i=0; i< strlen(url_data); i++) {
					if(url_data[i] == '후') {
						memmove(&url_data[0], &url_data[i+2], strlen(url_data)-(i+2));
						memset(&url_data[strlen(url_data)-(i+2)], 0, strlen(url_data)-(i+2)+1);
					}
				}*/
				memmove(&url_data[0], &url_data[18], strlen(url_data)-(18));
				memset(&url_data[strlen(url_data)-(18)], 0, strlen(url_data)-(18)+1);
				for(int j=0; j<strlen(url_data); j++) {
					if(url_data[j] == ':') {
						memmove(&url_data[j], &url_data[j+1], strlen(url_data)-(j+1));
						memset(&url_data[strlen(url_data)-1], 0, 1);
					}
				}
				buf = atoi(url_data) + 120000;
				cout << url_data << endl;
					//}	
			}
			else if(strstr(url_data, "오전")!=0){
				/*for(int i=0; i< strlen(url_data); i++) {
					if(url_data[i] == '전') {
						memmove(&url_data[0], &url_data[i+2], strlen(url_data)-(i+2));
						memset(&url_data[strlen(url_data)-(i+2)], 0, strlen(url_data)-(i+2)+1);
					}
				}*/
				memmove(&url_data[0], &url_data[18], strlen(url_data)-(18));
				memset(&url_data[strlen(url_data)-(18)], 0, strlen(url_data)-(18)+1);
				for(int j=0; j<strlen(url_data); j++) {
					if(url_data[j] == ':') {
						memmove(&url_data[j], &url_data[j+1], strlen(url_data)-(j+1));
						memset(&url_data[strlen(url_data)-1], 0, 1);
					}
				}
				buf = atoi(url_data);
				cout << url_data << endl;
			}
		
			if(ans == 0){
				ans = buf;
				continue;
			}
			else{
				ans = buf - ans;
				hsk2 << ans << endl;
				ans = buf;
			}
			//hsk << atoi(url_data) << endl;
			//count++;
			//buf = atoi(url_data);
			//ans = (ans+buf)/count;
			}		
		}	
	}

//////////
hsk.close();
hsk2.close();
//////////
	return;
}

///////////////////////////////////////////////
////////È÷Æ®žŠ °Ë»çÇÏ°í ±×·¡ÇÁžŠ °»œÅ//////////
///////////////////////////////////////////////
void manage::comHit(int HI)
{
	HIDDEN_SIZE=HI;
	s_g_head->setHIDDEN_SIZE(HIDDEN_SIZE);

	Node* curNode = s_g_head->gethead();
	int i = 0;
	Node* temp_node = NULL;

	char readStr[1200] = { 0, };
	char* temp = NULL;
	char temp_token[1200]={0,};
	char temp_token_2[1200]={0,}; //{H}
	int sw = 0;
	int hitSw = 0;

	//int j = 0;

	while(!history.eof())
	{
		i=0;
		history.getline(readStr, 1200);
		if (strlen(readStr) == 0) continue;

		////////////////////////////////	
		if(sw == 0)
			strcpy(temp_token,strtok(readStr,"* \n"));
		else
			strcpy(temp_token_2,strtok(readStr,"* \n"));	

		if (strcmp(temp_token, "url") == 0 && sw == 0)
		{
			strcpy(temp_token, strtok(NULL, " "));
			sw=1;
		}
		else if (strcmp(temp_token_2, "loading_time") == 0 && sw == 1)
		{
//cout << "curNode->url : " << curNode->url << endl;
			strcpy(temp_token_2, strtok(NULL, " "));

			while(curNode->edge[i] != NULL) {
//cout <<"2 curNode->edge[i]->url " << curNode->edge[i]->url << endl;
				if(strcmp(curNode->edge[i]->url, temp_token) == 0) {
					cout << "Hit " << hitCount+1 << endl;
					hitCount++;
					hitSw=1;
					i=0;
					break;
				}
//cout <<"3" << endl;
				i++;
				if(i==HIDDEN_SIZE) {
					i=0;
					break;
				}
			}
			if(hitSw == 1) {
				hitSw = 0;
				totalTime += atol(temp_token_2);
				preLoadTime += 60;	
			}
			else {
				totalTime += atol(temp_token_2);
				preLoadTime += atol(temp_token_2);
			}

			if(curNode->e_index < HIDDEN_SIZE) {
				lessEdgeNodeCount++;
				if(strcmp(temp_token, initialUrl) == 0) {
					m_reloadCount++;
				}
			}
//cout <<"4" << endl;
			s_g_head->set_temp_node(s_g_head->count(curNode, temp_token, atol(temp_token_2)));
//cout <<"5" << endl;
			totalCount++;
			curNode = s_g_head->get_temp_node();
			//curNode->fill_edge();	//{H} here
			sw=0;
			memset(temp_token, 0, 1200);
			memset(temp_token_2, 0, 1200);

			hitRatio = (float)hitCount/(float)totalCount;
			loadTime = ((float)totalTime-(float)preLoadTime)/(float)totalTime;
			result << s_g_head->a_index << ", " << totalCount << ", "<< hitCount << ", "<< totalTime << ", " << preLoadTime << ", " << hitRatio << ", " << loadTime << endl;
		}
		
		j++;
/////////////////////////////////////////////////////////////////////////////////////////////////////////
	}
	history.close();
	return;
}

int manage::comLog() {
	com.open("history/compare5_ksm1.csv");
	s_g_head = new Graph;
	Node* curNode = s_g_head->gethead();
	Node* preNode = NULL;
	int i = 0;
	int y=0;
	Node* temp_node = NULL;
	int tempkey=0;
	char readStr[1200] = { 0, };
	char* temp = NULL;
	char temp_token[1200]={0,};
	char temp_token_2[1200]={0,}; //{H}
	int sw = 0;
	int hitSw = 0;

	int ph[100]={0, }; 
	Node* buf[100];
	for(int k=0; k<101; k++){
		buf[k]=NULL;
	}
	j = 0;
	/*for(int m=99; m>=0; m--){
		com << "ph[" << m << "]" << ph[m];

		if(m != 0)
			com << ", ";
		else
			com << "\n";
	}*/

	com << "currentURL, 1.URL, 1.Count, 2.URL, 2.Count, 3.URL, 3.Count, 4.URL, 4.Count, 5.URL, 5.Count"<< endl; 

	while(!history.eof())
	{
		history.getline(readStr, 1200);
		if (strlen(readStr) == 0) continue;

		////////////////////////////////	
		if(sw == 0)
			strcpy(temp_token,strtok(readStr,"* \n"));
		else
			strcpy(temp_token_2,strtok(readStr,"* \n"));	

		if (strcmp(temp_token, "url") == 0 && sw == 0)
		{
			strcpy(temp_token, strtok(NULL, " "));
			sw=1;
		}
		else if (strcmp(temp_token_2, "loading_time") == 0 && sw == 1)
		{
			s_g_head->set_temp_node(s_g_head->count(curNode, temp_token, atol(temp_token_2)));
			curNode = s_g_head->get_temp_node();
			sw=0;
			memset(temp_token, 0, 1200);
			memset(temp_token_2, 0, 1200);
			
			while(buf[99-i]!=NULL) {
				i++;
			}
			if(i >= 100) {
				i = 99;
				for(int t=98; t>=0; t--) {
					buf[t] = buf [t+1];
					ph[t] = ph[t+1];
				}
				buf[99]=curNode;
				ph[99]= curNode->v_count;
			}
			else {
				buf[99-i] = curNode;
			}

			for(int g=98; g>=0; g--) {
				if(buf[g] == NULL)
					break;

				if(strcmp(curNode->url, buf[g]->url)==0) {
					ph[g]++;
					cout << "ph[" << g << "] : " << ph[g] << endl;
					cout << "buf[g] : " << buf[g]->url << endl;
				}			
			}
			/*for(int g2=98; g2>=0; g2--) {
				com << ph[g2];

				if(g2 != 0)
					com << ", ";
				else
					com << "\n";
			}*/
			int key=0;
			Node* keyNode=NULL;
			int c=0;
//cout<<"11 : " << buf[98]->url << " " << ph[98] << endl;
			for(int b=98; b>0; b--){
				if(buf[b]==NULL)
					break;

				key=ph[b];
				keyNode=buf[b];
				for(c=b+1; c<=99 && key>ph[c]; c++){
					ph[c-1] = ph[c];
					buf[c-1] = buf[c];
				}
				ph[c-1] = key;
				buf[c-1] = keyNode;
			}
			i=0;
			y++;
			j=1;
cout<<"00\n";
			c=99;
			int gb=0;
			cout << curNode->url << ", ";
			com << curNode->url << ", ";
			while(gb<5 && c>=0){
				if(buf[c]==NULL){
					com << endl;
					c--;
					break;
				}
//cout<<"11\n";
				for(int s=c+1; s<=99; s++){
					if(strcmp(buf[c]->url, buf[s]->url)==0){
						tempkey=1;
						break;
					}
				}
				if(tempkey!=1){	
					cout << buf[c]->url << ", " << ph[c];
					com << buf[c]->url << ", " << ph[c];
					if(gb==4) {
						cout << endl;
						com << endl;
					}
					else {
						cout << ", ";
						com << ", ";
					}
					gb++;
				}
				tempkey=0;
				c--;
//cout<<"44\n";
			}
		}
		
		j++;
/////////////////////////////////////////////////////////////////////////////////////////////////////////
	}
	com.close();
	
	return 0;
}

void manage::comHit2()
{
	Node* curNode = s_g_head->gethead();
	int i = 0;
	Node* temp_node = NULL;

	char readStr[1200] = { 0, };
	char* temp = NULL;
	char temp_token[1200]={0,};
	char temp_token_2[1200]={0,}; //{H}
	int sw = 0;
	int hitSw = 0;
	//int j = 0;

	////////////////////////////////////////////////////////////////////
	////			{H} Analysis of log			////
	////////////////////////////////////////////////////////////////////
	Node* store[1500];
	Node* sort[1500];
	int preNodeRank[1500]={1,};
	int sortRank[1500]={0,};
	int b,c,d,e;	//{H} the function paratmeter FOR()
	int a2=0;
	int aa, bb, cc, dd, ee;
	for(int m=0; m<=99; m++){
		store[m]=NULL;
		sort[m]=NULL;
	}

	Node* keyNode;
	ofstream mnb;
	mnb.open("history/kks3_1500_vcount.csv");

	mnb << "current URL, , 1. URL, , 1. count, , 2. URL, , 2. count, , 3. URL, , 3. count, , 4. URL, , 4. count, , 5. URL, , 5. count\n";
	////////////////////////////////////////////////////////////////////

	while(!history.eof())
	{
		a2=0;
		i=0;
		history.getline(readStr, 1200);
		if (strlen(readStr) == 0) continue;

		////////////////////////////////	
		if(sw == 0)
			strcpy(temp_token,strtok(readStr,"* \n"));
		else
			strcpy(temp_token_2,strtok(readStr,"* \n"));	

		if (strcmp(temp_token, "url") == 0 && sw == 0)
		{
			strcpy(temp_token, strtok(NULL, " "));
			sw=1;
		}
		else if (strcmp(temp_token_2, "loading_time") == 0 && sw == 1)
		{
			strcpy(temp_token_2, strtok(NULL, " "));

			while(curNode->edge[i] != NULL) {
				if(strcmp(curNode->edge[i]->url, temp_token) == 0) {
					//cout << "Hit " << hitCount+1 << endl;
					hitCount++;
					hitSw=1;
					i=0;
					break;
				}

				i++;
				if(i==5) {
					i=0;
					break;
				}
			}
			if(hitSw == 1) {
				hitSw = 0;
				totalTime += atol(temp_token_2);
				preLoadTime += 60;	
			}
			else {
				totalTime += atol(temp_token_2);
				preLoadTime += atol(temp_token_2);
			}

			if(curNode->e_index < 5) {
				lessEdgeNodeCount++;
				if(strcmp(temp_token, initialUrl) == 0) {
					m_reloadCount++;
				}
			}
			s_g_head->set_temp_node(s_g_head->count(curNode, temp_token, atol(temp_token_2)));
			totalCount++;
			curNode = s_g_head->get_temp_node();
			sw=0;
			memset(temp_token, 0, 1200);
			memset(temp_token_2, 0, 1200);

			////////////////////////////////////////////////////////////////////
			////			{H} Analysis of log			////
			////////////////////////////////////////////////////////////////////	
			while(store[a2]!=NULL){
				if(a2==1499){
					a2=1505;
					break;
				}
				a2++;
			}			
//cout << "a2 : " <<a2<<endl;
			if(a2<1505){
				if(a2==0) {
					store[a2] = curNode;
					a2=1;
				}
				/////{H} Update Ranks//////
				/*for(c=0; c<a2; c++){
					for(d=0; d<a2; d++){
						if(c!=d){
							if(strcmp(store[c]->url, store[d]->url)==0){
								preNodeRank[c]++;
							}
						}
					}
				}*/

				for(c=0; c<a2; c++){
					preNodeRank[c] = store[c]->v_count;
				}
//cout << "11"<<endl;
				e=0;
				if(store[e]!=NULL) {
					sort[e] = store[e];
					sortRank[e]= preNodeRank[e];
				}
				e++;
				int sortKey=0;
//cout << "22"<<endl;
				for(aa=0; aa<a2; aa++){
					for(int a1=0; a1<e; a1++){
						if(strcmp(store[aa]->url, sort[a1]->url)==0){
							sortKey=1;
						}
					}
					if(sortKey==0){
						sort[e]=store[aa];
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
				//////////////////////////////////
				//{H} HERE -- Write URLs in file//
				//////////////////////////////////
				mnb << a2 << "_" << curNode->url << ", , ";
				ee=0;
				while(sort[ee] != NULL){
					mnb << sort[ee]->url << ", , " << sortRank[ee];

					if(sort[ee+1]==NULL || ee==4){
						mnb << endl;
					}
					else{
						mnb << ", , ";
					}
//cout << "ee : " << ee <<endl;
					if(ee==4)
						break;

					ee++;
				}
//cout << "55"<<endl;		
				memcpy(&store[a2], &curNode, sizeof(Node*));
//cout << "66"<<endl;
				for(ee=0; ee<1500; ee++){
					preNodeRank[ee]=1;
					sortRank[ee]=0;
					memset(&sort[ee], NULL, sizeof(Node*));
				}
			}
			else{
//cout << "a2 : " << a2 << endl;
				a2=1500;
				/////{H} Update Ranks//////
				/*for(c=0; c<a2; c++){
					for(d=0; d<a2; d++){
						if(c!=d){
							if(strcmp(store[c]->url, store[d]->url)==0){
								preNodeRank[c]++;
							}
						}
					}
				}*/

				for(c=0; c<a2; c++){
					preNodeRank[c] = store[c]->v_count;
				}


				e=0;
				if(store[e]!=NULL) {
					sort[e] = store[e];
					sortRank[e]= preNodeRank[e];
				}
				e++;
				int sortKey=0;
				for(aa=0; aa<a2; aa++){
					for(int a1=0; a1<e; a1++){
						if(strcmp(store[aa]->url, sort[a1]->url)==0){
							sortKey=1;
						}
					}
					if(sortKey==0){
						sort[e]=store[aa];
						sortRank[e]= preNodeRank[aa];
						e++;
					}
					sortKey=0;
				}
//mnb << sort[0]->url << " " << sortRank[0] << " " << preNodeRank[0];
				//{H}insertionSort
				for(bb=1; bb<e; bb++){
					memcpy(&keyNode, &sort[bb], sizeof(Node*));
					sortKey = sortRank[bb];
					for(cc=bb-1; cc>=0 && sortKey>sortRank[cc]; cc--){
						memcpy(&sort[cc+1], &sort[cc], sizeof(Node*));
						sortRank[cc+1] = sortRank[cc];
					}
					memcpy(&sort[cc+1], &keyNode, sizeof(Node*));
					sortRank[cc+1] = sortKey;
					memset(&keyNode, NULL, sizeof(Node*));
				}

				sortKey=0;

				//////////////////////////////////
				//{H} HERE -- Write URLs in file//
				//////////////////////////////////
				mnb << curNode->url << ", , ";
				ee=0;
				while(sort[ee] != NULL){
					mnb << sort[ee]->url << ", , " << sortRank[ee];

					if(sort[ee+1]==NULL || ee==4){
						mnb << endl;
					}
					else{
						mnb << ", , ";
					}
					if(ee==4)
						break;

					ee++;
				}
			
				for(dd=0; dd < a2-1; dd++){
					//memcpy(&store[dd], &store[dd+1], sizeof(Node*));
					store[dd] = store[dd+1];
				}
				//memcpy(&store[a2-1], &curNode, sizeof(Node*));
				store[a2-1]=curNode;

				for(ee=0; ee<1500; ee++){
					preNodeRank[ee]=1;
					sortRank[ee]=0;
					memset(&sort[ee], NULL, sizeof(Node*));
				}
			}
			/////////////////////////////{H} END//z/////////////////////////////
			result << s_g_head->a_index << ", " << totalCount << ", "<< hitCount << ", "<< totalTime << ", " << preLoadTime << endl;
		}
		
		j++;
/////////////////////////////////////////////////////////////////////////////////////////////////////////
	}
	mnb.close();
	return;
}

void manage::comHit_comNode()
{
	Node* curNode = s_g_head->gethead();
	int i = 0;
	Node* temp_node = NULL;

	char readStr[1200] = { 0, };
	char* temp = NULL;
	char temp_token[1200]={0,};
	char temp_token_2[1200]={0,}; //{H}
	int sw = 0;
	int hitSw = 0;
	//int j = 0;

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
	for(int m=0; m<=99; m++){
		store[m]=NULL;
		sort[m]=NULL;
	}

	Node* keyNode;
	ofstream mnb;
	mnb.open("history/ksm1_node_vcount.csv");

	mnb << "current URL, , 1. URL, , 1. count, , 2. URL, , 2. count, , 3. URL, , 3. count, , 4. URL, , 4. count, , 5. URL, , 5. count\n";
	////////////////////////////////////////////////////////////////////

	while(!history.eof())
	{
		a2=0;
		i=0;
		history.getline(readStr, 1200);
		if (strlen(readStr) == 0) continue;

		////////////////////////////////	
		if(sw == 0)
			strcpy(temp_token,strtok(readStr,"* \n"));
		else
			strcpy(temp_token_2,strtok(readStr,"* \n"));	

		if (strcmp(temp_token, "url") == 0 && sw == 0)
		{
			strcpy(temp_token, strtok(NULL, " "));
			sw=1;
		}
		else if (strcmp(temp_token_2, "loading_time") == 0 && sw == 1)
		{
			strcpy(temp_token_2, strtok(NULL, " "));

			while(curNode->edge[i] != NULL) {
				if(strcmp(curNode->edge[i]->url, temp_token) == 0) {
					//cout << "Hit " << hitCount+1 << endl;
					hitCount++;
					hitSw=1;
					i=0;
					break;
				}

				i++;
				if(i==5) {
					i=0;
					break;
				}
			}
			if(hitSw == 1) {
				hitSw = 0;
				totalTime += atol(temp_token_2);
				preLoadTime += 60;	
			}
			else {
				totalTime += atol(temp_token_2);
				preLoadTime += atol(temp_token_2);
			}

			if(curNode->e_index < 5) {
				lessEdgeNodeCount++;
				if(strcmp(temp_token, initialUrl) == 0) {
					m_reloadCount++;
				}
			}
			s_g_head->set_temp_node(s_g_head->count(curNode, temp_token, atol(temp_token_2)));
			totalCount++;
			curNode = s_g_head->get_temp_node();
			sw=0;
			memset(temp_token, 0, 1200);
			memset(temp_token_2, 0, 1200);

			////////////////////////////////////////////////////////////////////
			////			{H} Analysis of log			////
			////////////////////////////////////////////////////////////////////	
			while(s_g_head->arr_url[a2]!=NULL){
				a2++;
			}			
//cout << "a2 : " <<a2<<endl;
			//if(a2<1499){
				for(c=0; c<a2; c++){
					preNodeRank[c] = s_g_head->arr_url[c]->v_count;
				}
//cout << "11"<<endl;
				e=0;
				if(s_g_head->arr_url[e]!=NULL) {
					sort[e] = s_g_head->arr_url[e];
					sortRank[e]= preNodeRank[e];
				}
				e++;
				int sortKey=0;
//cout << "22"<<endl;
				for(aa=0; aa<a2; aa++){
					for(int a1=0; a1<e; a1++){
						if(strcmp(s_g_head->arr_url[aa]->url, sort[a1]->url)==0){
							sortKey=1;
						}
					}
					if(sortKey==0){
						sort[e]=s_g_head->arr_url[aa];
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
				//////////////////////////////////
				//{H} HERE -- Write URLs in file//
				//////////////////////////////////
				mnb << a2 << "_" << curNode->url << ", , ";
				ee=0;
				while(sort[ee] != NULL){
					mnb << sort[ee]->url << ", , " << sortRank[ee];

					if(sort[ee+1]==NULL || ee==4){
						mnb << endl;
					}
					else{
						mnb << ", , ";
					}
//cout << "ee : " << ee <<endl;
					if(ee==4)
						break;

					ee++;
				}
//cout << "55"<<endl;		
				//memcpy(&store[a2], &curNode, sizeof(Node*));
//cout << "66"<<endl;
				for(ee=0; ee<100; ee++){
					preNodeRank[ee]=0;
					sortRank[ee]=0;
					memset(&sort[ee], NULL, sizeof(Node*));
				}
			//}
			/*else{
//cout << "a2 : " << a2 << endl;
				a2=1499;
				/////{H} Update Ranks//////
				for(c=0; c<a2; c++){
					preNodeRank[c] = s_g_head->arr_url[c]->v_count;
				}
//cout << "11"<<endl;
				e=0;
				if(store[e]!=NULL) {
					sort[e] = s_g_head->arr_url[e];
					sortRank[e]= preNodeRank[e];
				}
				e++;
				int sortKey=0;
//cout << "22"<<endl;
				for(aa=0; aa<a2; aa++){
					for(int a1=0; a1<e; a1++){
						if(strcmp(store[aa]->url, sort[a1]->url)==0){
							sortKey=1;
						}
					}
					if(sortKey==0){
						sort[e]=store[aa];
						sortRank[e]= preNodeRank[aa];
						e++;
					}
					sortKey=0;
				}
//mnb << sort[0]->url << " " << sortRank[0] << " " << preNodeRank[0];
				//{H}insertionSort
				for(bb=1; bb<e; bb++){
					memcpy(&keyNode, &sort[bb], sizeof(Node*));
					sortKey = sortRank[bb];
					for(cc=bb-1; cc>=0 && sortKey>sortRank[cc]; cc--){
						memcpy(&sort[cc+1], &sort[cc], sizeof(Node*));
						sortRank[cc+1] = sortRank[cc];
					}
					memcpy(&sort[cc+1], &keyNode, sizeof(Node*));
					sortRank[cc+1] = sortKey;
					memset(&keyNode, NULL, sizeof(Node*));
				}

				sortKey=0;

				//////////////////////////////////
				//{H} HERE -- Write URLs in file//
				//////////////////////////////////
				mnb << curNode->url << ", , ";
				ee=0;
				while(sort[ee] != NULL){
					mnb << sort[ee]->url << ", , " << sortRank[ee];

					if(sort[ee+1]==NULL || ee==4){
						mnb << endl;
					}
					else{
						mnb << ", , ";
					}
					if(ee==4)
						break;

					ee++;
				}
			
				for(dd=0; dd < a2-1; dd++){
					//memcpy(&store[dd], &store[dd+1], sizeof(Node*));
					store[dd] = store[dd+1];
				}
				//memcpy(&store[a2-1], &curNode, sizeof(Node*));
				store[a2-1]=curNode;

				for(ee=0; ee<100; ee++){
					preNodeRank[ee]=1;
					sortRank[ee]=0;
					memset(&sort[ee], NULL, sizeof(Node*));
				}
			}*/
			/////////////////////////////{H} END//z/////////////////////////////
			result << s_g_head->a_index << ", " << totalCount << ", "<< hitCount << ", "<< totalTime << ", " << preLoadTime << endl;
		}
		
		j++;
/////////////////////////////////////////////////////////////////////////////////////////////////////////
	}
	mnb.close();
	return;
}

