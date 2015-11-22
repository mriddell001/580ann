#ifndef NODE_H
#define NODE_H

#include <string>
#include <vector>

using namespace std;

class Node {
public:
    void Prime_input_vector(double);
    void Prime_output_vector(double);
    void Prime_hidden_layers(int, int);
    void Connect_layers();
    void Input_to_first_hidden();
    void Hidden_to_hidden();
    void Hidden_to_output();

private:

    class OutputNode {
    public:
        double original_value;
    };
    vector<OutputNode*> output_vector;

    class DataNode {
    public:
        int layer;
        double node_weight;
        vector<DataNode*> input_edges;
        vector<OutputNode*> output_edges;
    };
    vector<DataNode*> data_vector;

    class InputNode {
    public:
        double original_value;
        vector<DataNode*> input_edges;

    };
    vector<InputNode*> input_vector;

    vector<DataNode*> hidden_layers;
};

#endif // NODE_H
