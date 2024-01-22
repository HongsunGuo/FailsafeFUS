#pragma once
#include <vector>

class DataProc
{
public:
	static std::vector<double> movemean(const std::vector<double> &data, const int& k);
	static std::pair<std::vector<double>, std::vector<double>>
		movemean(const std::pair<std::vector<double>, std::vector<double>> &data, const int &k);
	static std::vector<int> findMaxPeaks(const std::vector<double> &yData, const int minWinLen);
};
