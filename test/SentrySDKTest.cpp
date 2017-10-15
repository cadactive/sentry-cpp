/********************************************//**
* @file SentrySDKTest.cpp
* @brief Testing for SentrySDK.h
* @details
* @author James Sullivan
* @version
* @copyright CadActive Technologies, LLC
***********************************************/
#include "SentrySDK.h"
#include <gtest\gtest.h>

using namespace Sentry;
using namespace rapidjson;

/***********************************************
*	Functions
***********************************************/
/*! @test Test a client
*/
TEST(SDK, Base) {
  SDK empty;
  EXPECT_EQ(true, empty.IsValid()); // empty should be valid
  EXPECT_EQ(false, empty.GetName().empty());
}

/*! @test Test JSON methods
*/
TEST(SDK, JSON) {
  SDK some;
  EXPECT_EQ(true, some.IsValid());

  rapidjson::Document json;
  some.ToJson(json);

  SDK some_json(json);
  EXPECT_EQ(true, some_json.IsValid());
  EXPECT_EQ(true, some_json.GetName() == some.GetName());
}