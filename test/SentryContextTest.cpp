/********************************************//**
* @file SentryContextTest.cpp
* @brief Testing for SentryContext.h
* @details
* @author James Sullivan
* @version
* @copyright CadActive Technologies, LLC
***********************************************/
#include "SentryContext.h"
#include <gtest\gtest.h>

using namespace sentry;
using namespace rapidjson;

/***********************************************
*	Functions
***********************************************/
/*! @test Test a client
*/
TEST(ContextGeneral, Base) {
  ContextGeneral empty;
  EXPECT_EQ(false, empty.IsValid());

  ContextGeneral some("abcd", "some_function");
  EXPECT_EQ(true, some.IsValid());

  ContextGeneral copy(some);
  EXPECT_EQ(true, copy.IsValid());
}

/*! @test Test JSON methods
*/
TEST(ContextGeneral, JSON) {
  ContextGeneral some("abcd", "some_function");
  EXPECT_EQ(true, some.IsValid());

  rapidjson::Document json;
  some.ToJson(json);

  ContextGeneral some_json(json);
  EXPECT_EQ(true, some_json.IsValid());
  EXPECT_EQ(true, some_json.GetName() == some.GetName());
}

/*! @test Test a client
*/
TEST(ContextOS, Base) {

  ContextOS some("windows", "10");
  EXPECT_EQ(true, some.IsValid());

  ContextOS copy(some);
  EXPECT_EQ(true, copy.IsValid());
}

/*! @test Test JSON methods
*/
TEST(ContextOS, JSON) {
  ContextOS some("windows", "10");
  EXPECT_EQ(true, some.IsValid());

  rapidjson::Document json;
  some.ToJson(json);

  ContextOS some_json(json);
  EXPECT_EQ(true, some_json.IsValid());
  EXPECT_EQ(true, some_json.GetName() == some.GetName());
  EXPECT_EQ(true, some_json.GetVersion() == some.GetVersion());
}


/*! @test Test a client
*/
TEST(ContextRuntime, Base) {

  ContextRuntime some("cadapp", "0.0.1.0");
  EXPECT_EQ(true, some.IsValid());

  ContextRuntime copy(some);
  EXPECT_EQ(true, copy.IsValid());
}

/*! @test Test JSON methods
*/
TEST(ContextRuntime, JSON) {
  ContextRuntime some("cadapp", "0.0.1.0");
  EXPECT_EQ(true, some.IsValid());

  rapidjson::Document json;
  some.ToJson(json);

  ContextRuntime some_json(json);
  EXPECT_EQ(true, some_json.IsValid());
  EXPECT_EQ(true, some_json.GetName() == some.GetName());
  EXPECT_EQ(true, some_json.GetVersion() == some.GetVersion());
}