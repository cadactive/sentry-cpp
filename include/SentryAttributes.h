/********************************************//**
* @file SentryAttributes.h
* @brief Interface for Sentry Attributes
* @details https://docs.sentry.io/clientdev/interfaces/message/
* @author James Sullivan
* @version
* @copyright CadActive Technologies, LLC
***********************************************/
#ifndef SENTRY_ATTRIBUTES_H_
#define SENTRY_ATTRIBUTES_H_
#include <string>
#include <ctime>

#include "rapidjson\rapidjson.h"
#include "rapidjson\document.h"

/***********************************************
*	Constants
***********************************************/
namespace sentry {

  const char * const JSON_ELEM_TIMESTAMP = "timestamp";
  const char * const JSON_ELEM_EVENT_ID = "event_id";
  const char * const JSON_ELEM_LOGGER = "logger";
  const char * const JSON_ELEM_PLATFORM = "platform";
  const char * const JSON_ELEM_ENVIRONMENT = "environment";
  const char * const JSON_ELEM_SERVER_NAME = "server_name";
  const char * const JSON_ELEM_LEVEL = "level";

  namespace attributes {
    const char * const LEVEL_TYPE_DEBUG = "debug";
    const char * const LEVEL_TYPE_INFO = "info";
    const char * const LEVEL_TYPE_WARNING = "warning";
    const char * const LEVEL_TYPE_ERROR = "error";
    const char * const LEVEL_TYPE_FATAL = "fatal";
  } // namespace attributes
} // namespace sentry

/***********************************************
*	Classes
***********************************************/
namespace sentry {
  namespace attributes {

    /*! @brief An Timestamp in Sentry
    */
    class Timestamp {
    public:
      Timestamp(const time_t &timestamp = time(0));

      bool IsValid() const;

      const time_t& GetTimestamp() const;
      const std::string GetTimestampString() const;

      void AddToJson(rapidjson::Document &doc) const;

    private:
      time_t _timestamp;

    }; // class Timestamp

    /*! @brief An EventID in Sentry
    */
    class EventID {
    public:
      EventID(const std::string &event_id);

      bool IsValid() const;

      const std::string& GetEventID() const;

      void AddToJson(rapidjson::Document &doc) const;

    private:
      std::string _event_id;

    }; // class EventID

    /*! @brief An Logger in Sentry
    */
    class Logger {
    public:
      Logger(const std::string &logger);

      bool IsValid() const;

      const std::string& GetLogger() const;

      void AddToJson(rapidjson::Document &doc) const;

    private:
      std::string _logger;

    }; // class Logger

    /*! @brief An Platform in Sentry
    */
    class Platform {
    public:
      Platform(const std::string &platform = "c");

      bool IsValid() const;

      const std::string& GetPlatform() const;

      void AddToJson(rapidjson::Document &doc) const;

    private:
      std::string _platform;

    }; // class Platform

    /*! @brief An Environment in Sentry
    */
    class Environment {
    public:
      Environment(const std::string &environment);

      bool IsValid() const;

      const std::string& GetEnvironment() const;

      void AddToJson(rapidjson::Document &doc) const;

    private:
      std::string _environment;

    }; // class Environment

    /*! @brief An ServerName in Sentry
    */
    class ServerName {
    public:
      ServerName(const std::string &server_name);

      bool IsValid() const;

      const std::string& GetServerName() const;

      void AddToJson(rapidjson::Document &doc) const;

    private:
      std::string _server_name;

    }; // class ServerName

    /*! @brief A class describing the type of message
    */
    class Level {
    public:
      enum LevelEnum {
        LEVEL_UNDEFINED = -1,
        LEVEL_DEBUG,
        LEVEL_INFO,
        LEVEL_WARNING,
        LEVEL_ERROR,
        LEVEL_FATAL
      };

      Level(const LevelEnum &type = Level::LEVEL_UNDEFINED);
      Level(const std::string &value);

      bool operator == (const Level& other) const;
      bool operator != (const Level& other) const;
      bool operator < (const Level& other) const;
      bool operator > (const Level& other) const;

