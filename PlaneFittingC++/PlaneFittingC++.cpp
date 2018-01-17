// PlaneFittingC++.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "FileHandler.h"
#include "FittingFunctions.h"
#include "BackgroundFunctions.h"
#include <iostream>
#include <vector>





using namespace std;

int main()
{
	// Obtaining data from text file.

	FileHandler fileHandler;
	vector<double> xPoints, yPoints, xPointsN, yPointsN, xPointsN2, yPointsN2, xPointsN3, yPointsN3;
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
	BackgroundFunctions BF;

	PlaneFit sf, sfN, sfN2, sfN3, rf, rfN, rfN2, rfN3;
	if (a == 4) {
		// Reference 
		sf = FF.simplePlaneFit(xPoints, yPoints, zPoints);

		// Simple plane fit to noisy data
		sfN = FF.simplePlaneFit(xPointsN, yPointsN, zPointsN);
		sfN2 = FF.simplePlaneFit(xPointsN2, yPointsN2, zPointsN2);
		sfN3 = FF.simplePlaneFit(xPointsN3, yPointsN3, zPointsN3);

		//Robust plane fit to noisy data
		rfN = FF.robustPlaneFit(xPointsN, yPointsN, zPointsN);
		rfN2 = FF.robustPlaneFit(xPointsN2, yPointsN2, zPointsN2);
		rfN3 = FF.robustPlaneFit(xPointsN3, yPointsN3, zPointsN3);

		double rmsR, rmsNR;
		double rmsS1, rmsS2, rmsS3, rmsNS1, rmsNS2, rmsNS3;
		double rmsR1, rmsR2, rmsR3, rmsNR1, rmsNR2, rmsNR3;

		// Calculating reference points
		rmsR = BF.rmsError(zPoints, sf.calculated_z);
		vector<vector<double>> c_c(3), c_c1(3), c_c2(3), c_c3(3);
		c_c = { xPoints, yPoints, zPoints };
		rmsNR = BF.plane_to_point_rms(sf.normals, c_c, sf.centroid);

		// Calculating rms errors for simple fittings
		rmsS1 = BF.rmsError(sfN.calculated_z, zPoints);
		rmsS2 = BF.rmsError(sfN2.calculated_z, zPoints);
		rmsS3 = BF.rmsError(sfN3.calculated_z, zPoints);

		c_c1 = { xPointsN, yPointsN, sfN.calculated_z };
		c_c2 = { xPointsN2, yPointsN2, sfN2.calculated_z };
		c_c3 = { xPointsN3, yPointsN3, sfN3.calculated_z };
		rmsNS1 = BF.plane_to_point_rms(sf.normals, c_c1, sfN.centroid);
		rmsNS2 = BF.plane_to_point_rms(sf.normals, c_c2, sfN2.centroid);
		rmsNS3 = BF.plane_to_point_rms(sf.normals, c_c3, sfN3.centroid);

		// Calculating rms errors for robust fittings
		rmsR1 = BF.rmsError(rfN.calculated_z, zPoints);
		rmsR2 = BF.rmsError(rfN2.calculated_z, zPoints);
		rmsR3 = BF.rmsError(rfN3.calculated_z, zPoints);

		c_c1 = { xPointsN, yPointsN, rfN.calculated_z };
		c_c2 = { xPointsN2, yPointsN2, rfN2.calculated_z };
		c_c3 = { xPointsN3, yPointsN3, rfN3.calculated_z };
		rmsNR1 = BF.plane_to_point_rms(sf.normals, c_c1, rfN.centroid);
		rmsNR2 = BF.plane_to_point_rms(sf.normals, c_c2, rfN2.centroid);
		rmsNR3 = BF.plane_to_point_rms(sf.normals, c_c3, rfN3.centroid);
		


		cout << endl << "------------------- Creating Plane From Points -------------------" << endl;
		cout << "      noise = ±n units             points = (x, y, z)" << endl;
		cout << "      (Both fits are tested using same data) " << endl << endl;


		cout << "----------------- Reference Plane ----------------- " << endl;
		cout << "The normalized vector w/o noise: -------------- (" << sf.normals[0] << ", " << sf.normals[1] << ", " << sf.normals[2] << ")" << endl << endl;

		cout << "              ----- Z - Difference -----         " << endl;
		cout << "Point-by-Point RMS error no noise: ------------- " << rmsR << "%" << endl;
		cout << "              ----- Point to Plane -----         " << endl;
		cout << "Point-by-Point RMS error no noise: ------------- " << rmsNR << "%" << endl << endl;

		cout << endl << "------------------- Simple Plane Fit -------------------" << endl;
		cout << "The normalized vector w/ noise (±1 unit): ----- (" << sfN.normals[0] << ", " << sfN.normals[1] << ", " << sfN.normals[2] << ")" << endl;
		cout << "The normalized vector w/ noise (±2 unit): ----- (" << sfN2.normals[0] << ", " << sfN2.normals[1] << ", " << sfN2.normals[2] << ")" << endl;
		cout << "The normalized vector w/ noise (±3 unit): ----- (" << sfN3.normals[0] << ", " << sfN3.normals[1] << ", " << sfN3.normals[2] << ")" << endl << endl;

		cout << "              ----- Z - Difference -----         " << endl;
		cout << "Point-by-Point RMS error with noise (±2 unit): - " << rmsS1 << "%" << endl;
		cout << "Point-by-Point RMS error with noise (±2 unit): - " << rmsS2 << "%" << endl;
		cout << "Point-by-Point RMS error with noise (±3 unit): - " << rmsS3 << "%" << endl;
		cout << "              ----- Point to Plane -----         " << endl;
		cout << "Point-by-Point RMS error with noise (±1 unit): - " << rmsNS1 << "%" << endl;
		cout << "Point-by-Point RMS error with noise (±2 unit): - " << rmsNS2 << "%" << endl;
		cout << "Point-by-Point RMS error with noise (±3 unit): - " << rmsNS3 << "%" << endl << endl;

		cout << endl << "------------------- Robust Plane Fit -------------------" << endl;
		cout << "The normalized vector w/ noise (±1 unit): ----- (" << rfN.normals[0] << ", " << rfN.normals[1] << ", " << rfN.normals[2] << ")" << endl;
		cout << "The normalized vector w/ noise (±2 unit): ----- (" << rfN2.normals[0] << ", " << rfN2.normals[1] << ", " << rfN2.normals[2] << ")" << endl;
		cout << "The normalized vector w/ noise (±3 unit): ----- (" << rfN3.normals[0] << ", " << rfN3.normals[1] << ", " << rfN3.normals[2] << ")" << endl << endl;

		cout << "              ----- Z - Difference -----         " << endl;
		cout << "Point-by-Point RMS error with noise (±1 unit): - " << rmsR1 << "%" << endl;
		cout << "Point-by-Point RMS error with noise (±2 unit): - " << rmsR2 << "%" << endl;
		cout << "Point-by-Point RMS error with noise (±3 unit): - " << rmsR3 << "%" << endl;
		cout << "              ----- Point to Plane -----         " << endl;
		cout << "Point-by-Point RMS error with noise (±1 unit): - " << rmsNR1 << "%" << endl;
		cout << "Point-by-Point RMS error with noise (±2 unit): - " << rmsNR2 << "%" << endl;
		cout << "Point-by-Point RMS error with noise (±3 unit): - " << rmsNR3 << "%" << endl;
	}
	else {
		cout << "Not all files were opened succussefully" << endl;
	}
	return 0;
}

