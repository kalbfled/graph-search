/*
(C) David J. Kalbfleisch 2016
You are welcome to use this code pursuant to the GNU General Public license.  See the file, LICENSE.

This code tests GraphSearch.h.
*/

#pragma once

#include <functional>      // std::hash
#include "GraphSearch.h"
#include "FrontierTests.h" // SimpleStruct

enum class TestActions { start_state, left, right, up, down };

/*
This class defines a simple problem with a graph like: 1-2-3-4-5 (five integer states connected by single edges).
It is "simple" because the states are a native type (integers) rather than structures, and there are no cycles.
*/
class SimpleProblem : public graphsearch::Problem<int, TestActions>
{
	void actions(int state, std::vector<TestActions>& available_actions) const override;
	const int result(int state, TestActions action) override;
};

/*
This class is similar to SimpleProblem, but it represents a graph with cycles.

                              1 - 2 - 3
							  |       |
							  4       5 - 6 - 7
							  |       |       |
							  8 - 9 - 10      |
							  |               |
							  --------11------|
*/
class SimpleCyclesProblem : public graphsearch::Problem<int, TestActions>
{
	void actions(int state, std::vector<TestActions>& available_actions) const override;
	const int result(int state, TestActions action) override;
};

/*
Test the graph search problem when nodes are structures rather than simple types.  Use the SimpleStruct defined in
FrontierTests.h for this purpose.  Define a hasher to work with the default of the Problem class template's third variable.

https://stackoverflow.com/questions/8157937/how-to-specialize-stdhashkeyoperator-for-user-defined-type-in-unordered
*/
namespace std
{
	template<>
	struct hash<SimpleStruct>
	{
		size_t operator() (const SimpleStruct& ss) const
		{
			return std::hash<int>()(ss.x);
		}
	};
} // End of namespace std.

/*
This class defines a problem with a graph of structures: (1)-(2)-(3)-(4)-(5).
The rightmost state cycles back to the leftmost state.
*/
class SimpleStructProblem : public graphsearch::Problem<SimpleStruct, TestActions>
{
	void actions(SimpleStruct state, std::vector<TestActions>& available_actions) const override;
	const SimpleStruct result(SimpleStruct state, TestActions action) override;
};

// Test function prototypes:
void NoSolutionTest();
void SimpleBreadthFirstSearchTest();
void SimpleDepthFirstSearchTest();

void ExhaustiveSearchTest();
void SimpleCyclesDepthFirstSearchTest();

void SimpleStructBreadthFirstSearchTest();
void SimpleStructDepthFirstSearchTest();