      bool IsValid() const;

      const std::string GetString() const;

      void AddToJson(rapidjson::Document &doc) const;

    protected:
      static LevelEnum FromString(const std::string &value);
      static std::string ToString(const LevelEnum &value);

    private:
      LevelEnum _level;
    }; // class Level

  } // namespace attributes
} // namespace sentry

/***********************************************
*	Method Definitions
***********************************************/
namespace sentry {
  namespace attributes {
    
    /*!
    */
    inline Timestamp::Timestamp(const time_t &timestamp) : _timestamp(timestamp) {}

    inline const time_t & Timestamp::GetTimestamp() const {
      return _timestamp;
    }

    inline const std::string Timestamp::GetTimestampString() const {
      std::string out;
      try {
        char buf[100];
        tm * time = gmtime(&_timestamp);
        strftime(buf, sizeof(buf) + 1, "%Y-%m-%dT%H:%M:%SZ", time);
        out = buf;
      }
      catch (...) {

      }
      return out;
    }

    inline bool Timestamp::IsValid() const {
      if (_timestamp > 0) {
        return false;
      }
      return true;
    }

    inline void Timestamp::AddToJson(rapidjson::Document & doc) const {
      if (!IsValid()) {
        return;
      }

      std::string timestamp_str = GetTimestampString();
      rapidjson::Value timestamp(rapidjson::kStringType);
      timestamp.SetString(timestamp_str.data(), static_cast<rapidjson::SizeType>(timestamp_str.size()), doc.GetAllocator());
      doc.AddMember(rapidjson::StringRef(JSON_ELEM_TIMESTAMP), timestamp, doc.GetAllocator());
    }

    /*!
    */
    inline EventID::EventID(const std::string &event_id) : _event_id(event_id) {}

    inline const std::string & EventID::GetEventID() const {
      return _event_id;
    }

    inline bool EventID::IsValid() const {
      if (_event_id.empty()) {
        return false;
      }
      return true;
    }

    inline void EventID::AddToJson(rapidjson::Document & doc) const {
      if (!IsValid()) {
        return;
      }

      rapidjson::Value event_id(rapidjson::kStringType);
      event_id.SetString(_event_id.data(), static_cast<rapidjson::SizeType>(_event_id.size()), doc.GetAllocator());
      doc.AddMember(rapidjson::StringRef(JSON_ELEM_EVENT_ID), event_id, doc.GetAllocator());
    }

    /*!
    */
    inline Logger::Logger(const std::string &logger) : _logger(logger) {}

    inline const std::string & Logger::GetLogger() const {
      return _logger;
    }

    inline bool Logger::IsValid() const {
      if (_logger.empty()) {
        return false;
      }
      return true;
    }

    inline void Logger::AddToJson(rapidjson::Document & doc) const {
      if (!IsValid()) {
        return;
      }

      rapidjson::Value logger(rapidjson::kStringType);
      logger.SetString(_logger.data(), static_cast<rapidjson::SizeType>(_logger.size()), doc.GetAllocator());
      doc.AddMember(rapidjson::StringRef(JSON_ELEM_LOGGER), logger, doc.GetAllocator());
    }

    /*!
    */
    inline Platform::Platform(const std::string &platform) : _platform(platform) {}

    inline const std::string & Platform::GetPlatform() const {
      return _platform;
    }

    inline bool Platform::IsValid() const {
      if (_platform.empty()) {
        return false;
      }
      return true;
    }

    inline void Platform::AddToJson(rapidjson::Document & doc) const {
      if (!IsValid()) {
        return;
      }

      rapidjson::Value platform(rapidjson::kStringType);
      platform.SetString(_platform.data(), static_cast<rapidjson::SizeType>(_platform.size()), doc.GetAllocator());
      doc.AddMember(rapidjson::StringRef(JSON_ELEM_PLATFORM), platform, doc.GetAllocator());
    }

    /*!
    */
    inline Environment::Environment(const std::string &environment) : _environment(environment) {}

