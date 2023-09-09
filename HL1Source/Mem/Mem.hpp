#pragma once
#include <Windows.h>
#include <Psapi.h>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>



namespace M
{
	std::vector<std::string> split(const std::string& str, char delimiter);

	uintptr_t scanpattern(const char* Library, const char* Bytes);
}