#include<iostream>
#include<vector>
#include<stack>

using namespace std;

class Node {

public:
	int score;
    vector<double> score_distribution;
    double node_record_count;
    string predicate_feature;
    string predicate_operator;
    double predicate_value;
    vector<Node*> childNodes;

	Node(int score, vector<double> score_distribution, double node_record_count, string predicate_feature, string predicate_operator, double predicate_value) {
		this->score = score;
		this->score_distribution = score_distribution;
		this->node_record_count = node_record_count;
		this->predicate_feature = predicate_feature;
		this->predicate_operator = predicate_operator;
		this->predicate_value = predicate_value;
	}

	void addChildNode(Node *child) {
		childNodes.push_back(child);
	}
};


int main()
{
	cout << "Creating a node" << endl;

	static const double arr[] = {1,2,3,4};
	vector<double> s_distribution (arr, arr + sizeof(arr) / sizeof(arr[0]) );
	Node *newNode = new Node(1, s_distribution, 1.0, "f1", "equal", 2.0);

	Node *childNode = new Node(2, s_distribution, 1.0, "f2", "equal", 3.0);
	newNode->addChildNode(childNode);

	cout << newNode->score << endl;
	cout << newNode->childNodes[0]->score << endl;
}
