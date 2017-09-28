#include "PMMLNode.h"

Node::Node(int score, vector<double> score_distribution, double node_record_count, string predicate_feature, string predicate_operator, double predicate_value) {
	this->score = score;
	this->score_distribution = score_distribution;
	this->node_record_count = node_record_count;
	this->predicate_feature = predicate_feature;
	this->predicate_operator = predicate_operator;
	this->predicate_value = predicate_value;
}

void Node::addChildNode(Node *child) {
	childNodes.push_back(child);
}

Node::Node() {}
