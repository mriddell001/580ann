#ifndef NODE_H

#include <string>
#include <vector>

using namespace std;

class Node {
public:
    void Prime_input_vector(double);
    void Prime_output_vector(double);
    void Prime_hidden_layers(int, int);

private:
    class DataNode {
    public:
        int layer;
        double node_weight;
        vector<DataNode*> input_edges;
        vector<OutputNode*> output_edges;

        DataNode();
        ~DataNode();
    };
    vector<DataNode*> data_vector;

    class InputNode {
    public:
        double original_value;
        vector<DataNode*> input_edges;

        InputNode();
        ~InputNode();
    };
    vector<InputNode*> input_vector;

    class OutputNode {
    public:
        double original_value;
        OutputNode();
        ~OutputNode();
    };
    vector<OutputNode*> output_vector;

    vector<DataNode*> hidden_layers;
};

#endif // NODE_H
