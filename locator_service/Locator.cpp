#include "Locator.h"
#include <stdexcept>
#include <regex>
#include <cmath>
#include <fstream>

Locator::Locator() {
	spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] [thread %t] %v");
	std::string logFileName = "locator_log.csv";
	if (!spdlog::get("file_logger")) {
		auto file_logger = spdlog::basic_logger_mt("file_logger", logFileName);
		spdlog::set_default_logger(file_logger);
	}
	try {
		ConfigurationOnStart();
	}
	catch (const std::exception& error) {
		SPDLOG_CRITICAL(error.what());
		throw;
	}
}

void Locator::SetLogLevel(std::string log_level) {
	spdlog::level::level_enum log_level_enum = logLevelMap.count(log_level) ? logLevelMap.at(log_level) : throw std::exception("Incorrect log level input.");
	spdlog::set_level(log_level_enum);
	SPDLOG_WARN("Log level has been changed to {}", log_level_enum);
}

bool Locator::ConfigurationOnStart() {
	std::ifstream cfg("config.json");
	if (!cfg.is_open()) {
		throw std::exception("Config file wasn't found. Program terminated.");
	}
	const json& Config = json::parse(cfg);
	const json& zones = Config["zones"];
	for (const auto& zone : zones) {
		unsigned int id = zone["id"];
		std::string name = zone["name"];
		int x = zone["x"];
		int y = zone["y"];
		int radius = zone["radius"];
		AddZone(id, name, x, y, radius);
	}
	std::string log_level = Config.value("log_level", "info");
	spdlog::level::level_enum log_level_enum = logLevelMap.count(log_level) ? logLevelMap.at(log_level) : spdlog::level::info;
	spdlog::set_level(log_level_enum);
	return 1;
}

Zone* Locator::GetZone(unsigned int id) {
	return (zones.find(id) != zones.end()) ? zones[id] : nullptr;
}

Subscriber* Locator::GetSubscriber(std::string id) {
	//return (subscribers.find(id) != subscribers.end()) ? subscribers[id] : nullptr;
	auto it = subscribers.find(id);
	if (it != subscribers.end()) {
		SPDLOG_DEBUG("Subscriber {} found in the map.", id);
		return it->second;
	}
	else {
		SPDLOG_DEBUG("Subscriber {} not found in the map.", id);
		return nullptr;
	}
}

void Locator::AddZone(unsigned int id, std::string name, int x, int y, int radius) {
	const unsigned int objid = id;
	if (GetZone(id) != nullptr) return;
	if (name.empty() && radius <= 0) return;
	zones[id] = new Zone(objid, name, x, y, radius);
	zones[id]->setId(objid);
	SPDLOG_INFO("Zone {} added ", id);
}

void Locator::AddSubscriber(std::string id, int x, int y) {
	const std::string objid = id;
	std::regex phoneRegex(R"(^\+7\d{10}$)");
	if (GetSubscriber(id) != nullptr) return;
	if (!std::regex_match(id, phoneRegex)) return;
	subscribers[id] = new Subscriber(objid, x, y);
	subscribers[id]->setId(objid);
	SPDLOG_INFO("Subscriber {} added", id);
	auto it = subscribers.find(id);
	if (it != subscribers.end()) {
		SPDLOG_DEBUG("Subscriber {} found in the map.", id);
	}
}

void Locator::SetSubscriberLocation(std::string id, int x, int y) {
	if (GetSubscriber(id) == nullptr) return;
	int oldX = subscribers[id]->getX(),
		oldY = subscribers[id]->getY();
	subscribers[id]->setCoordinates(x, y);
	CheckZoneTriggers(id, oldX, oldY);
	CheckProximityTriggers(id, oldX, oldY);
}

bool Locator::CheckIfInZone(Zone* zone, Subscriber* subscriber) {
	int zone_x = zone->getX(),
		zone_y = zone->getY(),
		zone_radius = zone->getRadius(),
		subscriber_x = subscriber->getX(),
		subscriber_y = subscriber->getY();
	int expr = (std::pow((subscriber_x - zone_x), 2)) + (std::pow((subscriber_y - zone_y), 2));
	return (expr <= std::pow(zone_radius, 2)) ? 1 : 0;
}

std::vector<Subscriber*> Locator::GetSubscribersInZone(unsigned int id) {
	std::vector<Subscriber*> result;
	if (zones.find(id) != zones.end()) {
		Zone* zone = zones[id];
		int zone_x = zone->getX();
		int zone_y = zone->getY();
		int zone_radius = zone->getRadius();
		for (const auto& subscriber : subscribers) {
			if (CheckIfInZone(zone, subscriber.second) == 1) 
				result.push_back(subscriber.second);
		}
	}
	return result;
}

std::vector<Zone*> Locator::GetZonesForSubscriber(std::string id) {
	std::vector<Zone*> result;
	if (subscribers.find(id) != subscribers.end()) {
		Subscriber* subscriber = subscribers[id];
		int subscriber_x = subscriber->getX();
		int subscriber_y = subscriber->getY();
		for (const auto& zone : zones) {
			if (CheckIfInZone(zone.second, subscriber) == 1)
				result.push_back(zone.second);
		}
	}
	return result;
}

void Locator::RemoveSubscriber(std::string id) {
	auto it = subscribers.find(id);
	if (it != subscribers.end()) {
		delete it->second;
		subscribers.erase(it);
		SPDLOG_TRACE("Subcsriber {} removed", id);
	}
}

