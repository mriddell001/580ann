#ifndef NODE_H
#define NODE_H
#define e 2.71828182845904523536

#include <string>
#include <vector>

using namespace std;

class Node {
public:
    //   Sets initial ANN for calibration.
    void Prime_input_vector(double);
    void Prime_output_vector(double);
    void Prime_hidden_layers(int, int);

    //   Connects edges in ANN.
    void Connect_layers();
    void Input_to_first_hidden();
    void Hidden_to_hidden();
    void Hidden_to_output();

    //   Calculates activation function across ANN.
    void Calculate_layers();
    void Print_layers();

private:

    class OutputNode {
    public:
        double original_value;
    };
    vector<OutputNode*> output_vector;

    class DataNode {
    public:
        int index;
        int layer;
        double node_weight;
        vector<DataNode*> input_edges;
        vector<OutputNode*> output_edges;
        vector<double> edge_weight;
    };
    vector<DataNode*> data_vector;

    class InputNode {
    public:
        double original_value;
        vector<DataNode*> input_edges;
        vector<double> edge_weight;

    };
    vector<InputNode*> input_vector;

    vector<int> hidden_layers;
    int ind;
};

#endif // NODE_H
