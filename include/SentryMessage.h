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
#include <string>
#include <iostream>
#include <map>
#include "SentryAttributes.h"

#include "rapidjson\rapidjson.h"
#include "rapidjson\document.h"

/***********************************************
*	Constants
***********************************************/
namespace sentry {

  const char * const JSON_ELEM_MESSAGE = "message";
  const char * const JSON_ELEM_FORMAT_PARAMS = "params";

} // namespace sentry

/***********************************************
*	Classes
***********************************************/
namespace sentry {

  /*! @brief An Message in Sentry
  */
  class Message {
  public:
    Message();
    Message(const std::string &message, const  attributes::Level &message_level = attributes::Level::LEVEL_UNDEFINED);
    Message(const std::string &message, const std::string &format_params, const  attributes::Level &message_level = attributes::Level::LEVEL_UNDEFINED);
    Message(const rapidjson::Value &json);

    bool IsValid() const;

    const attributes::Level& GetLevel() const;
    const std::string& GetMessage() const;
    const std::string& GetFormatParams() const;
    const std::map<std::string, std::string>& GetAdditionalFields() const;
    void SetAdditionalFields(const std::map<std::string, std::string>& additional_fields);

    void ToJson(rapidjson::Document &doc) const;

  protected:
    void FromJson(const rapidjson::Value &json);

  private:
    std::string _title;
    attributes::Level _level;
    std::string _message;
    std::string _format_params;
    std::map<std::string, std::string> _additional_fields;

  }; // class Message

} // namespace sentry

/***********************************************
*	Method Definitions
***********************************************/
namespace sentry {

  /*!
  */
  inline Message::Message() {}

  inline Message::Message(const std::string & message, const attributes::Level & message_level) :
    _message(message), _level(message_level) {

  }

  inline Message::Message(const std::string &message, const std::string &format_params, const attributes::Level &level) :
    _message(message), _format_params(format_params), _level(level) {
  
  }

  inline const attributes::Level & Message::GetLevel() const {
    return _level;
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

  inline void Message::SetAdditionalFields(const std::map<std::string, std::string>& additional_fields) {
    _additional_fields = additional_fields;
  }

  /*!
  */
  inline Message::Message(const rapidjson::Value &json) {
    FromJson(json);
  }

  inline bool Message::IsValid() const {
    if (_message.empty()) {
      return false;
    }
    return true;
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
      } else if (strcmp(member->name.GetString(), JSON_ELEM_LEVEL) == 0) {
        if (!member->value.IsNull()) {
          if (member->value.IsString()) {
            std::string found_level = member->value.GetString();
            _level = found_level;
          } else {
            std::cerr << "not a string";
          }
        } else {
          std::cerr << "null";
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
  inline void Message::ToJson(rapidjson::Document &doc) const {
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

    if (_level.IsValid()) {
      rapidjson::Value level(rapidjson::kStringType);
      const std::string level_str = _level.GetString();
      level.SetString(level_str.data(), static_cast<rapidjson::SizeType>(level_str.size()), allocator);
      doc.AddMember(rapidjson::StringRef(JSON_ELEM_LEVEL), level, allocator);
    }

    for (auto additional = _additional_fields.cbegin(); additional != _additional_fields.cend(); ++additional) {
      rapidjson::Value key(rapidjson::kStringType);
      key.SetString(additional->first.data(), static_cast<rapidjson::SizeType>(additional->first.size()), allocator);

      rapidjson::Value value(rapidjson::kStringType);
      value.SetString(additional->second.data(), static_cast<rapidjson::SizeType>(additional->second.size()), allocator);

      doc.AddMember(key, value, allocator);
    }
  }

} // namespace sentry

#endif // SENTRY_MESSAGE_H_