#include <iostream>
#include <fstream>
#include "FileMgr.h"

using namespace std;

bool FileMgr::readDataFile(const string& filename,
    vector<pair<double, double>> &data)
{
	ifstream file(filename);
    // Check if the file was opened successfully
    if (!file.is_open()) {
        std::cerr << "Error opening file." << std::endl;
        return false;
    }

    double x, y;

    // Read the file line by line
    while (file >> x >> y) {
        data.push_back(std::make_pair(x, y));
    }

    file.close();
	return true;
}
