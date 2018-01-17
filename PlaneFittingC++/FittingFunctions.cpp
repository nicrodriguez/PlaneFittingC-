#include "stdafx.h"
#include "BackgroundFunctions.h"
#include "FittingFunctions.h"



FittingFunctions::FittingFunctions()
{
}


FittingFunctions::~FittingFunctions()
{
}


PlaneFit FittingFunctions::simplePlaneFit(vector<int> xPoints, vector<int> yPoints, vector<double> zPoints, vector<double> zPointsA) {
	BackgroundFunctions BF;
	vector<double> calculatedZ;

	if ((xPoints.size() != yPoints.size()) || (xPoints.size() != zPoints.size()) || (yPoints.size() != zPoints.size())) {
		cout << "Point vectors are not the same size" << endl;
		return{};
	}

	// Calculating sum of points
	double xSum = 0, ySum = 0, zSum = 0, xBar = 0, yBar = 0, zBar = 0;
	for (int i = 0; i < xPoints.size(); i++) {
		xSum += xPoints[i];
		ySum += yPoints[i];
		zSum += zPoints[i];
	}

	// Calculating centroid of points;
	xBar = xSum / xPoints.size();
	yBar = ySum / yPoints.size();
	zBar = zSum / zPoints.size();
	double centroid[3] = { xBar, yBar, zBar };

	// Calculating Covariance Matrix
	long double xx = 0, yy = 0, zz = 0, xy = 0, xz = 0, yz = 0, det_x, det_y, det_z;
	for (int i = 0; i < xPoints.size(); i++) {
		xx += (xPoints[i] - centroid[0])*(xPoints[i] - centroid[0]);
		yy += (yPoints[i] - centroid[1])*(yPoints[i] - centroid[1]);
		xy += (xPoints[i] - centroid[0])*(yPoints[i] - centroid[1]);
		xz += (xPoints[i] - centroid[0])*(zPoints[i] - centroid[2]);
		yz += (yPoints[i] - centroid[1])*(zPoints[i] - centroid[2]);
		zz += (zPoints[i] - centroid[2])*(zPoints[i] - centroid[2]);
	}

	// Calculating determinants for each axis
	det_x = yy * zz - yz * yz;
	det_y = xx * zz - xz * xz;
	det_z = xx * yy - xy * xy;
	long double max_num[3] = { det_x, det_y, det_z };

	// Determining the main axis
	long double max = 0;
	for (long double number : max_num) {
		if (max <= number) {
			max = number;
		}
	}

	// Making sure the points actually span a plane
	if (max == 0) {
		cout << "Points don't span a plane" << endl;
		return{};
	}

	vector<long double> n;
	if (max == det_x) {
		n.insert(n.end(), max);
		n.insert(n.end(), xz * yz - xy * zz);
		n.insert(n.end(), xy * yz - xz * yy);
	}
	else if (max == det_y) {
		n.insert(n.end(), xz * yz - xy * zz);
		n.insert(n.end(), max);
		n.insert(n.end(), xy * xz - yz * xx);
	}
	else {
		n.insert(n.end(), xy * yz - xz * yy);
		n.insert(n.end(), xy * xz - yz * xx);
		n.insert(n.end(), max);
	}

	// Normalizing the 3D vector
	vector<double> normal = BF.normalize3D(n);

	// Calculating quality of fit with rms Error
	// Equation for z-position of plane is z = -(a*x + b*y)/c

	for (int i = 0; i < zPoints.size(); i++) {
		calculatedZ.insert(calculatedZ.end(), -(normal[0] * xPoints[i] + normal[1] * yPoints[i]) / normal[2]);
	}

	// Calculating rms error
	double rmsError = BF.rmsError(calculatedZ, std::move(zPointsA));

	return{ rmsError, normal };
}

