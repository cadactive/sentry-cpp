/********************************************//**
* @file SentryAttributesTest.cpp
* @brief Testing for SentryAttributes.h
* @details
* @author James Sullivan
* @version
* @copyright CadActive Technologies, LLC
***********************************************/
#include "SentryAttributes.h"
#include <gtest\gtest.h>

using namespace sentry;
using namespace sentry::attributes;
using namespace rapidjson;

/***********************************************
*	Functions
***********************************************/
/*! @test Test a client
*/
TEST(Timestamp, Base) {
  Timestamp timestamp;
  EXPECT_EQ(true, timestamp.IsValid()); // empty should be valid
  EXPECT_EQ(false, timestamp.GetTimestampString().empty());
}

/*! @test Test a client
*/
TEST(EventID, Base) {
  EventID event_id("blablabla");
  EXPECT_EQ(true, event_id.IsValid()); // empty should be valid
  EXPECT_EQ(false, event_id.GetEventID().empty());
}

/*! @test Test a client
*/
TEST(Logger, Base) {
  Logger logger("blablabla");
  EXPECT_EQ(true, logger.IsValid()); // empty should be valid
  EXPECT_EQ(false, logger.GetLogger().empty());
}

/*! @test Test a client
*/
TEST(Platform, Base) {
  Platform platform("blablabla");
  EXPECT_EQ(true, platform.IsValid()); // empty should be valid
  EXPECT_EQ(false, platform.GetPlatform().empty());
}

/*! @test Test a client
*/
TEST(Environment, Base) {
  Environment environment("blablabla");
  EXPECT_EQ(true, environment.IsValid()); // empty should be valid
  EXPECT_EQ(false, environment.GetEnvironment().empty());
}


/*! @test Test a client
*/
TEST(ServerName, Base) {
  ServerName server_name("blablabla");
  EXPECT_EQ(true, server_name.IsValid()); // empty should be valid
  EXPECT_EQ(false, server_name.GetServerName().empty());
}

/*! @test Test a client
*/
TEST(Level, Base) {
  Level empty;
  EXPECT_EQ(false, empty.IsValid());

  Level some(Level::LEVEL_INFO);
  EXPECT_EQ(true, some.IsValid());
  EXPECT_EQ(true, some > empty);

  Level copy(some);
  EXPECT_EQ(true, copy.IsValid());
  EXPECT_EQ(true, some == copy);
}