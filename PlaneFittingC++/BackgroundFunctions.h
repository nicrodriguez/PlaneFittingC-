#pragma once
#include <iostream>
#include <vector>

using namespace std;
class BackgroundFunctions
{
public:
	BackgroundFunctions();
	~BackgroundFunctions();

	vector<double> normalize3D(vector<long double>);

	void planeFromNormal(long double, long double, long double, int, int, int, int);

	double rmsError(vector<double>, vector<double>);

};