// Robust Plane Fit Function
PlaneFit FittingFunctions::robustPlaneFit(vector<int> xPoints, vector<int> yPoints, vector<double> zPoints, vector<double> zPointsA) {
	BackgroundFunctions BF;
	vector<double> calculated_Z;

	// Calculating centroid of the points
	double xSum = 0, ySum = 0, zSum = 0, xBar = 0, yBar = 0, zBar = 0;

	// Summing points for centroid calculations
	for (int i = 0; i < xPoints.size(); i++) {
		xSum += xPoints[i];
		ySum += yPoints[i];
		zSum += zPoints[i];
	}

	// Calculating centroid
	xBar = xSum / xPoints.size();
	yBar = ySum / yPoints.size();
	zBar = zSum / zPoints.size();
	double centroid[3] = { xBar, yBar, zBar };

	// Calculating Covariance Matrix
	long double xx = 0, yy = 0, zz = 0, xy = 0, xz = 0, yz = 0, det_x, det_y, det_z, weightX, weightY, weightZ;
	for (int i = 0; i < xPoints.size(); i++) {
		xx += (xPoints[i] - centroid[0]) * (xPoints[i] - centroid[0]);
		yy += (yPoints[i] - centroid[1]) * (yPoints[i] - centroid[1]);
		xy += (xPoints[i] - centroid[0]) * (yPoints[i] - centroid[1]);
		xz += (xPoints[i] - centroid[0]) * (zPoints[i] - centroid[2]);
		yz += (yPoints[i] - centroid[1]) * (zPoints[i] - centroid[2]);
		zz += (zPoints[i] - centroid[2]) * (zPoints[i] - centroid[2]);
	}

	// Calculating determinants and weights for each axis
	long double weighted_nx = 0, weighted_ny = 0, weighted_nz = 0;

	det_x = yy * zz - yz * yz;
	det_y = xx * zz - xz * xz;
	det_z = xx * yy - xy * xy;

	weightX = det_x * det_x;
	weightY = det_y * det_y;
	weightZ = det_z * det_z;


	// Putting covariance matrix together
	long double nx[3] = { det_x, xz * yz - xy * zz, xy * yz - xz * yy };
	long double ny[3] = { xz * yz - xy * zz, det_y, xy * xz - yz * xx };
	long double nz[3] = { xy * yz - xz * yy, xy * xz - yz * xx, det_z };

	long double dot_prodx = 0 * nx[0] + 0 * nx[1] + 0 * nx[2];
	long double dot_prody = 0 * ny[0] + 0 * ny[1] + 0 * ny[2];
	long double dot_prodz = 0 * nz[0] + 0 * nz[1] + 0 * nz[2];

	if (dot_prodx < 0)
		weightX = -weightX;

	if (dot_prody < 0)
		weightY = -weightY;

	if (dot_prodz < 0)
		weightZ = -weightZ;

	for (int i = 0; i < sizeof(nx) / sizeof(nx[0]); i++) {
		weighted_nx += nx[i] * weightX;
		weighted_ny += ny[i] * weightY;
		weighted_nz += nz[i] * weightZ;
	}
	cout << "WeightNX: " << weighted_nx << endl;
	cout << "WeightNY: " << weighted_ny << endl;
	cout << "WeightNZ: " << weighted_nz << endl;
	cout << endl;

	// Prepping for normalization
	vector<long double> n;
	n.insert(n.end(), weighted_nx);
	n.insert(n.end(), weighted_ny);
	n.insert(n.end(), weighted_nz);

	for (double val : nx) {
		cout << "nx: " << val << endl;
	}
	cout << endl;
	for (double val : ny) {
		cout << "ny: " << val << endl;
	}
	cout << endl;
	for (double val : nz) {
		cout << "nz: " << val << endl;
	}
	cout << endl;
	// Normalizing the 3D vector
	vector<double> normal = BF.normalize3D(n);

	// Calculating plane points
	// Equation for z-position of plane is z = -(a*x + b*y)/c
	for (int i = 0; i < zPoints.size(); i++) {
		calculated_Z.insert(calculated_Z.end(), -(normal[0] * xPoints[i] + normal[1] * yPoints[i]) / normal[2]);
	}

	//for (double val : calculated_Z)
	//	cout << val << endl;
	// Calculating rms error
	double rmsError = BF.rmsError(calculated_Z, std::move(zPointsA));

	return{ rmsError,  normal };
}