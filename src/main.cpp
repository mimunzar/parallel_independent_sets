
// Name: main.cpp
// Description: Computes maximal independent set in given undirected graph.
// Authors: Jakub Sochor, xsocho06@stud.fit.vutbr.cz 
//		    Milan Munzar, xmunza00@stud.fit.vutbr.cz

#include <iostream>
#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include <boost/graph/graphml.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/adjacency_list_io.hpp>
#include <boost/graph/named_graph.hpp>
typedef boost::adjacency_list<boost::vecS, boost::vecS,
        boost::undirectedS> Graph;

#include "seq_ind_sets.hpp"

void printIndependentSets(vector<vector <int>> *independent_sets) {

	vector<vector <int>>::iterator it_outer;
 	vector<int>::iterator it_inner;
	for ( it_outer = independent_sets->begin() ; it_outer != independent_sets->end();
	 ++it_outer ) {
		for ( it_inner = (* it_outer).begin() ; it_inner != (* it_outer).end();
	 		++it_inner ) {
			std::cout << *it_inner << " ";
		}
		std::cout << std::endl;
	}
}

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
				std::cout << "Finds maximum independent set." << std::endl;
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
				// paralelni
				std::cout << "Paralelni" << std::endl;
			}
			else {
				independent_sets = seqIndSets(g);
				printIndependentSets(&independent_sets);
			}
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
		std::cout << "Finds maximum independent set." << std::endl;
		std::cout << desc << "\n";
	}

	return 0;

}

