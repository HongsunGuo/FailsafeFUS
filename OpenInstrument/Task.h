#pragma once
#include <string>
#include <memory>
#include <vector>

class Task
{
public:
	Task() {}
	std::string shortName; //unique
	std::string serialPortName;
	std::string description;
	std::string command;
};

class SequenceItem {
public:
	SequenceItem(std::shared_ptr<Task> ptr, int delay) :
		taskPtr(ptr), delaySec(delay) {}
	std::shared_ptr<Task> taskPtr;
	int delaySec = 0;
};


class SequenceListItem {
public:
	SequenceListItem() {};
	std::shared_ptr<std::vector<SequenceItem>> sequenceListPtr;
	std::string shortName; //unique
};
