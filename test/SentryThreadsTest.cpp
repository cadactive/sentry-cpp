/********************************************//**
* @file SentryThreadsTest.cpp
* @brief Testing for SentryThreads.h
* @details
* @author James Sullivan
* @version
* @copyright CadActive Technologies, LLC
***********************************************/
#include "SentryThreads.h"
#include <gtest\gtest.h>

using namespace Sentry;
using namespace rapidjson;

/***********************************************
*	Functions
***********************************************/
/*! @test Test a client
*/
TEST(Thread, Base) {
  Thread empty;
  EXPECT_EQ(false, empty.IsValid());

  Thread some(1, true, true);
  EXPECT_EQ(true, some.IsValid());

  EXPECT_EQ(false, some == empty);
  EXPECT_EQ(true, some != empty);
}

/*! @test Test JSON methods
*/
TEST(Thread, JSON) {
  Thread some(1, true, true);
  EXPECT_EQ(true, some.IsValid());

  rapidjson::Document json;
  some.ToJson(json);

  Thread some_json(json);
  EXPECT_EQ(true, some_json.IsValid());
  EXPECT_EQ(true, some_json.GetThreadID() == some.GetThreadID());
}

/*! @test Test a client
*/
TEST(Threads, Base) {
  Threads empty;
  EXPECT_EQ(false, empty.IsValid());

  std::vector<Thread> threads;
  threads.push_back(Thread(1, true, true));
  threads.push_back(Thread(2, false, false));

  Threads some(threads);
  EXPECT_EQ(true, some.IsValid());
}

/*! @test Test JSON methods
*/
TEST(Threads, JSON) {
  std::vector<Thread> threads;
  threads.push_back(Thread(1, true, true));
  threads.push_back(Thread(2, false, false));

  Threads some(threads);
  EXPECT_EQ(true, some.IsValid());

  rapidjson::Document json;
  some.ToJson(json);

  Threads some_json(json);
  EXPECT_EQ(true, some_json.IsValid());
  EXPECT_EQ(true, some_json.GetThreads().size() == some.GetThreads().size());
  EXPECT_EQ(true, some_json.GetThreads().at(1).GetThreadID() == some.GetThreads().at(1).GetThreadID());
}