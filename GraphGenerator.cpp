#ifdef TIXML_USE_STL
    #include <iostream>
    #include <sstream>
    #include <string>
    #include <stack>
    using namespace std;
#else
    #include <stdio.h>
#endif

#if defined( WIN32 ) && defined( TUNE )
    #include <crtdbg.h>
    _CrtMemState startMemState;
    _CrtMemState endMemState;
#endif

#include "tinyxml.h"
#include "GraphGenerator.h"

// ----------------------------------------------------------------------
// STDOUT dump and indenting utility functions
// ----------------------------------------------------------------------
//const unsigned int NUM_INDENTS_PER_SPACE=2;

/*
const char * getIndent( unsigned int numIndents )
{
    static const char * pINDENT="                                      + ";
    static const unsigned int LENGTH=strlen( pINDENT );
    unsigned int n=numIndents*NUM_INDENTS_PER_SPACE;
    if ( n > LENGTH ) n = LENGTH;

    return &pINDENT[ LENGTH-n ];
}

// same as getIndent but no "+" at the end
const char * getIndentAlt( unsigned int numIndents )
{
    static const char * pINDENT="                                        ";
    static const unsigned int LENGTH=strlen( pINDENT );
    unsigned int n=numIndents*NUM_INDENTS_PER_SPACE;
    if ( n > LENGTH ) n = LENGTH;

    return &pINDENT[ LENGTH-n ];
}


int dump_attribs_to_stdout(TiXmlElement* pElement, unsigned int indent)
{
    if ( !pElement ) return 0;

    TiXmlAttribute* pAttrib=pElement->FirstAttribute();
    int i=0;
    int ival;
    double dval;
    const char* pIndent=getIndent(indent);
    printf("\n");
    while (pAttrib)
    {
        printf( "%s%s: value=[%s]", pIndent, pAttrib->Name(), pAttrib->Value());

        if (pAttrib->QueryIntValue(&ival)==TIXML_SUCCESS)    printf( " int=%d", ival);
        if (pAttrib->QueryDoubleValue(&dval)==TIXML_SUCCESS) printf( " d=%1.1f", dval);
        printf( "\n" );
        i++;
        pAttrib=pAttrib->Next();
    }
    return i;    
}
*/

    void GraphGenerator::populatePredicate(Node* node, TiXmlElement* pElement) 
    {
        //Just for doubleCheck
        string nodeTag = "SimplePredicate";
        string trueTag = "True";
        if(!pElement || (nodeTag.compare(pElement->Value()) != 0 && trueTag.compare(pElement->Value()) !=0 )) 
        {
            cout<<"Expected predicate but didn't get it"<<endl;
            exit(-1);
            //nodeToReturn = getNodeDataFromThisNode
            //Add to the vector here
        }

        if (nodeTag.compare(pElement->Value()) == 0) {
            string field = pElement->Attribute("field");
            string currOperator = pElement->Attribute("operator");
            string value = pElement->Attribute("value");

            node->predicate_feature = field;
            node->predicate_operator = currOperator;
            node->predicate_value = stod(value);
            //cout<<"The loaded predicate values are field: "<<field<<" "<<" operator: "<<currOperator<<" value:  "<<value;    
        } else {
            node->predicate_feature = "true";
            node->predicate_operator = "true";
            node->predicate_value = 0.0;
        }

    }

    pair<double, double> GraphGenerator::getScoreDistribution(TiXmlElement* pElement) 
    {
        TiXmlAttribute* pAttribFirst=pElement->FirstAttribute();
        TiXmlAttribute* pAttribLast=pElement->LastAttribute();

        double score;
        double recordCount;

        if (pAttribFirst->QueryDoubleValue(&score)!=TIXML_SUCCESS) 
        {
         cout<<"Problem extracting the score value from the node";   
         exit(-1);
        } 
        if (pAttribLast->QueryDoubleValue(&recordCount)!=TIXML_SUCCESS) 
        {
         cout<<"Problem extracting the score value from the node";   
         exit(-1);
        } 

        pair<double, double> scoreDistribution = make_pair(score, recordCount);
        return scoreDistribution;
    }

    Node* GraphGenerator::getNodeDataFromThisNode(TiXmlElement* pElement) 
    {

        Node* node = new Node();
        //Just for doubleCheck
        string nodeTag = "Node";
        if(!pElement || nodeTag.compare(pElement->Value()) != 0) 
        {
            cout<<"Node sent is not an actual node";
            exit(-1);
        } 

        TiXmlAttribute* pAttribFirst=pElement->FirstAttribute();
        TiXmlAttribute* pAttribLast=pElement->LastAttribute();

        double score;
        double recordCount;

        if (pAttribFirst->QueryDoubleValue(&score)!=TIXML_SUCCESS) 
        {
         cout<<"Problem extracting the score value from the node";   
         exit(-1);
        } else
        {
            node->score = score;
        } 
        if (pAttribLast->QueryDoubleValue(&recordCount)!=TIXML_SUCCESS) 
        {
         cout<<"Problem extracting the score value from the node";   
         exit(-1);
        } else
        {
            node->node_record_count = recordCount;
        } 

        //Now let's add info about predicate
        TiXmlElement* pPredicate = pElement->FirstChild()->ToElement();
        populatePredicate(node, pPredicate);

        vector<double> scoreDistributions;
        scoreDistributions.resize(5, 0.0);
        string scoreDistributionTag = "ScoreDistribution";
        TiXmlNode* pChild;
        for ( pChild = pElement->FirstChild(); pChild != 0; pChild = pChild->NextSibling()) 
        {
            if (scoreDistributionTag.compare(pChild->Value()) == 0) {
                    pair<double, double> p1 = getScoreDistribution(pChild->ToElement());
                    double score = p1.first;
                    double value = p1.second;

                    int index = (int)score;
                    scoreDistributions[index] = value;
            }
            
        }

        node->score_distribution = scoreDistributions;

        return node;
    }

    Node* GraphGenerator::subGenerateGraph(TiXmlNode* pParent)
    {
        //if ( !pParent ) return;

        TiXmlNode* pChild;
        TiXmlText* pText;
        int t = pParent->Type();
        int num;
         
        Node* rootNode = getNodeDataFromThisNode(pParent->ToElement()); 

        string nodeTag = "Node";
        //if(pParent->FirstChild)
        for ( pChild = pParent->FirstChild(); pChild != 0; pChild = pChild->NextSibling()) 
        {
            if(nodeTag.compare(pChild->Value()) == 0) {
                rootNode->addChildNode(subGenerateGraph(pChild));
            }
            
        }

        return rootNode;
    }



    //returns the root of the tree.
    Node* GraphGenerator::dump_to_stdout( TiXmlNode* pParent)
    {
        if ( !pParent ) return NULL;

        TiXmlNode* pChild;
        TiXmlText* pText;
        int t = pParent->Type();
        //printf( "%s", getIndent(indent));
        int num;
    /*
        switch ( t )
        {
        case TiXmlNode::TINYXML_DOCUMENT:
            printf( "Document" );
            break;

        case TiXmlNode::TINYXML_ELEMENT:
            printf( "Element [%s]", pParent->Value() );
            num=dump_attribs_to_stdout(pParent->ToElement(), indent+1);
            switch(num)
            {
                case 0:  printf( " (No attributes)"); break;
                case 1:  printf( "%s1 attribute", getIndentAlt(indent)); break;
                default: printf( "%s%d attributes", getIndentAlt(indent), num); break;
            }
            break;

        case TiXmlNode::TINYXML_COMMENT:
            printf( "Comment: [%s]", pParent->Value());
            break;

        case TiXmlNode::TINYXML_UNKNOWN:
            printf( "Unknown" );
            break;

        case TiXmlNode::TINYXML_TEXT:
            pText = pParent->ToText();
            printf( "Text: [%s]", pText->Value() );
            break;

        case TiXmlNode::TINYXML_DECLARATION:
            printf( "Declaration" );
            break;
        default:
            break;
        }
        
        */
        
        string node = "Node";
        if(node.compare(pParent->Value()) == 0) 
        {
            //getNodeDataFromThisNode(pParent);
            Node* rootNode = getNodeDataFromThisNode(pParent->ToElement()); 
            //nodeToReturn = getNodeDataFromThisNode
            //Add to the vector here
            for ( pChild = pParent->FirstChild(); pChild != 0; pChild = pChild->NextSibling()) 
            {
                
                if(node.compare(pChild->Value()) == 0) {
                    rootNode->addChildNode(subGenerateGraph(pChild));
                }
                
            }

            return rootNode;
        } else {

            Node* deepInsertedRootToReturn = NULL;
            for ( pChild = pParent->FirstChild(); pChild != 0; pChild = pChild->NextSibling()) 
            {

                Node* tempReturnValue = dump_to_stdout(pChild);
                if (tempReturnValue != NULL)
                {
                    deepInsertedRootToReturn = tempReturnValue;
                }
                
            }

            return deepInsertedRootToReturn;    
        }

        
    }


    Node* GraphGenerator::dump_to_stdout(const char* pFilename)
    {
        TiXmlDocument doc(pFilename);
        bool loadOkay = doc.LoadFile();
        if (loadOkay)
        {
            return dump_to_stdout( &doc ); // defined later in the tutorial
        }
        else
        {
            printf("Failed to load file \"%s\"\n", pFilename);
            return NULL;
        }
    }

    // void GraphGenerator::traverse(Node* rootNode) {

    //     if (rootNode == NULL)
    //         return;

    //     stack<Node *> node_stack;
    //     Node *curr = rootNode;
        
    //     while(!node_stack.empty() || curr != NULL) {
            
    //         if(curr) {
    //             node_stack.push(curr);
    //             if(!curr -> childNodes.empty()) {
    //                 curr = curr -> childNodes[0];    
    //             } else {
    //                 curr = NULL;
    //             }
                
    //         } else {
    //             Node *temp = node_stack.top();
    //             cout<<"Score is: "<<temp->score<<" recordCount is: "<<temp->node_record_count<<endl;

    //             node_stack.pop();
    //             if(!curr -> childNodes.empty() && curr->childNodes.size() >1 ) {
    //                 curr = temp -> childNodes[1];
                
    //              } else {
    //                  curr = NULL;
    //              }
    //         }
    //     }
    // }

    void GraphGenerator::traverse(Node* rootNode) {
        
        stack<Node *> node_stack;
        Node *curr = rootNode;

        node_stack.push(curr);

        while(!node_stack.empty()) {

            Node* temp = node_stack.top();
            node_stack.pop();
            cout<<"Score is: "<<temp->score<<" recordCount is: "<<temp->node_record_count<<endl;
            if (temp->childNodes.size() == 1) {
                cout<<"This has only one child";
                exit(-1);
            }

            if (temp->childNodes.size() == 2) {
                node_stack.push(temp->childNodes[0]);
                node_stack.push(temp->childNodes[1]);
            }
        }

    }


    // int main(int argc, char* argv[])
    // {
        
    //     Node* rootNode = NULL;
        

    //     for (int i=1; i<argc; i++)
    //     {
    //         rootNode = dump_to_stdout(argv[i]);
    //     }

    //     if (rootNode != NULL) {
    //         cout<<"Root node is not null"<<endl;
    //     }

    //     cout<<"In the innder depths: "<<rootNode->childNodes[0]->childNodes[0]->childNodes[0]->node_record_count<<endl;
        
    //     traverse(rootNode);
        
    //     return 0;
    // }

    Node* GraphGenerator::getRootNode(const char* pFilename)
    {
        
        Node* rootNode = NULL;
        

            rootNode = dump_to_stdout(pFilename);
        
        // if (rootNode != NULL) {
        //     cout<<"Root node is not null"<<endl;
        // }

        //cout<<"In the innder depths: "<<rootNode->childNodes[0]->childNodes[0]->childNodes[0]->node_record_count<<endl;
        
        // traverse(rootNode);
     
        return rootNode;
    }




