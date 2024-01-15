#pragma once

#include <string>
#include <memory>

class Device;
class Sender;
class Stimulus
{
public:
	Stimulus(){}
	Stimulus(const Stimulus & other) {
		m_devicePtr = other.m_devicePtr;
	}
	virtual bool configure(std::shared_ptr<Sender> senderPtr) {
		return false;

	}
	virtual bool deliver(std::shared_ptr<Sender> senderPtr) {
		return false;
	}
	virtual bool bind(std::shared_ptr<Device> devicePtr)
	{
		m_devicePtr = devicePtr;
		return true;
	}
	virtual std::string toString() { return "Stimulus"; }
	~Stimulus() {}
private:
	std::shared_ptr<Device> m_devicePtr;
};
