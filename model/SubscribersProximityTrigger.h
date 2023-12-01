#pragma once
#include <string>

class SubscribersProximityTrigger {
private:
	std::string id;
	std::string subscriber1_id, subscriber2_id;
	double distance;
public:
	SubscribersProximityTrigger(std::string id, std::string subscriber1_id, std::string subscriber2_id, double distance);
	void setId(std::string id);
	const std::string getId();
	void setSubscriber1Id(std::string subscriber1_id);
	const std::string getSubscriber1Id();
	void setSubscriber2Id(std::string subscriber2_id);
	const std::string getSubscriber2Id();
	void setDistance(double distsnce);
	const double getDistance();
};