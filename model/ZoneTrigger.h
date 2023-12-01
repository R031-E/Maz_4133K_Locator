#pragma once
#include <string>

enum Event {
	ENTER_ZONE,
	EXIT_ZONE,
	ENTER_OR_EXIT_ZONE
};

class ZoneTrigger {
private:
	std::string id;
	std::string subscriber_id;
	unsigned int zone_id;
	Event event;
public:
	ZoneTrigger(std::string id, std::string subscriber_id, unsigned int zone_id, Event event);
	void setId(std::string id);
	std::string getId();
	void setSubscriberId(std::string subscriber_id);
	std::string getSubscriberId();
	void setZoneId(unsigned int zone_id);
	const unsigned int getZoneId();
	void setEvent(Event event);
	Event getEvent();
};