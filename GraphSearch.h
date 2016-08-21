/*
(C) David J. Kalbfleisch 2016
You are welcome to use this code pursuant to the GNU General Public license.  See the file, LICENSE.

This code implements an abstract base class for graph searching using algorithms presented in, "AI: A Modern Approach,"
by Stuart Russell and Peter Norvig.  The code makes use of C++1x features, including the standard template library.

A good reference for understanding the algorithms:
http://www.cs.cornell.edu/courses/CS2112/2012sp/lectures/lec24/lec24-12sp.html
*/

#pragma once

#include <algorithm>     // std::reverse
#include <functional>    // std::hash
#include <memory>        // std::unique_ptr
#include <queue>
#include <stack>
#include <unordered_set>
#include <vector>
#include "Frontier.h"

namespace graphsearch
{
	/*
	This enumerated class is used below in the Problem class template.  Any other enumerated type used with the Problem
	class template should also have a member named "start_state" because that value is hard-coded in the Problem class.
	TODO - Can I avoid this hard-coded value?
	*/
	enum class DefaultActions { start_state };
												 
	/*
	This is an abstract base class representing a graph search problem.  Subclass it.  At a minimum, you must implement
	the methods "actions" and "result."

	This implementation assumes that states are unique, although they can be reached by different sequences of actions.
	Additionally, path costs are not considered.  It assumes that actions are deterministic.  Taking a given action
	from a given state must yield one successor state that is not the same state.  It also assumes that all paths have
	equal cost, and it does not use heuristics to estimate the best path.
	
	TODO - Address these limitations.  The book deals with all of them.

	The type used for the template variable "StateType," which is likely to be a structure, must implement the equality
	(==) operator to work with the unordered_set container type.  It must also have an appropriate hashing class,
	which is the third template type variable, and it must implement the assignment (=) operator.
	*/
	template<typename StateType, typename ActionsType = DefaultActions, typename StateHashType = std::hash<StateType>>
	class Problem
	{
		typedef std::unordered_set<StateType, StateHashType> StateSet;
		typedef std::vector<ActionsType> SolutionVector;
		typedef std::vector<StateType> PathVector;

		/*
		A node includes a state plus additional information needed to describe the transition from some initial state
		to some goal state, which are parameters for a search method (below).  Graph search methods create instances
		of Node as necessary.
		*/
		struct Node
		{
			StateType state;
			int parent_index;    // The index into the "Problem::nodes" private member vector
			ActionsType action;

			Node() = default;

			Node(StateType the_state, int the_parent_index, ActionsType the_action)
				: state{the_state}, parent_index{the_parent_index}, action{the_action}
			{
			}
		};

		// Searches populate these private members, which are used the generation solution and path vectors.
		bool solution_found{false};  // Did the last search find a solution?
		std::vector<Node> nodes;

		/* Populate a vector with the actions that can be executed from the given state. */
		virtual void actions(StateType state, std::vector<ActionsType>& available_actions) const = 0;

		/*
		Return the state reached by taking a given action in a given state.  This implementation assumes that
		actions are deterministic (i.e. that the return type is a single value rather than a container).
		TODO - Support non-deterministic actions.
		*/
		virtual const StateType result(StateType state, ActionsType action) = 0;

		/*
		Return a vector of Node instances reachable from a given state.  The vector can be empty.  This
		implementation assumes a deterministic outcome; there is only one successor for a given action.
		TODO - Support non-deterministic actions.
		*/
		void expand(StateType state, int parent_index, std::vector<Node>& successors)
		{
			std::vector<ActionsType> available_actions;
			actions(state, available_actions);  // Populates available_actions.
			successors.clear();
			for (ActionsType the_action : available_actions)
			{
				successors.emplace_back(result(state, the_action), parent_index, the_action);
			}
		}

