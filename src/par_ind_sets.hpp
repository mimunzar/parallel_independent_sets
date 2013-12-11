// Name: par_ind_sets.hpp
// Description: Parallely computes maximal independent set in given graph.
// Author: Jakub Sochor, xsocoh06@stud.fit.vutbr.cz
#pragma once

#include "common.hpp"

/**
* Parallel version
*/
std::vector<std::vector <int>> parIndSets(const Graph& g);

/**
* Sequence version implemented in the same way as parIndSets
*/
std::vector<std::vector <int>> seq_parIndSets(const Graph& g);
