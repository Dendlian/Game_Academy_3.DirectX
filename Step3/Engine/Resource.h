#pragma once
#include <string>

#pragma region Function Pointer ***
/*
	int add(int a, int b)
	{ return a+b; }

	int (*fcnPtr)(int int) = add;
*/
#pragma endregion

namespace Resource
{
	void Import(std::string const directory, void (*const function)(std::string const & file));
}
