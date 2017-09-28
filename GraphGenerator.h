    #include <iostream>
    #include <sstream>
    #include <string>
    #include <stack>
    #include "PMMLNode.h"
#include "tinyxml.h"

using namespace std;

class GraphGenerator{

	public:
	void populatePredicate(Node* node, TiXmlElement* pElement) ;
    pair<double, double> getScoreDistribution(TiXmlElement* pElement);
    Node* getNodeDataFromThisNode(TiXmlElement* pElement);
    Node* subGenerateGraph(TiXmlNode* pParent);
    Node* dump_to_stdout( TiXmlNode* pParent);
    Node* dump_to_stdout(const char* pFilename);
    void traverse(Node* rootNode);
    Node* getRootNode(const char* pFilename) ;
};