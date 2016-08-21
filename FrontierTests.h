/*
(C) David J. Kalbfleisch 2016
You are welcome to use this code pursuant to the GNU General Public license.  See the file, LICENSE.

This code tests Frontier.h.
*/

#pragma once

/* A trivial structure to pass by reference to Frontier methods. */
struct SimpleStruct
{
	int x{0};

	SimpleStruct() = default;

	SimpleStruct(int _x) : x{_x}	{}

	bool operator==(const SimpleStruct& other) const
	{
		return x == other.x;
	}
};

// Test function prototypes:
void BreadthFirstFrontierTest();
void DepthFirstFrontierTest();
