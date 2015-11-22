#include <fstream>
#include <iomanip>
#include <iostream>
#include <math.h>
#include <string>
#include <queue>
#include <vector>
#include "Node.h"

using namespace std;

void Node::Prime_input_vector(double input) {
    InputNode *tmp = new InputNode();
    (*tmp).original_value = input;
    input_vector.push_back(tmp);
}

void Node::Prime_hidden_layers(int input, int layers) {
    /** This first section primes the top  **/
    /** input layer for easy access during **/
    /** edge linking and weight setting.   **/

    DataNode *tmp = new DataNode();
    (*tmp).node_weight = 1.0;
    (*tmp).layer = layers;
    data_vector.push_back(tmp);
    hidden_layers.push_back(tmp);

    for (int i = 1; i < input; i++) {
        DataNode *others = new DataNode();
        (*tmp).node_weight = 1.0;
        (*tmp).layer = layers;
        data_vector.push_back(tmp);
    }
}

void Node::Prime_output_vector(double input) {
    OutputNode *tmp = new OutputNode();
    (*tmp).original_value = input;
    output_vector.push_back(tmp);
}

void Node::Connect_layers() {
    Input_to_first_hidden();
    Hidden_to_output();
    Hidden_to_hidden();
}

/** This assumes there is at least one hidden layer to connect to. **/
void Node::Input_to_first_hidden() {
    for (vector<InputNode*>::iterator it = input_vector.begin(); it != input_vector.end(); ++it) {
        InputNode *tmp = *it;
        vector<DataNode*>::iterator itt = data_vector.begin();
        while ( (*itt)->layer == 0) {
            DataNode *edge = *itt;
            tmp->input_edges.push_back(edge);
            ++itt;
        }
    }
}

/** This connects the last hidden nodes to the nodes in the output layer. **/
void Node::Hidden_to_output() {
    for (int i = data_vector.size() -1; i > 0; i--) {
        if (data_vector[i]->layer == hidden_layers.size()-1) {
            DataNode *tmp = data_vector[i];
            for (int j = 0; j < output_vector.size(); j++) {
                OutputNode *out = output_vector[i];
                (*tmp).output_edges.push_back(out);
            }
        }
        else {
            i = 0;
        }
    }
}

void Node::Hidden_to_hidden() {
    for (int i = 0; i < data_vector.size(); i++) {
        /** This connects each hidden node to each node in the next layer. **/
        /** This loop quits when it's in the last layer, to minimize work. **/
        if (data_vector[i]->layer != hidden_layers.size()-1) {
            int next = data_vector[i]->layer + 1;
            DataNode *current = hidden_layers[i];
            DataNode *tmp = hidden_layers[next];
            DataNode *past = hidden_layers[next+1];
        /** Return the first node in the next layer. Stops at the next layer. **/
            for (vector<DataNode>::iterator it = tmp; it != past; ++it) {
                current.input_edges.push_back((*it));
            }
        }
        else {
            i = data_vector.size();
        }
    }
}
