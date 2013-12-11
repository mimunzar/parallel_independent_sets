// Name: par_ind_sets.cpp
// Description: Parallely computes maximal independent set in given graph.
// Author: Jakub Sochor, xsocoh06@stud.fit.vutbr.cz
#include "par_ind_sets.hpp"
#include <thread>
#include <atomic>
#include <condition_variable>

using namespace std;

namespace
{
    typedef typename boost::graph_traits<Graph>::adjacency_iterator AdjIterator;

    mutex IndependentSetsMutex;
    atomic_int RunningCount;
    condition_variable CommunicationChannel;
    mutex CommunicationChannelMutex;



    enum {NoSet = 0, RSet = 1, NSet, SSet};

    #define VAR_DUMP(var) { std::cerr << std::boolalpha << #var << " = " << var << std::endl; }
    void PrintRow(const std::vector<int>& set)
    {
        for(const auto v: set)
        {
            char c;
            if (v == RSet)
            {
                c = 'R';
            }
            else if (v == NSet)
            {
                c = 'N';
            }
            else if (v == SSet)
            {
                c = 'S';
            }
            else
            {
                c = '-';
            }
            std::cout << c;
        }
        std::cout << std::endl;
    }

    /**
    * Item 3 in algorithm
    */
    bool TestMaximality(const std::vector<int>& set)
    {
        for (const auto i: set)
        {
            if (i == NSet || i == SSet)
            {
                return false;
            }
        }
        return true;
    }

    /**
    * Item 4 in algorithm
    */
    bool TestExpansionPossible(const std::vector<int>& set)
    {
        for (const auto i: set)
        {
            if (i == NSet)
            {
                return true;
            }
        }
        return false;
    }


    /**
    * Test from item 5 in algorithm
    */
    bool EmptyAdjSSetIntersectionWithNSet(const Graph& g, const std::vector<int>& set)
    {
        for (int i = 0; i < set.size(); i++)
        {
            if (set[i] == SSet)
            {
                AdjIterator adjIt, adjEnd;
                bool intersectionFound = false;
                for (boost::tie(adjIt, adjEnd) = boost::adjacent_vertices(i,g); adjIt != adjEnd; ++adjIt)
                {
                    if (set[*adjIt] == NSet)
                    {
                        intersectionFound = true;
                        break;
                    }
                }
                if (!intersectionFound)
                {
                    return true;
                }
            }
        }
        return false;
    }


    void RunAlgorithm(int firstVertexSelect, const Graph& g, std::vector<int> initial,
                      std::vector<std::vector <int>>& independentSets)
    {
        std::vector<std::vector<int>> matrix;
        std::vector<int> independent;
        int useVertex = firstVertexSelect;
        bool reduce;

        while (true)
        {
            if (useVertex != -1)
            {
                // step 2
                if (matrix.empty())
                {
                    matrix.push_back(initial);
                }
                else
                {
                    matrix.push_back(matrix.back());
                }
            }
            vector<int>& last = matrix.back();
            if (useVertex != -1)
            {
                independent.push_back(useVertex);
                last[useVertex] = RSet;
                AdjIterator adjIt, adjEnd;
                for (boost::tie(adjIt, adjEnd) = boost::adjacent_vertices(useVertex,g); adjIt != adjEnd; ++adjIt)
                {
                    last[*adjIt] = NoSet;
                }
            }
            reduce = false;
            // step 3
            if (useVertex != -1 && TestMaximality(last))
            {
                std::vector<int> indSet = GetIndependentSet(last);
                IndependentSetsMutex.lock();
                independentSets.push_back(indSet);
                IndependentSetsMutex.unlock();
                reduce = true;
            }
            else if (!TestExpansionPossible(last)) // step 4
            {
                reduce = true;
            }
            else if (EmptyAdjSSetIntersectionWithNSet(g, last)) // step 5
            {
                reduce = true;
            }

            if (reduce)
            {
                if (matrix.size() == 1)
                {
                    RunningCount--;
                    CommunicationChannel.notify_one();
                    return;
                }
                int x = independent.back();
                independent.pop_back();
                matrix.pop_back();
                matrix.back()[x] = SSet;
                useVertex = -1;
            }
            else
            {
                useVertex = -1;
                for (int i = 0; i < last.size(); i++)
                {
                    if (last[i] == NSet)
                    {
                        useVertex = i;
                        break;
                    }
                }
            }
        }
    }
}

std::vector<std::vector <int>> parIndSets(const Graph& g)
{
    int maxThreads = static_cast<int>(std::thread::hardware_concurrency());
    maxThreads = max(2, maxThreads);
    RunningCount = 0;
    std::vector<std::vector<int>> independentSets;
    std::vector<int> initial;
    initial.resize(boost::num_vertices(g), NSet);

    vector<thread> threads;

    for (int i = 0; i < initial.size(); ++i)
    {
        unique_lock<mutex> lck(CommunicationChannelMutex);
        while (RunningCount > maxThreads)
        {
            CommunicationChannel.wait(lck);
        }
        RunningCount++;

        try
        {
            threads.push_back(std::thread(RunAlgorithm, i, std::ref(g), initial, std::ref(independentSets)));
        }
        catch (std::system_error& e)
        {
            std::cerr << "Error when creating thread:" << e.what() << std::endl;
        }
        catch (...)
        {
            std::cerr << "Unknown error when creating thread." << std::endl;
        }

        threads.back().detach();
        initial[i] = SSet;
        if(EmptyAdjSSetIntersectionWithNSet(g, initial))
        {
            break;
        }
    }

    while (RunningCount > 0)
    {
        unique_lock<mutex> lck(CommunicationChannelMutex);
        CommunicationChannel.wait(lck);
    }

    return independentSets;
}


std::vector<std::vector <int>> seq_parIndSets(const Graph& g)
{
    std::vector<std::vector<int>> independentSets;
    std::vector<int> initial;
    initial.resize(boost::num_vertices(g), NSet);


    for (int i = 0; i < initial.size(); ++i)
    {
        RunAlgorithm(i, g, initial, independentSets);
        initial[i] = SSet;
        if(EmptyAdjSSetIntersectionWithNSet(g, initial))
        {
            break;
        }
    }

    return independentSets;
}
