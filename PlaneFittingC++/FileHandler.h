#pragma once
#include <iostream>
#include <vector>
#include <iterator>
#include <fstream>
#include <sstream>

using namespace std;

class FileHandler
{
private:
	template<typename Out>
	void split(const std::string &s, char delim, Out result);
	vector<string> split(const string &s, char delim);
public:
	FileHandler();
	~FileHandler();
	bool read_point_file(const string &, const string &, vector<double> &, vector<double> &, vector<double> &);

};