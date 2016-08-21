/*
(C) David J. Kalbfleisch 2016
You are welcome to use this code pursuant to the GNU General Public license.  See the file, LICENSE.

This code tests GraphSearch.h.
*/

#include <unordered_set>
#include <vector>

#include "cute.h"
#include "GraphSearch.h"
#include "GraphSearchTests.h"

//------------------------------------------------------------------------------------------------------

void SimpleProblem::actions(int state, std::vector<TestActions>& available_actions) const
{
	switch (state)
	{
	case 1:
		available_actions.push_back(TestActions::right);
		break;
	case 5:
		available_actions.push_back(TestActions::left);
		break;
	default:
		// For states 2, 3, and 4
		available_actions.push_back(TestActions::right);
		available_actions.push_back(TestActions::left);
	}
}

const int SimpleProblem::result(int state, TestActions action)
{
	switch (action)
	{
	case TestActions::right:
		return state + 1;
	case TestActions::left:
		return state - 1;
	default:
		throw "Unrecognized action.";
	}
}

void NoSolutionTest()
{
	SimpleProblem problem;
	std::vector<TestActions> the_solution;
	std::vector<int> the_path;

	ASSERT_THROWSM("Asking for a solution when a solution isn't available should raise an exception.",
			problem.solution(the_solution), const char*);
	ASSERT_THROWSM("Asking for a path when a solution isn't available should raise an exception.",
			problem.path(the_path), const char*);
}

void SimpleBreadthFirstSearchTest()
{
	SimpleProblem problem;
	std::unordered_set<int> goal_states({1, 4});
	std::vector<TestActions> the_solution;
	std::vector<int> the_path;

	// This search should find the path 5-4 with solution {left}.
	ASSERTM("No solution found.", problem.searchBreadthFirst(5, goal_states));
	problem.solution(the_solution);
	ASSERTM("Incorrect solution.", (the_solution == std::vector<TestActions>({ TestActions::left })));
	problem.path(the_path);
	ASSERTM("Incorrect path.", (the_path == std::vector<int>({5, 4})));

	// This search should find the path 3-4 with solution {right}.
	ASSERTM("No solution found.", problem.searchBreadthFirst(3, goal_states));
	problem.solution(the_solution);
	ASSERTM("Incorrect solution.", (the_solution == std::vector<TestActions>({ TestActions::right })));
	problem.path(the_path);
	ASSERTM("Incorrect path.", (the_path == std::vector<int>({3, 4})));

	// This search should find the path 4 with solution {}.
	ASSERTM("No solution found.", problem.searchBreadthFirst(4, goal_states));
	problem.solution(the_solution);
	ASSERTM("Incorrect solution.", (the_solution == std::vector<TestActions>()));
	problem.path(the_path);
	ASSERTM("Incorrect path.", (the_path == std::vector<int>({4})));
}

void SimpleDepthFirstSearchTest()
{
	SimpleProblem problem;
	std::unordered_set<int> goal_states({3, 5});
	std::vector<TestActions> the_solution;
	std::vector<int> the_path;

	// This search should find the path 1-2-3 with solution {right, right}.
	ASSERTM("No solution found.", problem.searchDepthFirst(1, goal_states));
	problem.solution(the_solution);
	ASSERTM("Incorrect solution.", (the_solution == std::vector<TestActions>({ TestActions::right, TestActions::right })));
	problem.path(the_path);
	ASSERTM("Incorrect path.", (the_path == std::vector<int>({1, 2, 3})));

	// This search should find the path 4-3 with solution {left}.
	ASSERTM("No solution found.", problem.searchDepthFirst(4, goal_states));
	problem.solution(the_solution);
	ASSERTM("Incorrect solution.", (the_solution == std::vector<TestActions>({ TestActions::left })));
	problem.path(the_path);
	ASSERTM("Incorrect path.", (the_path == std::vector<int>({4, 3})));

	// This search should find the path 3 with solution {}.
	ASSERTM("No solution found.", problem.searchDepthFirst(3, goal_states));
	problem.solution(the_solution);
	ASSERTM("Incorrect solution.", (the_solution == std::vector<TestActions>()));
	problem.path(the_path);
	ASSERTM("Incorrect path.", (the_path == std::vector<int>({3})));
}

