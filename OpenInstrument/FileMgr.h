#pragma once
#include <string>
#include <memory>
#include <vector>

class Task;
class SequenceListItem;
class FileMgr
{
public:
/*
	static bool readDataFile(const std::string& filename,
		std::vector<std::pair<double, double>>& data);
*/
	static bool readDataFile(const std::string& filename,
		std::pair<std::vector<double>, std::vector<double>>& data);
	static bool saveTasksToFile(const std::vector<Task> &taskList,
		const std::string &fileName);
	static bool readTasksFromFile(std::vector<Task>& taskList,
		const std::string& fileName);
	static bool saveSeqListToFile(const std::vector<SequenceListItem>& seqList,
		const std::string& fileName);
	static bool readSeqListFromFile(std::vector<SequenceListItem>& seqList,
		const std::string& fileName);
};

