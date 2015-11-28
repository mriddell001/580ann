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
    ind = 0;
}

void Node::Prime_hidden_layers(int input, int layers) {
    /** This first section primes the top  **/
    /** input layer for easy access during **/
    /** edge linking and weight setting.   **/

    DataNode *tmp = new DataNode();
    (*tmp).node_weight = 0.01;
    (*tmp).layer = layers;
    (*tmp).index = ind;
    data_vector.push_back(tmp);
    hidden_layers.push_back(ind);
    ind++;

    for (int i = 1; i < input; i++) {
        DataNode *others = new DataNode();
        (*tmp).node_weight = 0.01;
        (*tmp).layer = layers;
        (*tmp).index = ind;
        data_vector.push_back(tmp);
        ind++;
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
            (*tmp).edge_weight.push_back(0.5);
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
                (*tmp).edge_weight.push_back(0.5);
            }
        }
        else {i = 0;}
    }
}

void Node::Hidden_to_hidden() {
    for (int i = 0; i < data_vector.size(); i++) {
        /** This connects each hidden node to each node in the next layer. **/
        /** This loop quits when it's in the last layer, to minimize work. **/
        if (data_vector[i]->layer != hidden_layers.size()-1) {
            int next = data_vector[i]->layer + 1;
        //  For each node in the hidden layer.
            DataNode *current = data_vector[i];
        //  Take the start of the next layer.
            int start = hidden_layers[next];
        //  Stop when you reach the layer after.
            int past = hidden_layers[next+1];

        /** Return the first node in the next layer. Stops at the next layer. **/
            for (int j = start; j < past; j++) {
                (*current).input_edges.push_back(data_vector[j]);
                (*current).edge_weight.push_back(0.5);
            }
        }
        else {
            i = data_vector.size();
        }
    }
}

void Node::Calculate_layers() {
    // Preform activation from input layer.
    for (int i = 0; i < input_vector.size(); i++) {
        InputNode *tmp = input_vector[i];
        for (int j = 0; j < (*tmp).edge_weight.size(); j++) {
            DataNode *hidden_node = (*tmp).input_edges[j];
            (*hidden_node).node_weight = (*hidden_node).node_weight + (*tmp).original_value * (*tmp).edge_weight[j];
        }
    }
    // Calculate activation function.
    for (int i = 0; i < data_vector.size(); i++) {
        DataNode *tmp = data_vector[i];
        double weight = (*tmp).node_weight;
        weight = 1.0 / (1.0 + pow(e, (-weight)));
        (*tmp).node_weight = weight;

        // Preform activation across hidden layers.
        if (i < (*hidden_layers.end())) {
            for (int j = 0; j < (*tmp).edge_weight.size(); j++) {
                DataNode *hidden_node = (*tmp).input_edges[j];
                (*hidden_node).node_weight = (*hidden_node).node_weight + (*tmp).node_weight * (*tmp).edge_weight[j];
            }
        }
        // Preform activation to output layer.
        else {
            for (int j = 0; j < (*tmp).edge_weight.size(); j ++) {
                OutputNode *output_node = (*tmp).output_edges[j];
                (*output_node).original_value = (*output_node).original_value + (*tmp).node_weight * (*tmp).edge_weight[j];
            }
        }
    }

    for (int i = 0; i < output_vector.size(); i++) {
        OutputNode *tmp = output_vector[i];
        double weight = (*tmp).original_value;
        weight = 1.0 / (1.0 + pow(e, (-weight)));
        (*tmp).original_value = weight;
    }
}

void Node::Print_layers() {
    for (int i = 0; i < output_vector.size(); i++) {
        OutputNode *tmp = output_vector[i];
        double weight = (*tmp).original_value;
        cout << i << ": " << weight << endl;
    }
}
