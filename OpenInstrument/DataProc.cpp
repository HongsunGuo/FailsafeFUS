#include "DataProc.h"

#include <utility>

using namespace std;
vector<double> DataProc::movemean(const vector<double>& data, const int &k) {
    vector<double> result;
    if (k <= 0 || k >= data.size())
		return result;
	
    double sum = 0, windowSize = k;
    
    for (size_t i = 0; i < data.size(); ++i) {
        sum += data[i];

        if (i >= windowSize - 1) {
            result.push_back(sum / windowSize);
            sum -= data[i - windowSize + 1];
        }
    }

    for (size_t i = data.size() - windowSize + 1; i < data.size(); i++) {
        sum -= data[i - 1];
        result.push_back(sum / (data.size() - i));
    }

    return result;
}

pair<vector<double>, vector<double>> DataProc::movemean(const pair<vector<double>, vector<double>>& data, const int& k) {
    pair<vector<double>, vector<double>> result;
    if (k <= 0 || k >= data.first.size() || data.first.size() != data.second.size())
        return result;
    result.first = data.first;
    //
    double sum = 0;;
    int halfWin = k / 2; // if k = 20, halfWin = 10
    for (int i = 0; i < halfWin - 1; i++) {
        sum += data.second[i];
    }
    for (size_t i = 0; i < data.first.size(); ++i) {
        if(i + halfWin - 1 < data.first.size())
            sum += data.second[i + halfWin - 1];
        if (i > halfWin)
            sum -= data.second[i - halfWin - 1];
        //
        int up = i < data.first.size() + 1 - halfWin ? i + halfWin - 1 : data.first.size();
        int lo = i > halfWin ? i - halfWin - 1 : 0;
        double winLen = up - lo;
        result.second.push_back(sum / winLen);
    }
    return result;
}

vector<int> DataProc::findMaxPeaks(const vector<double> &yData, const int minWinLen) {
    if (yData.size() <= minWinLen)
        return {};

    vector<int> res = {};
    int halfWin = minWinLen / 2;
    for (int k = halfWin; k < yData.size() - halfWin; k++) {
        int lo = k - halfWin >= 0 ? k - halfWin : 0;
        int up = k + halfWin <= yData.size() ? k + halfWin : yData.size();
        bool found = true;
        while (lo < up) {
            if (yData[lo] > yData[k]) {
                found = false;
                break;
            }
            lo++;
        }
        if (found)
            res.push_back(k);
       
    }

    return res;
}