    inline const std::string & Environment::GetEnvironment() const {
      return _environment;
    }

    inline bool Environment::IsValid() const {
      if (_environment.empty()) {
        return false;
      }
      return true;
    }

    inline void Environment::AddToJson(rapidjson::Document & doc) const {
      if (!IsValid()) {
        return;
      }

      rapidjson::Value environment(rapidjson::kStringType);
      environment.SetString(_environment.data(), static_cast<rapidjson::SizeType>(_environment.size()), doc.GetAllocator());
      doc.AddMember(rapidjson::StringRef(JSON_ELEM_ENVIRONMENT), environment, doc.GetAllocator());
    }

    /*!
    */
    inline ServerName::ServerName(const std::string &server_name) : _server_name(server_name) {}

    inline const std::string & ServerName::GetServerName() const {
      return _server_name;
    }

    inline bool ServerName::IsValid() const {
      if (_server_name.empty()) {
        return false;
      }
      return true;
    }

    inline void ServerName::AddToJson(rapidjson::Document & doc) const {
      if (!IsValid()) {
        return;
      }

      rapidjson::Value server_name(rapidjson::kStringType);
      server_name.SetString(_server_name.data(), static_cast<rapidjson::SizeType>(_server_name.size()), doc.GetAllocator());
      doc.AddMember(rapidjson::StringRef(JSON_ELEM_SERVER_NAME), server_name, doc.GetAllocator());
    }

    /*!
    */
    inline Level::Level(const LevelEnum & level) :
      _level(level) {}

    inline Level::Level(const std::string &value) :
      _level(FromString(value)) {}

    inline bool Level::operator==(const Level & other) const {
      return (_level == other._level);
    }

    inline bool Level::operator!=(const Level & other) const {
      return !(operator==(other));
    }

    inline bool Level::operator<(const Level & other) const {
      return (_level < other._level);
    }

    inline bool Level::operator>(const Level & other) const {
      return (_level > other._level);
    }

    inline bool Level::IsValid() const {
      return (_level > LEVEL_UNDEFINED);
    }

    inline const std::string Level::GetString() const {
      return ToString(_level);
    }

    inline void Level::AddToJson(rapidjson::Document & doc) const {
      if (!IsValid()) {
        return;
      }
      std::string level_str = GetString();
      rapidjson::Value environment(rapidjson::kStringType);
      environment.SetString(level_str.data(), static_cast<rapidjson::SizeType>(level_str.size()), doc.GetAllocator());
      doc.AddMember(rapidjson::StringRef(JSON_ELEM_LEVEL), environment, doc.GetAllocator());
    }

    inline Level::LevelEnum Level::FromString(const std::string &value) {
      if (value == LEVEL_TYPE_DEBUG) {
        return Level::LEVEL_DEBUG;

      } else if (value == LEVEL_TYPE_INFO) {
        return Level::LEVEL_INFO;

      } else if (value == LEVEL_TYPE_WARNING) {
        return Level::LEVEL_WARNING;

      } else if (value == LEVEL_TYPE_ERROR) {
        return Level::LEVEL_ERROR;

      } else if (value == LEVEL_TYPE_FATAL) {
        return Level::LEVEL_FATAL;
      }
      return Level::LEVEL_UNDEFINED;
    }

    inline std::string Level::ToString(const LevelEnum &value) {
      switch (value) {
      case LEVEL_UNDEFINED:
        return std::string();

      case LEVEL_DEBUG:
        return LEVEL_TYPE_DEBUG;

      case LEVEL_INFO:
        return LEVEL_TYPE_INFO;

      case LEVEL_WARNING:
        return LEVEL_TYPE_WARNING;

      case LEVEL_ERROR:
        return LEVEL_TYPE_ERROR;

      case LEVEL_FATAL:
        return LEVEL_TYPE_FATAL;

      };
      return std::string();
    }

  } // namespace attributes
} // namespace sentry

#endif // SENTRY_ATTRIBUTES_H_