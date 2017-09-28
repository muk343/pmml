#include <iostream>
#include <map>
#include <vector>
#include <stack>
#include <string>
#include <sstream>
#include "PMMLNode.h"

using namespace std;


bool lessOrEqual(double value, double test_value) {
	return test_value <= value;
}

bool greaterThan(double value, double test_value) {
	return test_value > value;
}

typedef bool (*FnPtr)(double, double);


class Score {
	Node *resPMMLNode;

	int getFeatureIndex(string feature_name) {
		feature_name.erase(0,1);
		return stoi(feature_name);
	}

	bool isPredicateTrue(Node *node) {
		int feature_idx = getFeatureIndex(node -> predicate_feature);
		if(predicateMap[node->predicate_operator](node -> predicate_value, input_vec[feature_idx])) {
			return true;
		} 
		return false;
	}

	void getScore(Node *parent) {
		if(resPMMLNode != NULL) return;
		for(int i=0; !(parent -> childNodes.empty()) && i<parent -> childNodes.size(); i++) {
			Node *currPMMLNode = parent -> childNodes.at(i);
			if(isPredicateTrue(currPMMLNode)) {
				getScore(currPMMLNode);
			}
		}
		resPMMLNode = parent;
	}

public:
	vector<double> input_vec;
	std::map<std::string, FnPtr> predicateMap;
	
	Score(vector<double> input_vector) {
		input_vec = input_vector;
		create_predicate_map();
	}

	void create_predicate_map() {
		predicateMap["lessOrEqual"] = lessOrEqual;
		predicateMap["greaterThan"] = greaterThan;
	}

	vector<double> generateProb(Node* root) {
		vector<double> result;
		if(root == NULL) {
			result.resize(5,0.0);
			return result;
		}
		getScore(root);
		if(resPMMLNode == NULL) {
			resPMMLNode = root;
		}

		for(int i=0; i<resPMMLNode->score_distribution.size(); i++) {
			double prob = resPMMLNode->score_distribution[i]/resPMMLNode->node_record_count;
			result.push_back(prob);
		}

		return result;
	}


};


int main()
{
	vector<double> input_vec;
	for(int i=0; i<22; i++) {
		input_vec.push_back(i);
	}
	Score s(input_vec);

	//cout << s.predicateMap["lessOrEqual"](4, 100) << endl;
	

    

}