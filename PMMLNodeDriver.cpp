// Usage :
// g++ PMMLNodeDriver.cpp PMMLNode.cpp
// ./a.out

#include "PMMLNode.h"

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
