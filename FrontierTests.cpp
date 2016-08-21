/*
(C) David J. Kalbfleisch 2016
You are welcome to use this code pursuant to the GNU General Public license.  See the file, LICENSE.

This code tests Frontier.h.
*/

#include <memory>          // std::unique_ptr
#include "cute.h"
#include "Frontier.h"
#include "FrontierTests.h"

static void frontierTestsHelper1(graphsearch::Frontier<SimpleStruct>* frontier)
{
	SimpleStruct ss1{1}, ss2{2};

	ASSERTM("The frontier should start out empty.", frontier->isEmpty());
	frontier->add(ss1);
	ASSERTM("The frontier should no longer be empty.", !frontier->isEmpty());
	frontier->add(ss2);
}

static void frontierTestsHelper2(graphsearch::Frontier<SimpleStruct>* frontier)
{
	frontier->pop();
	frontier->pop();
	ASSERTM("The frontier should now be empty.", frontier->isEmpty());
}

void BreadthFirstFrontierTest() {
	std::unique_ptr<graphsearch::BreadthFirstFrontier<SimpleStruct>> frontier{new graphsearch::BreadthFirstFrontier<SimpleStruct>};
	frontierTestsHelper1(frontier.get());
	ASSERTM("A breadth-first frontier should be first-in-first-out (FIFO).", (frontier->next() == SimpleStruct{1}));
	ASSERTM("A breadth-first frontier should not be last-in-first-out (LIFO).", !(frontier->next() == SimpleStruct{2}));
	frontierTestsHelper2(frontier.get());
}

void DepthFirstFrontierTest() {
	std::unique_ptr<graphsearch::DepthFirstFrontier<SimpleStruct>> frontier{new graphsearch::DepthFirstFrontier<SimpleStruct>};
	frontierTestsHelper1(frontier.get());
	ASSERTM("A depth-first frontier should be last-in-first-out (LIFO).", (frontier->next() == SimpleStruct{2}));
	ASSERTM("A depth-first frontier should not be first-in-first-out (FIFO).", !(frontier->next() == SimpleStruct{1}));
	frontierTestsHelper2(frontier.get());
}
