/*
(C) David J. Kalbfleisch 2016
You are welcome to use this code pursuant to the GNU General Public license.  See the file, LICENSE.

This code tests Frontier.h and GraphSearch.h.
*/

#include "cute.h"
#include "ide_listener.h"
#include "xml_listener.h"
#include "cute_runner.h"

#include "FrontierTests.h"
#include "GraphSearchTests.h"

// Create a test suite for Frontier.h.
void runFrontierTests(int argc, const char* argv[])
{
	cute::suite s;
	s.push_back(CUTE(BreadthFirstFrontierTest));
	s.push_back(CUTE(DepthFirstFrontierTest));
	cute::xml_file_opener xmlfile(argc,argv);
	cute::xml_listener<cute::ide_listener<>>  lis(xmlfile.out);
	cute::makeRunner(lis, argc, argv)(s, "Frontier Tests");
}

// Create a test suite for GraphSearch.h.
void runGraphSearchTests(int argc, const char* argv[])
{
	cute::suite s;
	s.push_back(CUTE(NoSolutionTest));
	s.push_back(CUTE(SimpleBreadthFirstSearchTest));
	s.push_back(CUTE(SimpleDepthFirstSearchTest));
	s.push_back(CUTE(ExhaustiveSearchTest));
	s.push_back(CUTE(SimpleCyclesDepthFirstSearchTest));
	s.push_back(CUTE(SimpleStructBreadthFirstSearchTest));
	s.push_back(CUTE(SimpleStructDepthFirstSearchTest));
	cute::xml_file_opener xmlfile(argc,argv);
	cute::xml_listener<cute::ide_listener<>>  lis(xmlfile.out);
	cute::makeRunner(lis, argc, argv)(s, "GraphSearch Tests");
}

// Run all test suites.
int main(int argc, const char* argv[])
{
    runFrontierTests(argc, argv);
    runGraphSearchTests(argc, argv);
    return 0;
}



