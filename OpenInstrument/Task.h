#pragma once
#include <string>
#include <memory>
#include <vector>
#include <iostream>
#include <sstream>

const char delimiter = '\v'; //'v' is least likely to be used in string.

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

	friend std::ostream& operator<<(std::ostream& os, const Task& t) {
		os << t.shortName << delimiter << t.serialPortName << delimiter
			<< t.description << delimiter << t.command << delimiter;
		return os;
	}

	friend std::istream& operator>>(std::istream& is, Task& t) {
		std::getline(is, t.shortName, delimiter);
		std::getline(is, t.serialPortName, delimiter);
		std::getline(is, t.description, delimiter);
		std::getline(is, t.command, delimiter);
		//is >> t.shortName >> delimiter >> t.serialPortName >> t.description >> t.command;
		return is;
	}
};

class SequenceItem {
public:
	SequenceItem(const std::string &shortname = "", int delay = 0) :
		taskShortName(shortname), delaySec(delay) {}
	//std::shared_ptr<Task> taskPtr;
	std::string taskShortName = "";
	int delaySec = 0;
	friend std::ostream& operator<<(std::ostream& os, const SequenceItem& sI) {
		os << sI.taskShortName << delimiter << sI.delaySec << delimiter;
		return os;
	}

	friend std::istream& operator>>(std::istream& is, SequenceItem& sI) {
		std::string line;
		std::getline(is, line);
		std::istringstream iss(line);
		std::getline(iss, sI.taskShortName, delimiter);
		iss >> sI.delaySec;
		return is;
	}
};

class SequenceListItem {
public:
	SequenceListItem(const std::string &name = "") : shortName(name) {};
	std::vector<SequenceItem> sequenceList;
	std::string shortName; //unique
	//
	friend std::ostream& operator<<(std::ostream& os, const SequenceListItem& slist) {
		for (const auto& s : slist.sequenceList) {
			os << s << std::endl;
		}
		return os;
	}

	friend std::istream& operator>>(std::istream& is, SequenceListItem& slist) {
		
		std::string line;
		/*
		std::getline(is, line);
		slist.shortName = line;
		*/
		SequenceItem sI;
		while (std::getline(is, line)) {
			if (line.empty())
				continue;
			std::istringstream iss(line);
			//getline(iss, sI.taskShortName, delimiter);
			iss >> sI;
			slist.sequenceList.push_back(sI);
		}
		return is;
	}
};
