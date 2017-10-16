/********************************************//**
* @file SentryFrameTest.cpp
* @brief Testing for SentryFrame.h
* @details
* @author James Sullivan
* @version
* @copyright CadActive Technologies, LLC
***********************************************/
#include "SentryFrame.h"
#include <gtest\gtest.h>

using namespace sentry;
using namespace rapidjson;

/***********************************************
*	Functions
***********************************************/
/*! @test Test a client
*/
TEST(Frame, Base) {
  Frame empty;
  EXPECT_EQ(false, empty.IsValid());

  Frame some("abcd", "some_function");
  EXPECT_EQ(true, some.IsValid());

  Frame copy(some);
  EXPECT_EQ(true, copy.IsValid());
}

/*! @test Test JSON methods
*/
TEST(Frame, JSON) {
  Frame some("abcd", "some_function");
  EXPECT_EQ(true, some.IsValid());

  rapidjson::Document json;
  some.ToJson(json);

  Frame some_json(json);
  EXPECT_EQ(true, some_json.IsValid());
  EXPECT_EQ(true, some_json.GetFunction() == some.GetFunction());
}