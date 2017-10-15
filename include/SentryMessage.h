/********************************************//**
* @file SentryMessage.h
* @brief Interface for Sentry Messages
* @details https://docs.sentry.io/clientdev/interfaces/message/
* @author James Sullivan
* @version
* @copyright CadActive Technologies, LLC
***********************************************/
#ifndef SENTRY_MESSAGE_H_
#define SENTRY_MESSAGE_H_
#include <map>

#include "rapidjson\rapidjson.h"
#include "rapidjson\document.h"

/***********************************************
*	Constants
***********************************************/
namespace Sentry {

  const char * const JSON_ELEM_MESSAGE = "message";
  const char * const JSON_ELEM_FORMAT_PARAMS = "params";
  const char * const JSON_ELEM_LEVEL = "level";

  const char * const MESSAGE_TYPE_DEBUG = "debug";
  const char * const MESSAGE_TYPE_INFO = "info";
  const char * const MESSAGE_TYPE_WARNING = "warning";
  const char * const MESSAGE_TYPE_ERROR = "error";
  const char * const MESSAGE_TYPE_FATAL = "fatal";

} // namespace Sentry

/***********************************************
*	Classes
***********************************************/
namespace Sentry {

  /*! @brief A class describing the type of message
  */
  class MessageLevel {
  public:
    enum MessageLevelEnum {
      MESSAGE_UNDEFINED = -1,
      MESSAGE_DEBUG,
      MESSAGE_INFO,
      MESSAGE_WARNING,
      MESSAGE_ERROR,
      MESSAGE_FATAL
    };

    MessageLevel(const MessageLevelEnum &type = MessageLevel::MESSAGE_UNDEFINED);
    MessageLevel(const std::string &value);

    bool IsValid() const;

    const std::string GetString() const;

  protected:
    static MessageLevelEnum FromString(const std::string &value);
    static std::string ToString(const MessageLevelEnum &value);

  private:
    MessageLevelEnum _level;
  }; // class MessageLevel

  /*! @brief An Message in Sentry
  */
  class Message {
  public:
    Message();
    Message(const std::string &message, const std::string &format_params, const MessageLevel &message_level = MessageLevel::MESSAGE_UNDEFINED);
    Message(const rapidjson::Value &json);

    const std::string& GetMessage() const;
    const std::string& GetFormatParams() const;
    const std::map<std::string, std::string>& GetAdditionalFields() const;

    void ToJson(rapidjson::Document &doc);

  protected:
    void FromJson(const rapidjson::Value &json);

  private:
    std::string _title;
    MessageLevel _level;
    std::string _message;
    std::string _format_params;
    std::map<std::string, std::string> _additional_fields;

  }; // class Message

} // namespace Sentry

/***********************************************
*	Method Definitions
***********************************************/
namespace Sentry {
  /*!
  */
  inline MessageLevel::MessageLevel(const MessageLevelEnum & level) :
    _level(level) {}

  inline MessageLevel::MessageLevel(const std::string &value) :
    _level(FromString(value)) {}

  inline bool MessageLevel::IsValid() const {
    return (_level > MESSAGE_UNDEFINED);
  }

  inline const std::string MessageLevel::GetString() const {
    return ToString(_level);
  }

  inline MessageLevel::MessageLevelEnum MessageLevel::FromString(const std::string &value) {
    if (value == MESSAGE_TYPE_DEBUG) {
      return MessageLevel::MESSAGE_DEBUG;

    } else if (value == MESSAGE_TYPE_INFO) {
      return MessageLevel::MESSAGE_INFO;

    } else if (value == MESSAGE_TYPE_WARNING) {
      return MessageLevel::MESSAGE_WARNING;

    } else if (value == MESSAGE_TYPE_ERROR) {
      return MessageLevel::MESSAGE_ERROR;

    } else if (value == MESSAGE_TYPE_FATAL) {
      return MessageLevel::MESSAGE_FATAL;
    }
    return MessageLevel::MESSAGE_UNDEFINED;
  }

