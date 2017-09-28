#include <iostream>
#include <map>
#include <vector>
#include <stack>
#include <string>
#include <sstream>
#include "GraphGenerator.h"

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
		for(int i=0; i< parent -> childNodes.size(); i++) {
			Node *currPMMLNode = parent -> childNodes.at(i);
			if(isPredicateTrue(currPMMLNode)) {
				//cout << "predicate true " <<currPMMLNode-> node_record_count << endl;
				getScore(currPMMLNode);
			}
		}
		if(resPMMLNode == NULL) {
			resPMMLNode = parent;
		}
		//cout << "resPMML node record count " << resPMMLNode->node_record_count << endl;
	}

public:
	vector<double> input_vec;
	std::map<std::string, FnPtr> predicateMap;
	
	Score(vector<double> input_vector) {
		input_vec = input_vector;
		create_predicate_map();
		resPMMLNode = NULL;
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
		
		for(int i=0; i<5; i++) {
			double prob = (resPMMLNode->score_distribution[i])/(resPMMLNode->node_record_count);
			result.push_back(prob);
		}

		return result;
	}


};

int main()
{

	GraphGenerator graphGenerator;
	Node* rootNode = graphGenerator.getRootNode("pmmlModel.xml");	

	string line;
	while (getline(cin, line)) {
	  	std::istringstream iss(line);
	  	vector<double> input_vec;
	  	std::string token;
    	while(std::getline(iss, token, '\t')) {
        	input_vec.push_back(stod(token));
        }

        Score s(input_vec);
		
		vector<double> res = s.generateProb(rootNode);
		int size = res.size();
		cout.precision(12);
		for(int i=0; i<size-1; i++) {
			cout << res[i] << ",";
		}
		cout << res[size-1] << "\n";
	}

}