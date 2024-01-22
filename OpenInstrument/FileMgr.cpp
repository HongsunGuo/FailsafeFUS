#include <iostream>
#include <fstream>
#include "FileMgr.h"
#include "Task.h"

using namespace std;

/*
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
*/

bool FileMgr::readDataFile(const string& filename, pair<vector<double>, vector<double>>& data) {
    ifstream file(filename);
    // Check if the file was opened successfully
    if (!file.is_open()) {
        std::cerr << "Error opening file." << std::endl;
        return false;
    }

    double x, y;
    // Read the file line by line
    while (file >> x >> y) {
        data.first.push_back(x);
        data.second.push_back(y);
    }

    file.close();
    return true;
}

//Will change the file to JOSN format because iofstream treat spaces as delimiters. If a string has delimiters,
//it may not be processed correctly.
bool FileMgr::saveTasksToFile(const vector<Task>& taskList, const string& fileName)
{
    ofstream file(fileName);
    if (!file.is_open())
        return false;

    file << taskList.size() << endl;
    for (const auto& t : taskList) {
        file << t;
    }
    return true;
}

bool FileMgr::readTasksFromFile(vector<Task>& taskList, const string& fileName)
{
    ifstream file(fileName);
    if (!file.is_open())
        return false;
    size_t len = 0;
    file >> len;
    Task t;
    taskList.clear();
    for (size_t i = 0; i < len; ++i) {
        file >> t;
        taskList.push_back(t);
    }

    return true;
}

bool FileMgr::saveSeqListToFile(const vector<SequenceListItem>& seqList, const string& fileName) {
    ofstream file(fileName);
    if (!file.is_open())
        return false;

    for (const auto& sL : seqList) {
        file << sL;
    }
    return true;
}

bool FileMgr::readSeqListFromFile(vector<SequenceListItem>& seqList, const string& fileName) {
    std::ifstream file(fileName);
    if (!file.is_open())
        return false;

    SequenceListItem sL;
    seqList.clear();
    while (file >> sL) {
        seqList.push_back(sL);
    }
    return true;
}
