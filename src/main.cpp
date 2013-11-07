
// Name: main.cpp
// Description: Computes maximal independent set in given undirected graph.
// Authors: Jakub Sochor, xsocho06@stud.fit.vutbr.cz 
//		    Milan Munzar, xmunza00@stud.fit.vutbr.cz

#include <iostream>
#include <boost/program_options.hpp>
#include <chrono>
#include "seq_ind_sets.hpp"
#include "par_ind_sets.hpp"

namespace po = boost::program_options;
using namespace std;

void printIndependentSets(const vector<vector <int>>& independent_sets)
{
    std::cout << "Maximum independent sets: \n";
	for ( auto it_outer = independent_sets.begin() ; it_outer != independent_sets.end();
	 ++it_outer )
    {
		for (unsigned v = 0; v < it_outer->size(); v++)
        {
			if (it_outer->at(v))
            {
                std::cout << " " << v;
            }
		}
		std::cout << std::endl;
	}
}

#define SPEED_MEASURE(f)\
do {\
    auto start = chrono::high_resolution_clock::now();\
    f;\
    auto end = chrono::high_resolution_clock::now();\
    double runningTime = chrono::duration<double>(end - start).count();\
    std::cerr << "Running time [s]: " << runningTime <<std::endl;\
} while(false)

int main(int argc, char *argv[])
{
	std::string input_file;
    bool p_given = false;
	vector<vector <int>> independent_sets;
	
	po::options_description desc("Allowed options");
    desc.add_options()
        ("help", "produce help message")
        ("sequential,s", "[default] uses sequential algorithm")
        ("parallel,p", po::bool_switch(&p_given), "uses parallel algorithm")
		("input-file", po::value(&input_file), "Input file containing graph")
	;
	po::positional_options_description p;
	p.add("input-file", -1);

	if ( argc <= 3 && argc > 1 ) {
		try {
			po::variables_map vm;        
		    po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
		    po::notify(vm);

		    if (vm.count("help")) {
				std::cout << "Finds maximum independent sets." << std::endl;
		        std::cout << desc << "\n";
		        return 0;
		    }
			if ( input_file.empty() == true ) {
		        std::cout << desc << "\n";
		        return 0;
			}

        	std::ifstream in;
        	in.open(input_file, std::ifstream::in);
        	assert(in.is_open());

        	Graph g;
        	boost::dynamic_properties dp;
        	boost::read_graphml(in, g, dp);

			if ( p_given == true ) {
                SPEED_MEASURE(independent_sets = parIndSets(g));
			}
			else {
                SPEED_MEASURE(independent_sets = seqIndSets(g));
			}
            printIndependentSets(independent_sets); // TODO add switch to disable independent sets printing
		}
		catch (std::exception& e)
		{
		    std::cerr << "Error: " << e.what() << std::endl;
		}
		catch(...)
		{
			std::cerr << "Exception of unknown type!" << std::endl;
		}
	}
	else
	{
		std::cout << "Finds maximum independent sets." << std::endl;
		std::cout << desc << "\n";
	}

	return 0;

}

