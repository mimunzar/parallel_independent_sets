#ifndef SEQUENTIAL_INDEPENDENT_SET
#define SEQUENTIAL_INDEPENDENT_SET

// #define __DEBUG

// Name: seq_ind_sets.hpp
// Description: Sequentialy computes maximal independent set in given graph.
// Author: Milan Munzar, xmunza00@stud.fit.vutbr.cz

#include <iostream>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>

using namespace std;

enum { NOP = 0, R, N, S };

// Prints row in state matrix.
void printRow(vector<int> *row) {

	for ( vector<int>::iterator it = row->begin() ; it != row->end(); ++it )
		cout << "\t" << *it ;
	cout << endl;
}

// Returns adjacent vertices for given vertex.
vector<int> adjacentVertices(Graph g, int vertex) {

	vector<int> adjacency_list;
    typename boost::graph_traits<Graph>::adjacency_iterator ai;
    typename boost::graph_traits<Graph>::adjacency_iterator ai_end;

    for ( tie(ai, ai_end) = adjacent_vertices(vertex, g); ai != ai_end; ++ai )
        adjacency_list.push_back(*ai);

	return adjacency_list;
}

// 3. Tests R maximality.
bool Rmaximality(vector<int> *row) {

	for ( vector<int>::iterator it = row->begin() ; it != row->end(); ++it )
		if ( *it == N or *it == S )
			return false;
	
	return true;
}

// 4. Tests expandibility.
bool Rexpandibility(vector<int> *row) {

	for ( vector<int>::iterator it = row->begin() ; it != row->end(); ++it )
		if ( *it == N )
			return true;
	
	return false;
}

// Finds maximal independent sets.
// Algorithm taken from: Grafy a jejich aplikace (Jiri Demel).
vector<vector <int>> seqIndSets(Graph g){

	// 1. Initialization
	unsigned int n_vertices = boost::num_vertices(g);
	vector<vector <int>> matrix(1, vector<int>(n_vertices) );
	unsigned int k = 0;
	unsigned int row_index = k;
	vector<vector <int>> independent_sets;

	matrix[k].assign(n_vertices, N);
#ifdef __DEBUG
	cout << k << ": ";
	printRow(&matrix[k]);
#endif

	// 2. Expanding independent set
	unsigned int chosen_vertex = 0;
	unsigned int adjacent_vertex;
	vector<int> adjacency_list;
 	unsigned int it;

	for ( it = 0; it < n_vertices; it++ ) {
		if ( matrix[k][it] == N ) {	
			chosen_vertex = it;
			break;
		}
	}

	matrix.push_back(vector<int>(n_vertices));
	row_index += 1;
	
	matrix[k + 1] = matrix[k];
	matrix[k + 1][chosen_vertex] = R;		

	adjacency_list = adjacentVertices(g, chosen_vertex);
	while ( adjacency_list.empty() == false ) {
		adjacent_vertex = adjacency_list.back();
		adjacency_list.pop_back();
		matrix[k + 1][adjacent_vertex] = NOP;
	}

	k += 1;
#ifdef __DEBUG
	cout << k << ": ";
	printRow(&matrix[k]);
#endif

	bool reduce;
	bool intersect;

	while ( 1 ) {
		reduce = true;
		intersect = false;

		// 3. Testing R maximality
		if ( Rmaximality(&matrix[k]) == false ) {
			// 4. Testing expandibility
			if ( Rexpandibility(&matrix[k]) == true ) {
				// 5. Expandibility of maximal ind. set
				reduce = false;
				for ( it = 0; it < n_vertices; it++ ) {
					if ( matrix[k][it] == S ) {
						intersect = false;
						adjacency_list = adjacentVertices(g, it);
						while ( adjacency_list.empty() == false ){
							chosen_vertex = adjacency_list.back();
							adjacency_list.pop_back();
							if ( matrix[k][chosen_vertex] == N ) {
								intersect = true;
								break;
							}
						}
						if ( intersect == false ) {
							reduce = true;
							break;
						}
					}
				} // 5.		
			} // 4.
		} // 3.
		else {
			independent_sets.push_back(vector<int>(n_vertices));
			independent_sets.back() = matrix[k];
		}

		if ( reduce == false ) {
			// 2. Expanding independent set
			for ( it = 0; it < n_vertices; it++ ) {
				if ( matrix[k][it] == N ) {	
					chosen_vertex = it;
					break;
				}
			}

			if ( k == row_index ) {	
				matrix.push_back(vector<int>(n_vertices));
				row_index += 1;
			}
			matrix[k + 1] = matrix[k];
			matrix[k + 1][chosen_vertex] = R;		

			adjacency_list = adjacentVertices(g, chosen_vertex);
			while ( adjacency_list.empty() == false ) {
				adjacent_vertex = adjacency_list.back();
				adjacency_list.pop_back();
				matrix[k + 1][adjacent_vertex] = NOP;
			}

			k += 1;
		}
		else {
			// 6. Reducing item set
			if ( k == 0 )
				return independent_sets;
			for ( it = n_vertices - 1; it >= 0; it-- )
				if ( matrix[k][it] == R ) {
					chosen_vertex = it;
					break;
				}
			matrix[k - 1][chosen_vertex] = S;
			k -= 1;		
		}
#ifdef __DEBUG
	cout << k << ": ";
	printRow(&matrix[k]);
#endif
	} // while

} // seqIndSets

#endif