//------------------------------------------------------------------------------------------------------

void SimpleCyclesProblem::actions(int state, std::vector<TestActions>& available_actions) const
{
	switch (state)
	{
	case 1:
		available_actions = std::vector<TestActions>({ TestActions::down, TestActions::right });
		break;
	case 2:
	case 6:
	case 9:
	case 11:
		available_actions = std::vector<TestActions>({ TestActions::left, TestActions::right });
		break;
	case 3:
	case 7:
		available_actions = std::vector<TestActions>({ TestActions::down, TestActions::left });
		break;
	case 4:
		available_actions = std::vector<TestActions>({ TestActions::down, TestActions::up });
		break;
	case 5:
	case 8:
		available_actions = std::vector<TestActions>({ TestActions::down, TestActions::up, TestActions::right });
		break;
	case 10:
		available_actions = std::vector<TestActions>({ TestActions::up, TestActions::left });
		break;
	default:
		throw "Unrecognized state.";
	}
}

const int SimpleCyclesProblem::result(int state, TestActions action)
{
	switch (state)
	{
	case 1:
		switch (action)
		{
		case TestActions::right:
			return 2;
		case TestActions::down:
			return 4;
		default:
			throw "Unrecognized action.";
		}
	case 2:
		switch (action)
		{
		case TestActions::left:
			return 1;
		case TestActions::right:
			return 3;
		default:
			throw "Unrecognized action.";
		}
	case 3:
		switch (action)
		{
		case TestActions::left:
			return 2;
		case TestActions::down:
			return 5;
		default:
			throw "Unrecognized action.";
		}
	case 4:
		switch (action)
		{
		case TestActions::up:
			return 1;
		case TestActions::down:
			return 8;
		default:
			throw "Unrecognized action.";
		}
	case 5:
		switch (action)
		{
		case TestActions::right:
			return 6;
		case TestActions::down:
			return 10;
		case TestActions::up:
			return 3;
		default:
			throw "Unrecognized action.";
		}
	case 6:
		switch (action)
		{
		case TestActions::left:
			return 5;
		case TestActions::right:
			return 7;
		default:
			throw "Unrecognized action.";
		}
	case 7:
		switch (action)
		{
		case TestActions::left:
			return 6;
		case TestActions::down:
			return 11;
		default:
			throw "Unrecognized action.";
		}
	case 8:
		switch (action)
		{
		case TestActions::right:
			return 9;
		case TestActions::down:
			return 11;
		case TestActions::up:
			return 4;
		default:
			throw "Unrecognized action.";
		}
	case 9:
		switch (action)
		{
		case TestActions::right:
			return 10;
		case TestActions::left:
			return 8;
		default:
			throw "Unrecognized action.";
		}
	case 10:
		switch (action)
		{
		case TestActions::left:
			return 9;
		case TestActions::up:
			return 5;
		default:
			throw "Unrecognized action.";
		}
	case 11:
		switch (action)
		{
		case TestActions::right:
			return 7;
		case TestActions::left:
			return 8;
		default:
			throw "Unrecognized action.";
		}
	default:
		throw "Unrecognized state.";
	}
}

void ExhaustiveSearchTest()
{
	SimpleCyclesProblem problem;
	ASSERTM("Searching without a goal should not cause errors.", !problem.searchDepthFirst(9, std::unordered_set<int>()));
}

