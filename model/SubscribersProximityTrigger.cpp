#include "SubscribersProximityTrigger.h"
#include <string>

SubscribersProximityTrigger::SubscribersProximityTrigger(std::string id, std::string subscriber1_id, std::string subscriber2_id, double distance) {
	this->id = id;
	this->subscriber1_id = subscriber1_id;
	this->subscriber2_id = subscriber2_id;
	this->distance = distance;
}

void SubscribersProximityTrigger::setId(std::string id) {
	this->id = id;
}

const std::string SubscribersProximityTrigger::getId() {
	return this->id;
}

void SubscribersProximityTrigger::setSubscriber1Id(std::string subscriber1_id) {
	this->subscriber1_id = subscriber1_id;
}

const std::string SubscribersProximityTrigger::getSubscriber1Id() {
	return this->subscriber1_id;
}

void SubscribersProximityTrigger::setSubscriber2Id(std::string subscriber2_id) {
	this->subscriber2_id = subscriber2_id;
}

const std::string SubscribersProximityTrigger::getSubscriber2Id() {
	return this->subscriber2_id;
}

void SubscribersProximityTrigger::setDistance(double distance) {
	this->distance = distance;
}

const double SubscribersProximityTrigger::getDistance() {
	return this->distance;
}