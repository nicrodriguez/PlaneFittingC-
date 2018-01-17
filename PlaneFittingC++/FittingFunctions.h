#pragma once
#include <iostream>
#include <vector>
#include <utility>

using namespace std;

struct PlaneFit {
	vector<double> calculated_z;
	vector<double> normals;
	vector<double> centroid;
};

class FittingFunctions {



public:

	FittingFunctions();
	~FittingFunctions();

	// Plane Fitting Functions
	PlaneFit simplePlaneFit(vector<double>, vector<double>, vector<double>);
	PlaneFit robustPlaneFit(vector<double>, vector<double>, vector<double>);
};