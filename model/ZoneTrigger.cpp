#include "ZoneTrigger.h"
#include <string>

ZoneTrigger::ZoneTrigger(std::string id, std::string subscriber_id, unsigned int zone_id, Event event) {
	this->id = id;
	this->subscriber_id = subscriber_id;
	this->zone_id = zone_id;
	this->event = event;
}

void ZoneTrigger::setId(std::string id) {
	this->id = id;
}

std::string ZoneTrigger::getId() {
	return this->id;
}

void ZoneTrigger::setSubscriberId(std::string subscriber_id) {
	this->subscriber_id = subscriber_id;
}

std::string ZoneTrigger::getSubscriberId() {
	return this->subscriber_id;
}

void ZoneTrigger::setZoneId(unsigned int zone_id) {
	this->zone_id = zone_id;
}

const unsigned int ZoneTrigger::getZoneId() {
	return this->zone_id;
}

void ZoneTrigger::setEvent(Event event) {
	this->event = event;
}

Event ZoneTrigger::getEvent() {
	return this->event;
}