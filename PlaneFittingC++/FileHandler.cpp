#include "stdafx.h"
#include "FileHandler.h"



FileHandler::FileHandler()
{
}


FileHandler::~FileHandler()
{
}


template<typename Out>
void FileHandler::split(const std::string &s, char delim, Out result) {
	stringstream ss(s);
	string item;
	while (getline(ss, item, delim)) {
		*(result++) = item;
	}
}

vector<string> FileHandler::split(const string &s, char delim) {
	vector<string> elems;
	split(s, delim, std::back_inserter(elems));
	return elems;
}

bool FileHandler::read_point_file(const string &directory, const string &point_type, vector<int> &xPoints, vector<int> &yPoints, vector<double> &zPoints) {
	string line, delimiter = ",";
	// Reading from perfect plane file
	ifstream point_file(directory);
	if (point_file.is_open()) {
		cout << point_type << " Successfully Opened" << endl;
		while (getline(point_file, line)) {
			vector<string> coord = split(line, ',');
			xPoints.insert(xPoints.end(), stoi(coord.at(0)));
			yPoints.insert(yPoints.end(), stoi(coord.at(1)));
			zPoints.insert(zPoints.end(), stod(coord.at(2)));
		}
		cout << "Data Obtained Successfully" << endl;
		point_file.close();
		cout << point_type << " Closed" << endl << endl;
		return true;
	}
	else {
		cout << point_type << " Unsuccessfully Opened" << endl;
		return false;
	}
}
