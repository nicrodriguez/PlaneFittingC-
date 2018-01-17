#pragma once
#include <iostream>
#include <vector>
#include <cmath>

using namespace std;
class BackgroundFunctions
{
public:
	BackgroundFunctions();
	~BackgroundFunctions();

	vector<double> normalize3D(vector<long double>);


	void planeFromNormal(long double, long double, long double, int, int, int, int);

	double rmsError(vector<double>, vector<double>);

	double point_to_plane_distance(vector<double>, double, double, double, double);

	double plane_to_point_rms(vector<double>, vector<vector<double>>, vector<double>);

};

