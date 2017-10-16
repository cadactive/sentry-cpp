/********************************************//**
* @file SentryStacktraceTest.cpp
* @brief Testing for SentryStacktrace.h
* @details
* @author James Sullivan
* @version
* @copyright CadActive Technologies, LLC
***********************************************/
#include "SentryStacktrace.h"
#include <gtest\gtest.h>

using namespace sentry;
using namespace rapidjson;

/***********************************************
*	Functions
***********************************************/
/*! @test Test a client
*/
TEST(Stacktrace, Base) {
  Stacktrace empty;
  EXPECT_EQ(false, empty.IsValid());

  std::vector<Frame> frames;
  frames.push_back(Frame("abcd", "some_function"));
  frames.push_back(Frame("efgh", "other_function"));

  Stacktrace some(frames);
  EXPECT_EQ(true, some.IsValid());
}

/*! @test Test JSON methods
*/
TEST(Stacktrace, JSON) {
  std::vector<Frame> frames;
  frames.push_back(Frame("abcd", "some_function"));
  frames.push_back(Frame("efgh", "other_function"));

  Stacktrace some(frames);
  EXPECT_EQ(true, some.IsValid());

  rapidjson::Document json;
  some.ToJson(json);

  Stacktrace some_json(json);
  EXPECT_EQ(true, some_json.IsValid());
  EXPECT_EQ(true, some_json.GetFrames().size() == some.GetFrames().size());
  const std::string& some_json_function = some_json.GetFrames().at(1).GetFunction();
  const std::string& some_function = some.GetFrames().at(1).GetFunction();
  EXPECT_EQ(true, some_function == some_json_function);
}