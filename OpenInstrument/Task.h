#pragma once
#include <string>
#include <memory>
#include <vector>
#include <iostream>

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
		os << t.shortName << " " << t.serialPortName << " "
			<< t.description << " " << t.command << "\n";
		return os;
	}

	friend std::istream& operator>>(std::istream& is, Task& t) {
		is >> t.shortName >> t.serialPortName >> t.description >> t.command;
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
		os << sI.taskShortName << " " << sI.delaySec << "\n";
		return os;
	}

	friend std::istream& operator>>(std::istream& is, SequenceItem& sI) {
		is >> sI.taskShortName >> sI.delaySec;
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
		os << slist.shortName << " " << slist.sequenceList.size() << " ";
		for (const auto& s : slist.sequenceList) {
			os << s << " ";
		}
		os << "\n";
		return os;
	}

	friend std::istream& operator>>(std::istream& is, SequenceListItem& slist) {
		size_t size;
		is >> slist.shortName >> size;
		slist.sequenceList.clear();
		SequenceItem sI;
		for (size_t i = 0; i < size; ++i) {
			is >> sI;
			slist.sequenceList.push_back(sI);
		}
		return is;
	}
};
