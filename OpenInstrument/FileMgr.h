#pragma once
#include <string>
#include <memory>
#include <vector>

class FileMgr
{
public:
	static bool readDataFile(const std::string& filename,
		std::vector<std::pair<double, double>>& data);
};

