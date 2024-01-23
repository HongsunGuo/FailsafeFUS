#include "DataProc.h"

#include <utility>

using namespace std;

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

vector<double> DataProc::movemean(const vector<double>& data, const int& k) {
    vector<double>  result;
    if (k <= 0 || k >= data.size())
        return result;
    //
    double sum = 0;;
    int halfWin = k / 2; // if k = 20, halfWin = 10
    for (int i = 0; i < halfWin - 1; i++) {
        sum += data[i];
    }
    for (size_t i = 0; i < data.size(); ++i) {
        if (i + halfWin - 1 < data.size())
            sum += data[i + halfWin - 1];
        if (i > halfWin)
            sum -= data[i - halfWin - 1];
        //
        int up = i < data.size() + 1 - halfWin ? i + halfWin - 1 : data.size();
        int lo = i > halfWin ? i - halfWin - 1 : 0;
        double winLen = up - lo;
        result.push_back(sum / winLen);
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


vector<vector<int>> DataProc::findRangesOfPeaks(const vector<double>& yData, const vector<int> &peakInd) {
    vector<double> diffData(yData.size() - 1, 0);
    for (int i = 0; i < diffData.size(); i++) {
        diffData[i] = yData[i + 1] - yData[i];
    }

    diffData = DataProc::movemean(diffData, 10); //denoise
    double posLim = 0.01, negLim = -0.01;
    vector<vector<int>> res;
    for (const auto& ind : peakInd) {
        int curInd = ind;
        vector<int> curRange;
        while (curInd > 0) {
            if (diffData[curInd] > posLim && diffData[curInd - 1] < posLim) {
                break;
            }
            curInd--;
        }
        curRange.push_back(curInd);
        curInd = ind;
        while (curInd < diffData.size() - 1) {
            if (diffData[curInd] < negLim && diffData[curInd + 1] > negLim) {
                break;
            }
            curInd++;
        }
        curRange.push_back(curInd);
        res.push_back(curRange);
    }
    return res;
}

bool DataProc::removeBaseline(const std::vector<double>& yData, std::vector<double>& newYData) {
    vector<double> filteredData = movemean(yData, 20);
    vector<int> peakInds = findMaxPeaks(filteredData, 70);
    if (peakInds.empty())
        return false;
    newYData.resize(yData.size(), 0);
    vector<vector<int>> ranges = findRangesOfPeaks(filteredData, peakInds);
    for (const auto& rg : ranges) {
        double y0 = filteredData[rg[0]], y1 = filteredData[rg[1]];
        double k = (y1 - y0) / (rg[1] - rg[0]);
        for (int i = rg[0]; i <= rg[1]; i++) {
            newYData[i] = filteredData[i] - (k * (i - rg[0]) + y0);
        }
    }
    return true;
}
