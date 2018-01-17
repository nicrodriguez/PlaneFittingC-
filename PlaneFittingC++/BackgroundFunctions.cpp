#include "stdafx.h"
#include "BackgroundFunctions.h"


BackgroundFunctions::BackgroundFunctions()
{
}


BackgroundFunctions::~BackgroundFunctions()
{
}

vector<double> BackgroundFunctions::normalize3D(vector<long double> unormalized_vector)
{
	vector<double> normalized_vector;

	long double magnitude = sqrt(unormalized_vector[0] * unormalized_vector[0]
		+ unormalized_vector[1] * unormalized_vector[1]
		+ unormalized_vector[2] * unormalized_vector[2]);

	normalized_vector.insert(normalized_vector.end(), static_cast<double &&>(unormalized_vector.at(0) / magnitude));
	normalized_vector.insert(normalized_vector.end(), static_cast<double &&>(unormalized_vector.at(1) / magnitude));
	normalized_vector.insert(normalized_vector.end(), static_cast<double &&>(unormalized_vector.at(2) / magnitude));

	return normalized_vector;
}

void BackgroundFunctions::planeFromNormal(long double a, long double b, long double c, int xMin = 0, int xMax = 10, int yMin = 0, int yMax = 10) {

	// Creating x and y ranges for mesh grid to create surface
	vector<int> xRange, yRange;
	vector<vector<long double>> meshgrid;
	for (int i = xMin; i <= xMax; i++)
		xRange.insert(xRange.end(), i);


	for (int i = yMin; i <= yMax; i++)
		yRange.insert(yRange.end(), i);

	for (int x : xRange) {
		for (int y : yRange) {
			vector<long double> point;
			point.insert(point.end(), x);
			point.insert(point.end(), y);
			point.insert(point.end(), -(a*x + b * y) / c);
			meshgrid.insert(meshgrid.end(), point);
		}
	}

}

double BackgroundFunctions::rmsError(vector<double> fittedValues, vector<double> actualValues) {



	if (fittedValues.size() != actualValues.size()) {
		cout << "Value vectors are not the same size" << endl;
		return -1.0;
	}
	// finding the error between corresponding points
	const unsigned long N = fittedValues.size();
	double errorRMS, meanError, errorSum = 0;
	vector<double> errors(fittedValues.size());

	// squaring the percent error to prepare for RMS evaluation
	for (int i = 0; i < fittedValues.size(); i++) {

		//        cout << fittedValues.at(i) << " "<< actualValues.at(i) << endl;
		if (actualValues[i] == 0) {
			errors[i] = (actualValues[i] - fittedValues[i]) / fittedValues[i];
			errors[i] = errors[i] * errors[i];
			errorSum += errors[i];
			continue;
		}
		errors[i] = (fittedValues[i] - actualValues[i]) / actualValues[i];
		errors[i] = errors[i] * errors[i];
		errorSum += errors[i];

	}

	// Calculating the mean of the squared errors
	meanError = errorSum / fittedValues.size();

	// Calculating the RMS error [%]
	errorRMS = 100 * sqrt(abs(meanError));

	return errorRMS;
}
