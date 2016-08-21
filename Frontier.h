/*
(C) David J. Kalbfleisch 2016
You are welcome to use this code pursuant to the GNU General Public license.  See the file, LICENSE.

This code implements several types of "Frontier" classes for use with GraphSearch.h.  This is useful
for ensuring frontiers have a standard interface, which in turn reduces code.  For example, a breadth-
first search should use a queue as the frontier; a depth-first search, a stack.  In both cases, we
need to be able to add nodes to the frontier and get/remove the next node.  The C++ standard
template library provides queues and stacks, but they use different method names for these operations.

TODO - This code has room for improvement.  Three of the four overridden methods are identical.
I would like to have the container member as part of the base class.  Then I could define those
three methods in the base class and only override "add."  Alternatively, I might be able to
use multiple inheritance with the child classes.
*/

#pragma once

#include <queue>
#include <stack>

namespace graphsearch
{
	/* This abstract base class specifies an interface for use in GraphSearch.h. */
	template <typename NodeType>
	class Frontier
	{
	public:
		virtual void add(const NodeType& node) = 0;

		virtual const NodeType& next() const = 0;

		virtual void pop() = 0;

		virtual bool isEmpty() const = 0;
	};

	template <typename NodeType>
	class DepthFirstFrontier : public Frontier<NodeType>
	{
		std::stack<NodeType> container;

	public:
		void add(const NodeType& node) override
		{
			container.push(node);
		}

		const NodeType& next() const override
		{
			return container.top();
		}

		void pop() override
		{
			container.pop();
		}

		bool isEmpty() const override
		{
			return container.empty();
		}
	};

	template <typename NodeType>
	class BreadthFirstFrontier : public Frontier<NodeType>
	{
		std::queue<NodeType> container;

	public:
		void add(const NodeType& node) override
		{
			container.push(node);
		}

		const NodeType& next() const override
		{
			return container.front();
		}

		void pop() override
		{
			container.pop();
		}

		bool isEmpty() const override
		{
			return container.empty();
		}
	};
} // End of the graphsearch namespace.
