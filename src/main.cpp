#include <iostream>
#include <boost/graph/graphml.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/adjacency_list_io.hpp>
#include <boost/graph/named_graph.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS,
        boost::undirectedS> Graph;

int main(int argc, char *argv[])
{
    assert(argc >= 2);
    try
    {
        std::ifstream in;
        in.open(argv[1], std::ifstream::in);
        assert(in.is_open());

        Graph g;
        boost::dynamic_properties dp;
        boost::read_graphml(in, g, dp);

        boost::GraphPrinter<Graph, boost::no_property, boost::no_property> printer(g);
        std::cout << printer << std::endl;
    }
    catch (std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}
