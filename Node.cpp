#include <fstream>
#include <iomanip>
#include <iostream>
#include <math.h>
#include <string>
#include <queue>
#include <vector>
#include "Node.h"

void Node::Prime_input_vector(double input) {
    InputNode *tmp = new InputNode();
    (*tmp).original_value = input;
    input_vector.push_back(tmp);
}

void Node::Prime_output_vector(double input) {
    OutputNode *tmp = new OutputNode();
    (*tmp).original_value = input;
    output_vector.push_back(tmp);
}

void Node::Prime_hidden_layers(int input, int layers) {
    DataNode *tmp = new DataNode();
    (*tmp).node_weight = 1.0;
    (*tmp).layer = layers;
    output_vector.push_back(tmp);

    /** For each added node, connect to all nodes in the previous layer. **/
}
