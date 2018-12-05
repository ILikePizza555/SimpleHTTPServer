#pragma once

#include <string>
#include <vector>

std::vector<std::string> split(std::string string, std::string delim, int count) {
	std::vector<std::string> rv;

	while (count != 0) {


		int i = string.find(delim);
		rv.push_back(string.substr(0, i));
		string = string.substr(i);
		count--;
	}
}