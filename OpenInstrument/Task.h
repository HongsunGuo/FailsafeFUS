#pragma once
#include <string>
#include <memory>
#include <vector>

class Task
{
public:
	Task(const std::string &sname = "", const std::string& port = "",
		const std::string &des = "", const std::string& co = "") :
		shortName(sname), serialPortName(port), description(des), command(co){}
	std::string shortName; //unique
	std::string serialPortName;
	std::string description;
	std::string command;
};

class SequenceItem {
public:
	SequenceItem(const std::string &shortname, int delay = 0) :
		taskShortName(shortname), delaySec(delay) {}
	//std::shared_ptr<Task> taskPtr;
	std::string taskShortName = "";
	int delaySec = 0;
};


class SequenceListItem {
public:
	SequenceListItem(const std::string &name): shortName(name) {};
	std::vector<SequenceItem> sequenceList;
	std::string shortName; //unique
};
