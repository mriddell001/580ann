#include <fstream>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <vector>
#include "Node.h"
#include "bmp.h"

using namespace std;

int main(int argc, char *argv[])
{
    Node node;

    double data = 0.0;
    int dat = 0;
    int layer = 0;
    string tmp;

    /** Input Source **/
    ifstream in_file(argv[1], ios::in);
    while(in_file >> data) {
        node.Prime_input_vector(data);
    }
    /** Completed Input **/

    /** Prime Hidden Layers **/
    ifstream h_file(argv[2], ios::in);
    while (h_file >> dat) {
        node.Prime_hidden_layers(dat, layer);
        layer++;
    }
    /** Completed Hidden Layers **/

    /** Output Source**/
    ifstream o_file(argv[3], ios::in);
    while (o_file >> data) {
        node.Prime_output_vector(data);
    }
    /** Completed Output **/

    node.Connect_layers();

    return 0;
}