void Locator::RemoveZone(unsigned int id) {
	auto it = zones.find(id);
	if (it != zones.end()) {
		delete it->second;
		zones.erase(it);
		SPDLOG_TRACE("Zone {} removed", id);
	}
}

void Locator::AddZoneTrigger(std::string id, std::string subscriber_id, unsigned int zone_id, Event event) {
	const std::string objid = id;
	if (id.empty() || GetSubscriber(subscriber_id) == nullptr || GetZone(zone_id) == nullptr) return;
	zoneTriggers[id] = new ZoneTrigger(id, subscriber_id, zone_id, event);
	zoneTriggers[id]->setId(objid);
	SPDLOG_INFO("Zone trigger {} added", id);
}

void Locator::AddProximityTrigger(std::string id, std::string subscriber1_id, std::string subscriber2_id, double distance) {
	const std::string objid = id;
	if (id.empty() || GetSubscriber(subscriber1_id) == nullptr || GetSubscriber(subscriber2_id) == nullptr) return;
	proximityTriggers[id] = new SubscribersProximityTrigger(id, subscriber1_id, subscriber2_id, distance);
	proximityTriggers[id]->setId(objid);
	SPDLOG_INFO("Proximity trigger {} added", id);
}

void Locator::RemoveZoneTrigger(std::string id) {
	auto it = zoneTriggers.find(id);
	if (it != zoneTriggers.end()) {
		delete it->second;
		zoneTriggers.erase(it);
		SPDLOG_TRACE("Zone trigger {} removed", id);
	}
}

void Locator::RemoveProximityTrigger(std::string id) {
	auto it = proximityTriggers.find(id);
	if (it != proximityTriggers.end()) {
		delete it->second;
		proximityTriggers.erase(it);
		SPDLOG_TRACE("Proximity trigger {} removed", id);
	}
}

ZoneTrigger* Locator::GetZoneTrigger(std::string id) {
	return (zoneTriggers.find(id) != zoneTriggers.end()) ? zoneTriggers[id] : nullptr;
}

SubscribersProximityTrigger* Locator::GetProximityTrigger(std::string id) {
	return (proximityTriggers.find(id) != proximityTriggers.end()) ? proximityTriggers[id] : nullptr;
}

std::vector<ZoneTrigger*> Locator::CheckZoneTriggers(std::string subscriber_id, int oldX, int oldY) {
	std::string id = subscriber_id;
	std::vector<ZoneTrigger*> triggered;
	Subscriber* subscriber = GetSubscriber(subscriber_id);
	Subscriber* oldTmp = new Subscriber(subscriber_id, oldX, oldY);
	oldTmp->setId(id);
	for (const auto& trigger : zoneTriggers) {
		if (trigger.second->getSubscriberId() == subscriber_id) {
			ZoneTrigger* trig = trigger.second;
			bool wasInZone = CheckIfInZone(GetZone(trig->getZoneId()), oldTmp);
			bool isInZone = CheckIfInZone(GetZone(trig->getZoneId()), subscriber);
			if (wasInZone && !isInZone &&
				(trig->getEvent() == EXIT_ZONE || trig->getEvent() == ENTER_OR_EXIT_ZONE)) {
				SPDLOG_TRACE("Zone exit (enter or exit) trigger {} has been triggered", trig->getId());
				triggered.push_back(trig);
			}
			else if (!wasInZone && isInZone &&
				(trig->getEvent() == ENTER_ZONE || trig->getEvent() == ENTER_OR_EXIT_ZONE)) {
				SPDLOG_TRACE("Zone enter (enter or exit) trigger {} has been triggered", trig->getId());
				triggered.push_back(trig);
			}
		}
	}
	delete oldTmp;
	return triggered;
}

std::vector<SubscribersProximityTrigger*> Locator::CheckProximityTriggers(std::string subscriber_id, int oldX, int oldY) {
	std::string id = subscriber_id;
	std::vector<SubscribersProximityTrigger*> triggered;
	Subscriber* subscriber = GetSubscriber(subscriber_id);
	Subscriber* oldTmp = new Subscriber(subscriber_id, oldX, oldY);
	oldTmp->setId(id);
	for (const auto& trigger : proximityTriggers) {
		if (trigger.second->getSubscriber1Id() == subscriber_id || trigger.second->getSubscriber2Id() == subscriber_id) {
			SubscribersProximityTrigger* trig = trigger.second;
			bool wasInProximity = CheckIfInProximity(oldTmp, GetSubscriber(trig->getSubscriber2Id()), trig->getDistance());
			bool isInProximity = CheckIfInProximity(subscriber, GetSubscriber(trig->getSubscriber2Id()), trig->getDistance());
			if (!wasInProximity && isInProximity) {
				SPDLOG_TRACE("Proximity trigger {} has been triggered", trig->getId());
				triggered.push_back(trig);
			}
		}
	}
	delete oldTmp;
	return triggered;
}

	bool Locator::CheckIfInProximity(Subscriber * subscriber1, Subscriber * subscriber2, double distance) {
		int s1_x = subscriber1->getX(),
			s1_y = subscriber1->getY(),
			s2_x = subscriber2->getX(),
			s2_y = subscriber2->getY();
		double expr = std::sqrt(std::pow((s2_x - s1_x), 2) + std::pow((s2_y - s1_y), 2));
		return (expr <= distance) ? 1 : 0;
	}