	public:
		/*
		The solution is the sequence of actions an agent must take to progress from the start state
		to a goal state.  Return the solution to the most recent search as a vector of actions.

		// TODO - For non-deterministic actions, the solution is a contingency plan.
		*/
		void solution(SolutionVector& the_solution)
		{
			if (!solution_found)
			{
				throw "You asked for a solution, but no solution was found.";  // TODO - Throw a standard exception?
			}

			the_solution.clear();  // Remove any previous solutions.

			// The last node added to "nodes" contains a goal state.
			int index = (int)nodes.size() - 1;

			while (nodes[index].action != ActionsType::start_state)
			{
				the_solution.emplace_back(nodes[index].action);
				index = nodes[index].parent_index;
			}
			std::reverse(the_solution.begin(), the_solution.end());  // Re-orders in place.
		}

		/*
		The path is the sequence of states through which an agent must progress from the start state
		to a goal state.  Return the path of the most recent search as a vector of states.
		*/
		void path(PathVector& the_path)
		{
			if (!solution_found)
			{
				throw "You asked for a path, but no path was found.";  // TODO - Throw a standard exception?
			}

			the_path.clear();  // Remove the path for any previous solutions.

			// The last node added to "nodes" contains a goal state.
			int index = (int)nodes.size() - 1;

			while (index > 0)
			{
				the_path.emplace_back(nodes[index].state);
				index = nodes[index].parent_index;
			}
			the_path.emplace_back(nodes[0].state);           // Add the start state.
			std::reverse(the_path.begin(), the_path.end());  // Re-orders in place.
		}

		/*
		Perform a graph search using a Frontier instance passed as a pointer.  Return True if a solution to the
		problem is found.  Otherwise, return False.  Upon success/true, the private member "solution" is populated,
		and the "path" or "solution" method can be called without raising an exception.

		To search to exhaustion--all available states have been expanded--set goal_states to an empty set.  You
		could use this to traverse all states while computing side effects of some sort.
		*/
		bool search(StateType initial_state, const StateSet& goal_states, Frontier<Node>* frontier)
		{
			Node current_node;
			std::vector<Node> children;
			int current_index;
			StateSet explored;             // Contains states already expanded; don't revisit them.
			std::vector<Node> successors;  // For a given state, these are the states that can be reached with the available actions.

			// Clear remnants of prior searches.
			nodes.clear();
			solution_found = false;

			// Push a node for the problem's initial state onto the frontier, which is a container for unexplored nodes.
			frontier->add(Node{initial_state, 0, ActionsType::start_state});
			explored.insert(initial_state);

			// Expand nodes until the frontier is empty or until a goal state is found (whichever is sooner).
			while (!frontier->isEmpty())
			{
				current_node = frontier->next();           // References the next node on the frontier.
				nodes.emplace_back(current_node);          // Remember the current node.
				current_index = (int)nodes.size() - 1;     // The index of the current node in Problem::nodes

				if (goal_states.count(current_node.state) == 1)
				{
					// Found a goal state.
					solution_found = true;
					return true;
				}

				// The current node is not a goal.  Find its successors/children, and remove it from the frontier.
				expand(current_node.state, current_index, successors);
				frontier->pop();  // Pops the current node, and returns void.  Do this before pushing successors.

				// Push unexplored successors onto the frontier.
				for (const Node& successor : successors)
				{
					if (explored.count(successor.state) == 0)
					{
						// This successor is unexplored.
						frontier->add(successor);
						explored.insert(current_node.state);
					}
				}
				successors.clear();  // Prepare for next iteration.
			}

			// The frontier is empty, and we didn't reach a goal node.
			return false;
		}

		/* Perform a standard depth-first search.  This is the general search using a stack as the frontier. */
		bool searchDepthFirst(StateType initial_state, const StateSet& goal_states)
		{
			std::unique_ptr<DepthFirstFrontier<Node>> frontier{new DepthFirstFrontier<Node>};
			return search(initial_state, goal_states, frontier.get());
		}

		/* Perform a standard breadth-first search.  This is the general search using a queue as the frontier. */
		bool searchBreadthFirst(StateType initial_state, const StateSet& goal_states)
		{
			std::unique_ptr<BreadthFirstFrontier<Node>> frontier{new BreadthFirstFrontier<Node>};
			return search(initial_state, goal_states, frontier.get());
		}

		// TODO - Best-first search
		// TODO - A* search (best-first search with back-tracking)
	};
} // End of the graphsearch namespace.
