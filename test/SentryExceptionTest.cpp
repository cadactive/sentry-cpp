/********************************************//**
* @file SentryExceptionTest.cpp
* @brief Testing for SentryException.h
* @details
* @author James Sullivan
* @version
* @copyright CadActive Technologies, LLC
***********************************************/
#include "SentryException.h"
#include <gtest\gtest.h>

using namespace Sentry;
using namespace rapidjson;

/***********************************************
*	Functions
***********************************************/
/*! @test Test a client
*/
TEST(Exception, Base) {
  Exception empty;
  EXPECT_EQ(false, empty.IsValid());

  Exception some("abcd", "EXCEPTION_FOUND", "module");
  EXPECT_EQ(true, some.IsValid());

  Exception copy(some);
  EXPECT_EQ(true, copy.IsValid());
}

/*! @test Test JSON methods
*/
TEST(Exception, JSON) {
  Exception some("abcd", "EXCEPTION_FOUND", "module");
  EXPECT_EQ(true, some.IsValid());

  rapidjson::Document json;
  some.ToJson(json);

  Exception some_json(json);
  EXPECT_EQ(true, some_json.IsValid());
  EXPECT_EQ(true, some_json.GetValue() == some.GetValue());
}