#include "stdafx.h"
#include "BackgroundFunctions.h"
#include "FittingFunctions.h"



FittingFunctions::FittingFunctions()
{
}


FittingFunctions::~FittingFunctions()
{
}


PlaneFit FittingFunctions::simplePlaneFit(vector<double> xPoints, vector<double> yPoints, vector<double> zPoints) {
	BackgroundFunctions BF;
	vector<double> calculatedZ;

	if ((xPoints.size() != yPoints.size()) || (xPoints.size() != zPoints.size()) || (yPoints.size() != zPoints.size())) {
		cout << "Point vectors are not the same size" << endl;
		return{};
	}

	// Calculating sum of points
	double xSum = 0, ySum = 0, zSum = 0, xBar = 0, yBar = 0, zBar = 0;
	for (unsigned int i = 0; i < xPoints.size(); i++) {
		xSum += xPoints[i];
		ySum += yPoints[i];
		zSum += zPoints[i];
	}

	// Calculating centroid of points;
	xBar = xSum / xPoints.size();
	yBar = ySum / yPoints.size();
	zBar = zSum / zPoints.size();
	vector<double> centroid(3);
	centroid = { xBar, yBar, zBar };

	// Calculating Covariance Matrix
	long double xx = 0, yy = 0, zz = 0, xy = 0, xz = 0, yz = 0, det_x, det_y, det_z;
	for (unsigned int i = 0; i < xPoints.size(); i++) {
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

	for (unsigned int i = 0; i < zPoints.size(); i++) {
		calculatedZ.insert(calculatedZ.end(), -(normal[0] * xPoints[i] + normal[1] * yPoints[i]) / normal[2]);
	}

	//// Calculating rms error
	//double rmsError = BF.rmsError(calculatedZ, std::move(zPointsA));

	return{calculatedZ, normal, centroid};
}

// Robust Plane Fit Function
PlaneFit FittingFunctions::robustPlaneFit(vector<double> xPoints, vector<double> yPoints, vector<double> zPoints) {
	BackgroundFunctions BF;
	vector<double> calculated_Z;

	// Calculating centroid of the points
	double xSum = 0, ySum = 0, zSum = 0, xBar = 0, yBar = 0, zBar = 0;
	unsigned int n = xPoints.size();
	// Summing points for centroid calculations
	for (unsigned int i = 0; i < xPoints.size(); i++) {
		xSum += xPoints[i];
		ySum += yPoints[i];
		zSum += zPoints[i];
	}

	// Calculating centroid
	vector<double> centroid(3);
	centroid = { xSum/n, ySum/n, zSum/n };

	// Calculating Covariance Matrix
	long double xx = 0, yy = 0, zz = 0, xy = 0, xz = 0, yz = 0, det_x, det_y, det_z, weightX, weightY, weightZ;
	for (unsigned int i = 0; i < xPoints.size(); i++) {
		xx += (xPoints[i] - centroid[0]) * (xPoints[i] - centroid[0]);
		yy += (yPoints[i] - centroid[1]) * (yPoints[i] - centroid[1]);
		xy += (xPoints[i] - centroid[0]) * (yPoints[i] - centroid[1]);
		xz += (xPoints[i] - centroid[0]) * (zPoints[i] - centroid[2]);
		yz += (yPoints[i] - centroid[1]) * (zPoints[i] - centroid[2]);
		zz += (zPoints[i] - centroid[2]) * (zPoints[i] - centroid[2]);
	}
	
	xx /= n;
	yy /= n;
	xy /= n;
	xz /= n;
	yz /= n;
	zz /= n;

	// Calculating determinants and weights for each axis
	det_x = yy * zz - yz * yz;
	det_y = xx * zz - xz * xz;
	det_z = xx * yy - xy * xy;

	weightX = det_x * det_x;
	weightY = det_y * det_y;
	weightZ = det_z * det_z;
	vector<long double>w(3), nx(3), ny(3), nz(3);
	
	// Putting covariance matrix together
	w = { weightX, weightY, weightZ };
	nx = { det_x, xz * yz - xy * zz, xy * yz - xz * yy };
	ny = { xz * yz - xy * zz, det_y, xy * xz - yz * xx };
	nz = { xy * yz - xz * yy, xy * xz - yz * xx, det_z };

	vector<long double> weightedN(3);
	weightedN = { 0, 0, 0 };
	vector<vector<long double>>wn(3);
	wn = { nx, ny, nz };
	for (unsigned int i = 0; i < nx.size(); i++) {
		long double dot_prod = weightedN[0] * wn[i][0] + weightedN[1] * wn[i][1] + weightedN[2] * wn[i][2];
		
		if (dot_prod < 0)
			w[i] *= -1;

		weightedN[i] += wn[i][0] * w[i];
		weightedN[i] += wn[i][1] * w[i];
		weightedN[i] += wn[i][2] * w[i];
	}

	//long double dot_prodx = 0 * nx[0] + 0 * nx[1] + 0 * nx[2];
	//long double dot_prody = 0 * ny[0] + 0 * ny[1] + 0 * ny[2];
	//long double dot_prodz = 0 * nz[0] + 0 * nz[1] + 0 * nz[2];

	//if (dot_prodx < 0)
	//	weightX = -weightX;

	//if (dot_prody < 0)
	//	weightY = -weightY;

	//if (dot_prodz < 0)
	//	weightZ = -weightZ;

	//for (int i = 0; i < sizeof(nx) / sizeof(nx[0]); i++) {
	//	weighted_nx += nx[i] * weightX;
	//	weighted_ny += ny[i] * weightY;
	//	weighted_nz += nz[i] * weightZ;
	//}

	//// Prepping for normalization
	//vector<long double> n;
	//n.insert(n.end(), weighted_nx);
	//n.insert(n.end(), weighted_ny);
	//n.insert(n.end(), weighted_nz);

	// Normalizing the 3D vector
	vector<double> normal = BF.normalize3D(weightedN);

	// Calculating plane points
	// Equation for z-position of plane is z = -(a*x + b*y)/c
	for (unsigned int i = 0; i < zPoints.size(); i++) {
		calculated_Z.insert(calculated_Z.end(), -(normal[0] * xPoints[i] + normal[1] * yPoints[i]) / normal[2]);
	}

	//for (double val : calculated_Z)
	//	cout << val << endl;
	// Calculating rms error
	//double rmsError = BF.rmsError(calculated_Z, std::move(zPointsA));

	return{calculated_Z, normal, centroid};
}