/********************************************//**
* @file SentryMessageTest.cpp
* @brief Testing for SentryMessage.h
* @details
* @author James Sullivan
* @version
* @copyright CadActive Technologies, LLC
***********************************************/
#include "SentryMessage.h"
#include <gtest\gtest.h>

using namespace Sentry;
using namespace rapidjson;

/***********************************************
*	Functions
***********************************************/
/*! @test Test a client
*/
TEST(MessageLevel, Base) {
  MessageLevel empty;
  EXPECT_EQ(false, empty.IsValid());

  MessageLevel some(MessageLevel::MESSAGE_INFO);
  EXPECT_EQ(true, some.IsValid());
  EXPECT_EQ(true, some > empty);

  MessageLevel copy(some);
  EXPECT_EQ(true, copy.IsValid());
  EXPECT_EQ(true, some == copy);
}

/*! @test Test a client
*/
TEST(Message, Base) {
  Message empty;
  EXPECT_EQ(false, empty.IsValid());

  Message some("abcd", "EXCEPTION_FOUND", MessageLevel::MESSAGE_INFO);
  EXPECT_EQ(true, some.IsValid());

  Message copy(some);
  EXPECT_EQ(true, copy.IsValid());
}

/*! @test Test JSON methods
*/
TEST(Message, JSON) {
  Message some("abcd", "EXCEPTION_FOUND", MessageLevel::MESSAGE_INFO);
  EXPECT_EQ(true, some.IsValid());

  rapidjson::Document json;
  some.ToJson(json);
  
  Message some_json(json);
  EXPECT_EQ(true, some_json.IsValid());
  EXPECT_EQ(true, some_json.GetLevel() == some.GetLevel());
  EXPECT_EQ(true, some_json.GetMessage() == some.GetMessage());
}