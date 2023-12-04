#pragma once

#include "../model/Subscriber.h"
#include "../model/Zone.h"
#include "../model/ZoneTrigger.h"
#include "../model/SubscribersProximityTrigger.h"
#include <unordered_map>
#include <fstream>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

using json = nlohmann::json;

class Locator {
public:
	Locator();
	void AddZone(unsigned int id, std::string name, int x, int y, int radius);
	void AddSubscriber(std::string id, int x, int y);
	void SetSubscriberLocation(std::string id, int x, int y);
	Subscriber* GetSubscriber(std::string id);
	Zone* GetZone(unsigned int id);
	ZoneTrigger* GetZoneTrigger(std::string id);
	SubscribersProximityTrigger* GetProximityTrigger(std::string id);
	std::vector<Subscriber*> GetSubscribersInZone(unsigned int id);
	std::vector<Zone*> GetZonesForSubscriber(std::string id);
	void AddZoneTrigger(std::string id, std::string subcriber_id, unsigned int zone_id, Event event);
	void AddProximityTrigger(std::string id, std::string subscriber1_id, std::string subscriber2_id, double distance);
	void RemoveSubscriber(std::string id);
	void RemoveZone(unsigned int id);
	void RemoveZoneTrigger(std::string id);
	void RemoveProximityTrigger(std::string id);
	std::vector <ZoneTrigger*> CheckZoneTriggers(std::string subscriber_id, int oldX, int oldY);
	std::vector<SubscribersProximityTrigger*> CheckProximityTriggers(std::string subscriber_id, int oldX, int oldY);
	bool CheckIfInZone(Zone* zone, Subscriber* subscriber);
	bool CheckIfInProximity(Subscriber* subscriber1, Subscriber* subscriber2, double distance);
	bool ConfigurationOnStart();
	void SetLogLevel(std::string log_level);


private:
	std::string log_level;
	std::unordered_map<std::string, Subscriber*> subscribers;
	std::unordered_map<unsigned int, Zone*> zones;
	std::unordered_map<std::string, ZoneTrigger*> zoneTriggers;
	std::unordered_map<std::string, SubscribersProximityTrigger*> proximityTriggers;
	const std::unordered_map<std::string, spdlog::level::level_enum> logLevelMap = {
	{"trace", spdlog::level::trace},
	{"debug", spdlog::level::debug},
	{"info", spdlog::level::info},
	{"warn", spdlog::level::warn},
	{"error", spdlog::level::err},
	{"off", spdlog::level::off},
	};
};