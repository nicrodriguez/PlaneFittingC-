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
	for (unsigned int i = 0; i < fittedValues.size(); i++) {

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

double BackgroundFunctions::point_to_plane_distance(vector<double> normal, double x, double y, double z, double D)
{
	double n1 = normal[0] * x + normal[1] * y + normal[2] * z + D;
	double n2 = sqrt(normal[0]*normal[0] + normal[1]*normal[1] + normal[2]*normal[2]);
	return n1/n2;
}

double BackgroundFunctions::plane_to_point_rms(vector<double> normal_ref, vector<vector<double>> coordinates_c, vector<double> centroid)
{
	vector<double> x = coordinates_c[0], y = coordinates_c[1], z = coordinates_c[2];
	double x_c = centroid[0], y_c = centroid[1], z_c = centroid[2];

	vector<double>z_new(x.size());
	for (unsigned int i = 0; i < x.size(); i++) {
		double D = -(normal_ref[0] * x_c + normal_ref[1] * y_c + normal_ref[2] * z_c);
		double d = point_to_plane_distance(normal_ref, x[i], y[i], z[i], D);

		double x_new, y_new;
		if (d < 0) {
			x_new = x[i] + normal_ref[0] * d;
			y_new = y[i] + normal_ref[1] * d;

		}else {
			x_new = x[i] - normal_ref[0] * d;
			y_new = y[i] - normal_ref[1] * d;
		}

		z_new[i] = -(normal_ref[0] * x_new + normal_ref[1] * y_new) / normal_ref[2];
		}
	return rmsError(z, z_new);
}
