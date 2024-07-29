#include "bench_parser.h"
#include "dijkstra.h"
#include <iostream>
#include <unordered_map>
#include <stdexcept>


//to check if the no of command line arguments provide to right 
int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: ./iscas <bench file> <input gate> <output gate>\n";
        return 1;
    }


    //extracting the command line arguments 
    std::string bench_file = argv[1];
    std::string input_gate_name = argv[2];
    std::string output_gate_name = argv[3];
    std::unordered_map<std::string, int> gate_mapping;


    // parasing the bench file and build graph and uopdate the graph mapping 
    try {
        Graph graph = parse_bench_file(bench_file, gate_mapping);

	// to check whether the inout and output graph is present is the parase graph 
        if (gate_mapping.find(input_gate_name) == gate_mapping.end()) {
            throw std::runtime_error("Signal " + input_gate_name + " not found in file " + bench_file);
        }
        if (gate_mapping.find(output_gate_name) == gate_mapping.end()) {
            throw std::runtime_error("Signal " + output_gate_name + " not found in file " + bench_file);
        }


	// from the gate mapping we can find the input and output gates 
        int input_index = gate_mapping[input_gate_name];
        int output_index = gate_mapping[output_gate_name];


        std::vector<int> path = dijkstra(graph, input_index, output_index); //dijkstra's algorithm 


	// to check if there is any valid path 
        if (path.empty() || path.front() != input_index || path.back() != output_index) {
            throw std::runtime_error("No path found between " + input_gate_name + " and " + output_gate_name);
        }


	// if found, print the name along with the shortest path 
        for (int index : path) {
            for (const auto& gate : gate_mapping) {
                if (gate.second == index) {
                    std::cout << gate.first << ",";
                    break;
                }
            }
        }
        std::cout << "\n";
    } catch (const std::runtime_error& e) {
        std::cerr << e.what() << "\n";
        return 1;//if error code 
    } // to handle runtime errors abc rint error message 

    return 0;//if success 
}

