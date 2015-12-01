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
    (*tmp).actual_value = input;
    (*tmp).node_weight = 0.01;
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
    (*tmp).node_weight = 0.01;
    output_vector.push_back(tmp);
}

void Node::Connect_layers() {
    Input_to_first_hidden();
    Hidden_to_output();
    Hidden_to_hidden();
}

void Node::Prime_class_label(double input) {
    class_label.push_back(input);
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

    progress = 0.0;
}

void Node::Calculate_layers() {
    // Preform activation from input layer.
    for (int i = 0; i < input_vector.size(); i++) {
        InputNode *tmp = input_vector[i];
        for (int j = 0; j < (*tmp).edge_weight.size(); j++) {
            DataNode *hidden_node = (*tmp).input_edges[j];
            (*hidden_node).node_weight = (*hidden_node).node_weight + (*tmp).actual_value * (*tmp).edge_weight[j];
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
                (*output_node).node_weight = (*output_node).node_weight + (*tmp).node_weight * (*tmp).edge_weight[j];
            }
        }
    }

    for (int i = 0; i < output_vector.size(); i++) {
        OutputNode *tmp = output_vector[i];
        double weight = (*tmp).node_weight;
        weight = 1.0 / (1.0 + pow(e, (-weight)));
        (*tmp).node_weight = weight;
    }
}

void Node::Print_layers() {
    for (int i = 0; i < output_vector.size(); i++) {
        OutputNode *tmp = output_vector[i];
        double weight = (*tmp).node_weight;
        cout << i << ": " << weight << endl;
        cout << endl;
    }
}

void Node::Euclidean_distance() {
    double sum = 0.0;
    for (int i = 0; i < output_vector.size(); i++) {
        OutputNode *tmp = output_vector[i];
        double class_value = class_label[i];
        double output_weight = (*tmp).node_weight;
        sum = sum + pow((class_value - output_weight), 2.0);
    }
    double euclidean_d = sqrt(sum);
    cout << "Euclidean Distance: " << euclidean_d << "\t\tProgress: " <<  euclidean_d - progress << endl;
    progress = euclidean_d;
}

void Node::Calculate_error() {
    /** This calculates the error for every node. **/
    for (int i = 0; i < output_vector.size(); i++) {
        OutputNode *tmp = output_vector[i];
        double class_value = class_label[i];
        double output_weight = (*tmp).node_weight;
        (*tmp).error_value = output_weight * (1 - output_weight) * (class_value - output_weight);
    }

    for (int i = (data_vector.size()-1); i > -1; i--) {
        DataNode *tmp = data_vector[i];
        double weight = (*tmp).node_weight;
        double error_sum = 0.0;

        if (i < (*hidden_layers.end())) {
            for (int j = 0; j < (*tmp).edge_weight.size(); j++) {
                DataNode *hidden_node = (*tmp).input_edges[j];
                error_sum += (*tmp).edge_weight[j] * (*hidden_node).delta_error;
            }
        }
        else {
            for (int j = 0; j < (*tmp).edge_weight.size(); j++) {
                OutputNode *output_node = (*tmp).output_edges[j];
                error_sum += (*tmp).edge_weight[j] * (*output_node).error_value;
            }
        }
        (*tmp).delta_error = weight * (1 - weight) * error_sum;
    }

    for (int i = (input_vector.size()-1); i > -1; i--) {
        InputNode *tmp = input_vector[i];
        double input_weight = (*tmp).node_weight;
        double error_sum = 0.0;

        for (int j = 0; j < (*tmp).edge_weight.size(); j++) {
            DataNode *data_node = (*tmp).input_edges[j];
            error_sum += (*tmp).edge_weight[j] * (*data_node).delta_error;
        }
        (*tmp).input_error = input_weight * (1 - input_weight) * error_sum;
    }
}

void Node::Propagate_error() {
    double alpha = 0.005;

    /** Through the input layer. **/
    for (int i = 0; i < input_vector.size(); i++) {
        InputNode *tmp = input_vector[i];
        double in_err = (*tmp).input_error;
        double dummy = (*tmp).node_weight;
        double actual = (*tmp).actual_value;

        /** This calculates the node weight for weight(0,i) **/
        //dummy = dummy + alpha * dummy * in_err;
        (*tmp).node_weight = dummy;                         // Is this even needed?

        /** This calculates the edge weight for edge(i,j) **/
        for (int j = 0; j < (*tmp).edge_weight.size(); j++) {
            DataNode *data = (*tmp).input_edges[j];
            double edge_prop = (*tmp).edge_weight[j];
            double da_err = (*data).delta_error;

            (*tmp).edge_weight[j] = edge_prop + alpha * actual * da_err;
        }
    }
    /** Through the hidden layer(s). **/
    for (int i = 0; i < data_vector.size(); i++) {
        DataNode *tmp = data_vector[i];
        double hi_err = (*tmp).delta_error;
        double weight = (*tmp).node_weight;

        weight = weight + alpha * weight * hi_err;
        (*tmp).node_weight = weight;
        if (i < (*hidden_layers.end())) {
            for (int j = 0; j < (*tmp).edge_weight.size(); j++) {
                DataNode *data = (*tmp).input_edges[j];
                double edge_prop = (*tmp).edge_weight[j];
                double da_err = (*data).delta_error;
                (*tmp).edge_weight[j] = edge_prop + alpha * weight * hi_err;
            }
        }
        else {
            for (int j = 0; j < (*tmp).edge_weight.size(); j++) {
                OutputNode *out = (*tmp).output_edges[j];
                double edge_prop = (*tmp).edge_weight[j];
                //double out_err = (*tmp).error_value;
                //(*tmp).edge_weight[j] = edge_prop + alpha * weight * out_err;
            }
        }
    }
}
