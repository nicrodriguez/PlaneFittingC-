#pragma once
#include <iostream>
#include <vector>
#include <utility>

using namespace std;

struct PlaneFit {
	double rmsError;
	vector<double> normals;
};

class FittingFunctions {



public:

	FittingFunctions();
	~FittingFunctions();

	// Plane Fitting Functions
	PlaneFit simplePlaneFit(vector<int>, vector<int>, vector<double>, vector<double>);
	PlaneFit robustPlaneFit(vector<int>, vector<int>, vector<double>, vector<double>);
};