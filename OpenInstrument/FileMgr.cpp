#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include "FileMgr.h"
#include "Task.h"

using namespace std;
namespace fs = std::filesystem;
const string seqFolder = "./seq";
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

    for (const auto& t : taskList) {
        file << t << endl;
    }
    return true;
}

bool FileMgr::readTasksFromFile(vector<Task>& taskList, const string& fileName)
{
    ifstream file(fileName);
    if (!file.is_open())
        return false;

    string line;
    Task t;
    while (std::getline(file, line)) {
        if (line.empty())
            continue;
        istringstream iss(line);
        getline(iss, t.shortName, delimiter);
        getline(iss, t.serialPortName, delimiter);
        getline(iss, t.description, delimiter);
        getline(iss, t.command, delimiter);
        taskList.push_back(t);
    }

    return true;
}

bool FileMgr::saveSeqListToFile(const vector<SequenceListItem>& seqList) {
    fs::create_directory(seqFolder); // if seqFolder already exists, it will return false and will not do anything.
    fs::path folder(seqFolder);

    for (const auto& sL : seqList) {
        fs::path fullPath = folder / sL.shortName;
        ofstream file(fullPath.string());
        if (!file.is_open())
            continue;
        file << sL;
    }
    return true;
}

bool FileMgr::readSeqListFromFile(vector<SequenceListItem>& seqList) {
    fs::path folder(seqFolder);
    seqList.clear();
    vector<string> shortNames;
    if (!fs::exists(seqFolder) || !fs::is_directory(seqFolder))
        return false;

    for (const auto& entry : fs::directory_iterator(seqFolder)) {
        if (fs::is_regular_file(entry)) {
            shortNames.push_back(entry.path().filename().string());
        }
    }
    if (shortNames.empty())
        return false;

    for (const auto& entry : shortNames) {
        SequenceListItem sL;
        sL.shortName = entry;
        fs::path fullPath = folder / sL.shortName;
        std::ifstream file(fullPath.string());
        if (!file.is_open())
           continue;
        file >> sL;
        seqList.push_back(sL);
    }

    return shortNames.empty() ? false : true;
}
