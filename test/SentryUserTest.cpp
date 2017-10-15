/********************************************//**
* @file SentryUserTest.cpp
* @brief Testing for SentryUser.h
* @details
* @author James Sullivan
* @version
* @copyright CadActive Technologies, LLC
***********************************************/
#include "SentryUser.h"
#include <gtest\gtest.h>

using namespace Sentry;
using namespace rapidjson;

/***********************************************
*	Functions
***********************************************/
/*! @test Test a client
*/
TEST(User, Base) {
  User empty;
  EXPECT_EQ(false, empty.IsValid());

  User some("abcd", "email@email.com", "username");
  EXPECT_EQ(true, some.IsValid());

  User copy(some);
  EXPECT_EQ(true, copy.IsValid());
}

/*! @test Test JSON methods
*/
TEST(User, JSON) {
  User some("abcd", "email@email.com", "username");
  EXPECT_EQ(true, some.IsValid());

  rapidjson::Document json;
  some.ToJson(json);

  User some_json(json);
  EXPECT_EQ(true, some_json.IsValid());
  EXPECT_EQ(true, some_json.GetEmail() == some.GetEmail());
}