// Maz_Protei_locator_test.cpp: определяет точку входа для приложения.
//

#include "Maz_Protei_locator.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>
#include <gtest/gtest.h>


using json = nlohmann::json;

TEST(LocatorTest, AddZone) {
    Locator locator;
    locator.AddZone(1, "TestZone", 10, 20, 30);
    Zone* addedZone = locator.GetZone(1);
    ASSERT_NE(addedZone, nullptr);
    EXPECT_EQ(addedZone->getName(), "TestZone");
}

TEST(LocatorTest, GetTest) {
    Locator locator;
    locator.AddSubscriber("+79117614222", 0, 0);
    auto subscriber = locator.GetSubscriber("+79117614222");
    ASSERT_TRUE(subscriber->getId() == "+79117614222");
}

TEST(LocatorTest, GetTestUnknown) {
    Locator locator;
    auto subscriber = locator.GetSubscriber("unknown");
    ASSERT_TRUE(!subscriber);
}

TEST(LocatorTest, SetSubscriberLocation) {
    Locator locator;
    locator.AddSubscriber("+71234567890", 10, 20);
    locator.SetSubscriberLocation("+71234567890", 30, 40);
    Subscriber* updatedSubscriber = locator.GetSubscriber("+71234567890");
    ASSERT_NE(updatedSubscriber, nullptr);
    ASSERT_EQ(updatedSubscriber->getX(), 30);
    ASSERT_EQ(updatedSubscriber->getY(), 40);
}

TEST(LocatorTest, RemoveSubscriber) {
    Locator locator;
    locator.AddSubscriber("+71234567890", 10, 20);
    locator.RemoveSubscriber("+71234567890");
    Subscriber* removedSubscriber = locator.GetSubscriber("+71234567890");
    EXPECT_EQ(removedSubscriber, nullptr);
}

TEST(LocatorTest, RemoveZone) {
    Locator locator;
    locator.AddZone(1, "TestZone", 10, 20, 30);
    locator.RemoveZone(1);
    Zone* removedZone = locator.GetZone(1);
    EXPECT_EQ(removedZone, nullptr);
}

TEST(LocatorTest, AddZoneTrigger) {
    Locator locator;
    locator.AddZone(1, "TestZone", 10, 20, 30);
    locator.AddSubscriber("+71234567890", 15, 25);
    locator.AddZoneTrigger("Trigger1", "+71234567890", 1, ENTER_ZONE);
    ZoneTrigger* addedTrigger = locator.GetZoneTrigger("Trigger1");
    ASSERT_NE(addedTrigger, nullptr);
    EXPECT_EQ(addedTrigger->getEvent(), ENTER_ZONE);
}

TEST(LocatorTest, AddProximityTrigger) {
    Locator locator;
    locator.AddSubscriber("+71234567890", 10, 20);
    locator.AddSubscriber("+76543210987", 15, 25);
    locator.AddProximityTrigger("Trigger2", "+71234567890", "+76543210987", 10.0);
    SubscribersProximityTrigger* addedTrigger = locator.GetProximityTrigger("Trigger2");
    ASSERT_NE(addedTrigger, nullptr);
    EXPECT_EQ(addedTrigger->getDistance(), 10.0);
}

TEST(LocatorTest, RemoveZoneTrigger) {
    Locator locator;
    locator.AddZone(1, "TestZone", 10, 20, 30);
    locator.AddSubscriber("+71234567890", 15, 25);
    locator.AddZoneTrigger("Trigger1", "+71234567890", 1, ENTER_ZONE);
    locator.RemoveZoneTrigger("Trigger1");
    ZoneTrigger* removedTrigger = locator.GetZoneTrigger("Trigger1");
    EXPECT_EQ(removedTrigger, nullptr);
}

TEST(LocatorTest, RemoveProximityTrigger) {
    Locator locator;
    locator.AddSubscriber("+71234567890", 10, 20);
    locator.AddSubscriber("+76543210987", 15, 25);
    locator.AddProximityTrigger("Trigger2", "+71234567890", "+76543210987", 10.0);
    locator.RemoveProximityTrigger("Trigger2");
    SubscribersProximityTrigger* removedTrigger = locator.GetProximityTrigger("Trigger2");
    EXPECT_EQ(removedTrigger, nullptr);
}

TEST(LocatorTest, ZoneTriggerActivation) {
    Locator locator;
    locator.AddZone(1, "TestZone", 0, 0, 100);
    locator.AddSubscriber("+71234567890", 125, 125);
    locator.AddZoneTrigger("EnterZoneTrigger", "+71234567890", 1, ENTER_ZONE);
    locator.SetSubscriberLocation("+71234567890", 50, 50);
    std::vector<ZoneTrigger*> &triggeredTriggers = locator.CheckZoneTriggers("+71234567890", 125, 125);
    ASSERT_EQ(triggeredTriggers.size(), 1);
    EXPECT_EQ(triggeredTriggers[0]->getEvent(), ENTER_ZONE);
}

TEST(LocatorTest, ProximityTriggerActivation) {
    Locator locator;
    locator.AddSubscriber("+71234567890", 100, 100);
    locator.AddSubscriber("+76543210987", 15, 25);
    locator.AddProximityTrigger("ProximityTrigger", "+71234567890", "+76543210987", 10.0);
    locator.SetSubscriberLocation("+71234567890", 10, 20);
    locator.SetSubscriberLocation("+76543210987", 15, 25);
    std::vector<SubscribersProximityTrigger*> &triggeredTriggers = locator.CheckProximityTriggers("+71234567890", 100, 100);
    ASSERT_EQ(triggeredTriggers.size(), 1);
    EXPECT_EQ(triggeredTriggers[0]->getDistance(), 10.0);
}

TEST(LocatorTest, ConfigTest) {
    Locator locator;
    auto zone = locator.GetZone(5);
    ASSERT_EQ(zone->getName(), "msk");
}


int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