void SimpleCyclesDepthFirstSearchTest()
{
	SimpleCyclesProblem problem;
	std::unordered_set<int> goal_states1({ 1, 6 });
	std::unordered_set<int> goal_states2({ 3 });
	std::vector<TestActions> the_solution;
	std::vector<int> the_path;

	// This search should find the path 9-10-5-6 with solution {right, up, right}.
	ASSERTM("No solution found.", problem.searchDepthFirst(9, goal_states1));
	problem.solution(the_solution);
	ASSERTM("Incorrect solution.", (the_solution == std::vector<TestActions>({ TestActions::right, TestActions::up, TestActions::right })));
	problem.path(the_path);
	ASSERTM("Incorrect path.", (the_path == std::vector<int>({ 9, 10, 5, 6 })));

	// This search should find the path 3-2-1 with solution {left, left}.
	ASSERTM("No solution found.", problem.searchDepthFirst(3, goal_states1));
	problem.solution(the_solution);
	ASSERTM("Incorrect solution.", (the_solution == std::vector<TestActions>({ TestActions::left, TestActions::left })));
	problem.path(the_path);
	ASSERTM("Incorrect path.", (the_path == std::vector<int>({ 3, 2, 1 })));

	// This search should find the path 11-7-6 with solution {right, left}.
	ASSERTM("No solution found.", problem.searchDepthFirst(11, goal_states1));
	problem.solution(the_solution);
	ASSERTM("Incorrect solution.", (the_solution == std::vector<TestActions>({ TestActions::right, TestActions::left })));
	problem.path(the_path);
	ASSERTM("Incorrect path.", (the_path == std::vector<int>({ 11, 7, 6 })));

	// This search should find the path 8-9-10-5-3 with solution {right, right, up, up}.  The only goal state is 3 for this
	// test.  The agent should search by moving 8-9-10-5-6-7-11.  At 11, it should realize that proceeding to 8 would be a
	// cycle, and it should instead expand 3, which is the goal.
	ASSERTM("No solution found.", problem.searchDepthFirst(8, goal_states2));
	problem.solution(the_solution);
	ASSERTM("Incorrect solution.", (the_solution == std::vector<TestActions>({ TestActions::right, TestActions::right, TestActions::up, TestActions::up })));
	problem.path(the_path);
	ASSERTM("Incorrect path.", (the_path == std::vector<int>({ 8, 9, 10, 5, 3 })));
}

//------------------------------------------------------------------------------------------------------

void SimpleStructProblem::actions(SimpleStruct state, std::vector<TestActions>& available_actions) const
{
	// In every state, "left" and "right" are available because the graph is a single loop.
	available_actions = std::vector<TestActions>({ TestActions::left, TestActions::right });
}

const SimpleStruct SimpleStructProblem::result(SimpleStruct state, TestActions action)
{
	int next_x = state.x;

	if (action == TestActions::left)
	{
		if (next_x == 1)
		{
			next_x = 5;
		}
		else
		{
			--next_x;
		}
	}
	else if (action == TestActions::right)
	{
		if (next_x == 5)
		{
			next_x = 1;
		}
		else
		{
			++next_x;
		}
	}
	else
	{
		throw "Unrecognized action.";
	}
	return SimpleStruct{ next_x };
}

void SimpleStructBreadthFirstSearchTest()
{
	SimpleStructProblem problem;
	std::unordered_set<SimpleStruct> goal_states({ SimpleStruct{2} });
	std::vector<TestActions> the_solution;
	std::vector<SimpleStruct> the_path;

	// This search should find the path (4)-(3)-(2) with solution {left, left}.
	ASSERTM("No solution found.", problem.searchBreadthFirst(SimpleStruct{ 4 }, goal_states));
	problem.solution(the_solution);
	ASSERTM("Incorrect solution.", (the_solution == std::vector<TestActions>({ TestActions::left, TestActions::left })));
	problem.path(the_path);
	ASSERTM("Incorrect path.", (the_path == std::vector<SimpleStruct>({SimpleStruct{ 4 }, SimpleStruct{ 3 }, SimpleStruct{ 2 }})));
}

void SimpleStructDepthFirstSearchTest()
{
	SimpleStructProblem problem;
	std::unordered_set<SimpleStruct> goal_states({ SimpleStruct{2} });
	std::vector<TestActions> the_solution;
	std::vector<SimpleStruct> the_path;

	// This search should find the path (5)-(1)-(2) with solution {right, right}.
	ASSERTM("No solution found.", problem.searchDepthFirst(SimpleStruct{ 5 }, goal_states));
	problem.solution(the_solution);
	ASSERTM("Incorrect solution.", (the_solution == std::vector<TestActions>({ TestActions::right, TestActions::right })));
	problem.path(the_path);
	ASSERTM("Incorrect path.", (the_path == std::vector<SimpleStruct>({SimpleStruct{ 5 }, SimpleStruct{ 1 }, SimpleStruct{ 2 }})));
}
