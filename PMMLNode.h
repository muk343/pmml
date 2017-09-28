#include<iostream>
#include<vector>

using namespace std;

class Node{
	public:
	double score;
    vector<double> score_distribution;
    double node_record_count;
    string predicate_feature;
    string predicate_operator;
    double predicate_value;
    vector<Node*> childNodes;

	Node(int score, vector<double> score_distribution, double node_record_count, string predicate_feature, string predicate_operator, double predicate_value);
	Node();
    void addChildNode(Node *child);
};