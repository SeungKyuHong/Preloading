#include "boost/graph/graphviz.hpp"
#include "boost/graph/adjacency_list.hpp"



struct Vertex{
	char* label;
	char* url;
	int count;
};

struct Edge{
	char* label;
	int count;
};

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, Vertex ,Edge> boost_graph;

using namespace std;
using namespace boost;
