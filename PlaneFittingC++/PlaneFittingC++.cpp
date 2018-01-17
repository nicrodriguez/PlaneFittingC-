// PlaneFittingC++.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "FileHandler.h"
#include "FittingFunctions.h"
#include <iostream>
#include <vector>





using namespace std;

int main()
{
	// Obtaining data from text file.

	FileHandler fileHandler;
	vector<int> xPoints, yPoints, xPointsN, yPointsN, xPointsN2, yPointsN2, xPointsN3, yPointsN3;
	vector<double>zPoints, zPointsN, zPointsN2, zPointsN3;

	string perfect_plane_file = "points_on_plane.txt", file_type = "Point File";
	string noisy_plane_file = "noisy_points_plane.txt", file_type2 = "Noisy Point File";
	string noisy_plane_file2 = "noisy_points_plane2.txt", file_type3 = "Noisy Point2 File";
	string noisy_plane_file3 = "noisy_points_plane3.txt", file_type4 = "Noisy Point3 File";

	int a = 0;
	// Reading from perfect plane file
	if (fileHandler.read_point_file(perfect_plane_file, file_type, xPoints, yPoints, zPoints))
		a++;

	cout << "Got throught the first one" << endl;
	// Reading from noisy plane file Â± 1 unit
	if (fileHandler.read_point_file(noisy_plane_file, file_type2, xPointsN, yPointsN, zPointsN))
		a++;

	// Reading from noisy plane file Â± 2 units
	if (fileHandler.read_point_file(noisy_plane_file2, file_type3, xPointsN2, yPointsN2, zPointsN2))
		a++;

	// Reading from noisy plane file Â± 3 units
	if (fileHandler.read_point_file(noisy_plane_file3, file_type4, xPointsN3, yPointsN3, zPointsN3))
		a++;

	// Fitting the data
	FittingFunctions FF;
	PlaneFit sf, sfN, sfN2, sfN3, rf, rfN, rfN2, rfN3;
	if (a == 4) {
		sf = FF.simplePlaneFit(xPoints, yPoints, zPoints, zPoints);
		sfN = FF.simplePlaneFit(xPointsN, yPointsN, zPointsN, zPoints);
		sfN2 = FF.simplePlaneFit(xPointsN2, yPointsN2, zPointsN2, zPoints);
		sfN3 = FF.simplePlaneFit(xPointsN3, yPointsN3, zPointsN3, zPoints);

		rf = FF.robustPlaneFit(xPoints, yPoints, zPoints, zPoints);
		rfN = FF.robustPlaneFit(xPointsN, yPointsN, zPointsN, zPoints);
		rfN2 = FF.robustPlaneFit(xPointsN2, yPointsN2, zPointsN2, zPoints);
		rfN3 = FF.robustPlaneFit(xPointsN3, yPointsN3, zPointsN3, zPoints);

		cout << endl << "------------------- Creating Plane From Points -------------------" << endl;
		cout << "      noise = ±n units             points = (x, y, z)" << endl;
		cout << "      (Both fits are tested tested using same data) " << endl;
		cout << endl << "------------------- Simple Plane Fit -------------------" << endl;
		cout << "The normalized vector w/o noise: -------------- (" << sf.normals[0] << ", " << sf.normals[1] << ", " << sf.normals[2] << ")" << endl;
		cout << "The normalized vector w/ noise (±1 unit): ----- (" << sfN.normals[0] << ", " << sfN.normals[1] << ", " << sfN.normals[2] << ")" << endl;
		cout << "The normalized vector w/ noise (±2 unit): ----- (" << sfN2.normals[0] << ", " << sfN2.normals[1] << ", " << sfN2.normals[2] << ")" << endl;
		cout << "The normalized vector w/ noise (±3 unit): ----- (" << sfN3.normals[0] << ", " << sfN3.normals[1] << ", " << sfN3.normals[2] << ")" << endl << endl;

		cout << "Point-by-Point RMS error no noise: ------------- " << sf.rmsError << "%" << endl;
		cout << "Point-by-Point RMS error with noise (±1 unit): - " << sfN.rmsError << "%" << endl;
		cout << "Point-by-Point RMS error with noise (±2 unit): - " << sfN2.rmsError << "%" << endl;
		cout << "Point-by-Point RMS error with noise (±3 unit): - " << sfN3.rmsError << "%" << endl << endl;

		cout << endl << "------------------- Robust Plane Fit -------------------" << endl;
		cout << "The normalized vector w/o noise: -------------- (" << rf.normals[0] << ", " << rf.normals[1] << ", " << rf.normals[2] << ")" << endl;
		cout << "The normalized vector w/ noise (±1 unit): ----- (" << rfN.normals[0] << ", " << rfN.normals[1] << ", " << rfN.normals[2] << ")" << endl;
		cout << "The normalized vector w/ noise (±2 unit): ----- (" << rfN2.normals[0] << ", " << rfN2.normals[1] << ", " << rfN2.normals[2] << ")" << endl;
		cout << "The normalized vector w/ noise (±3 unit): ----- (" << rfN3.normals[0] << ", " << rfN3.normals[1] << ", " << rfN3.normals[2] << ")" << endl << endl;

		cout << "Point-by-Point RMS error no noise: ------------- " << rf.rmsError << "%" << endl;
		cout << "Point-by-Point RMS error with noise (±1 unit): - " << rfN.rmsError << "%" << endl;
		cout << "Point-by-Point RMS error with noise (±2 unit): - " << rfN2.rmsError << "%" << endl;
		cout << "Point-by-Point RMS error with noise (±3 unit): - " << rfN3.rmsError << "%" << endl << endl;
	}
	else {
		cout << "Not all files were opened succussefully" << endl;
	}
	return 0;
}