  inline std::string MessageLevel::ToString(const MessageLevelEnum &value) {
    switch (value) {
    case MESSAGE_UNDEFINED:
      return std::string();

    case MESSAGE_DEBUG:
      return MESSAGE_TYPE_DEBUG;

    case MESSAGE_INFO:
      return MESSAGE_TYPE_INFO;

    case MESSAGE_WARNING:
      return MESSAGE_TYPE_WARNING;

    case MESSAGE_ERROR:
      return MESSAGE_TYPE_ERROR;

    case MESSAGE_FATAL:
      return MESSAGE_TYPE_FATAL;

    };
    return std::string();
  }

  /*!
  */
  inline Message::Message() {}

  inline Message::Message(const std::string &message, const std::string &format_params, const MessageLevel &level) :
    _message(message), _format_params(format_params), _level(level) {
  
  }

  inline const std::string & Message::GetMessage() const {
    return _message;
  }

  inline const std::string & Message::GetFormatParams() const {
    return _message;
  }

  inline const std::map<std::string, std::string>& Message::GetAdditionalFields() const {
    return _additional_fields;
  }

  /*!
  */
  inline Message::Message(const rapidjson::Value &json) {
    FromJson(json);
  }

  /*! @brief Construct from a JSON object
  */
  inline void Message::FromJson(const rapidjson::Value & json) {
    if (json.IsNull()) { return; }
    if (!json.IsObject()) { return; }

    // Looping so that we find "everything"
    for (rapidjson::Value::ConstMemberIterator member = json.MemberBegin(); member != json.MemberEnd(); ++member) {

      if (strcmp(member->name.GetString(), JSON_ELEM_MESSAGE) == 0) {
        if (!member->value.IsNull()) {
          if (member->value.IsString()) {
            _message = member->value.GetString();
          }
        }
      } else if (strcmp(member->name.GetString(), JSON_ELEM_FORMAT_PARAMS) == 0) {
        if (!member->value.IsNull()) {
          if (member->value.IsString()) {
            _format_params = member->value.GetString();
          }
        }
      } else {
        if (strlen(member->name.GetString()) > 0) {
          if (member->value.IsString()) {
            _additional_fields[member->name.GetString()] = member->value.GetString();
          }
        }
      }
    }
  }

  /*! @brief Convert to a JSON object
  */
  inline void Message::ToJson(rapidjson::Document &doc) {
    doc.SetObject();
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

    if (!_message.empty()) {
      rapidjson::Value message(rapidjson::kStringType);
      message.SetString(_message.data(), static_cast<rapidjson::SizeType>(_message.size()), allocator);
      doc.AddMember(rapidjson::StringRef(JSON_ELEM_MESSAGE), message, allocator);
    }

    if (!_format_params.empty()) {
      rapidjson::Value format_params(rapidjson::kStringType);
      format_params.SetString(_format_params.data(), static_cast<rapidjson::SizeType>(_format_params.size()), allocator);
      doc.AddMember(rapidjson::StringRef(JSON_ELEM_FORMAT_PARAMS), format_params, allocator);
    }

    if (!_level.IsValid()) {
      rapidjson::Value level(rapidjson::kStringType);
      const std::string level_str = _level.GetString();
      level.SetString(level_str.data(), static_cast<rapidjson::SizeType>(level_str.size()), allocator);
      doc.AddMember(rapidjson::StringRef(JSON_ELEM_LEVEL), level, allocator);
    }

    for (auto additional = _additional_fields.begin(); additional != _additional_fields.end(); ++additional) {
      rapidjson::Value key(rapidjson::kStringType);
      key.SetString(additional->first.data(), static_cast<rapidjson::SizeType>(additional->first.size()), allocator);

      rapidjson::Value value(rapidjson::kStringType);
      value.SetString(additional->second.data(), static_cast<rapidjson::SizeType>(additional->second.size()), allocator);

      doc.AddMember(key, value, allocator);
    }
  }

} // namespace Sentry

#endif // SENTRY_MESSAGE_H_