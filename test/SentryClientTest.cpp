/********************************************//**
* @file SentryClientTest.cpp
* @brief Testing for SentryClient.h
* @details 
* @author James Sullivan
* @version
* @copyright CadActive Technologies, LLC
***********************************************/
#include "SentryClient.h"
#include <gtest\gtest.h>

using namespace sentry;
using namespace rapidjson;

/***********************************************
*	Functions
***********************************************/
/*! @test Test a DSN
*/
TEST(DSN, Base) {
  DSN bad("some_string");
  EXPECT_EQ(false, bad.IsValid());

  DSN good("https://public:secret@sentry.example.com/1");
  EXPECT_EQ(true, good.IsValid());
}

TEST(DSN, URL) {
  DSN good("https://public:secret@sentry.example.com/1");
  EXPECT_EQ(true, good.IsValid());
  EXPECT_EQ(true, good.GetUrl() == "https://sentry.example.com/api/1/store/");
  EXPECT_EQ(true, good.GetPublicKey() == "public");
  EXPECT_EQ(true, good.GetSecretKey() == "secret");
  EXPECT_EQ(true, good.GetProjectID() == "1");
}

/*! @test Test JSON methods
*/
TEST(Client, Timestamp) {
  std::string time = Client::GenerateTimestampString(std::time(nullptr));
  EXPECT_EQ(false, time.empty());
}

/*! @test Test JSON methods
*/
TEST(Client, JSON) {
  DSN bad("some_string");
  Client bad_client(bad);
  EXPECT_EQ(false, bad_client.IsEnabled());

  DSN good("https://public:secret@sentry.example.com/1");
  Client good_client(good);
  EXPECT_EQ(true, good_client.IsEnabled());

  std::string authentication = good_client.GenerateAuthentication();
  EXPECT_EQ(false, authentication.empty());
}