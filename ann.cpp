#include <fstream>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <vector>
#include "Node.h"
//#include "bmp.h"

using namespace std;

int main(int argc, char *argv[])
{
    Node node;

    string tmp;
    double data = 0.0;
    int dat = 0;
    int layer = 0;

    /** Input Source **/
    //  Expects a text file with input data in the form:
    //  0.9 : Pixel is on.
    //  0.1 : Pixel is off.

    ifstream in_file(argv[1], ios::in);
    while (in_file >> data) {
        node.Prime_input_vector(data);
    }
    /** Completed Input **/

    /** Prime Hidden Layers **/
    //  Expects a text file with integers representing
    //  the number of nodes in each hidden layer.

    ifstream h_file(argv[2], ios::in);
    while (h_file >> dat) {
        node.Prime_hidden_layers(dat, layer);
        layer++;
    }
    /** Completed Hidden Layers **/

    /** Output Source**/
    //  Expects a text file with output node layer data
    //  including size of output and initial weight.

    ifstream o_file(argv[3], ios::in);
    while (o_file >> data) {
        node.Prime_output_vector(data);
    }
    /** Completed Output **/

    ifstream c_file(argv[4], ios::in);
    while (c_file >> data) {
        node.Prime_class_label(data);
    }

    node.Connect_layers();

    for (int i = 0; i < 10000; i++) {
        node.Calculate_layers();
        //node.Print_layers();
        node.Euclidean_distance();
        node.Calculate_error();
        node.Propagate_error();
    }

    return 0;
}
