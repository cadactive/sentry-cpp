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

#include "rapidjson\rapidjson.h"
#include "rapidjson\document.h"

/***********************************************
*	Constants
***********************************************/
namespace Sentry {

  const char * const JSON_ELEM_MESSAGE = "message";
  const char * const JSON_ELEM_FORMAT_PARAMS = "params";

} // namespace Sentry

/***********************************************
*	Classes
***********************************************/
namespace Sentry {

  /*! @brief An Message in Sentry
  */
  class Message {
  public:
    Message();
    Message(const std::string &message, const std::string &format_params);
    Message(const rapidjson::Value &json);

    const std::string& GetMessage() const;
    const std::string& GetFormatParams() const;

    void ToJson(rapidjson::Document &doc);

  protected:
    void FromJson(const rapidjson::Value &json);

  private:
    std::string _message;
    std::string _format_params;

  }; // class Message

  /*!
  */
  inline Message::Message() {}

  inline Message::Message(const std::string &type, const std::string &_message) {}

  inline const std::string & Message::GetMessage() const {
    return _message;
  }

  inline const std::string & Message::GetFormatParams() const {
    return _message;
  }

  /*!
  */
  inline Message::Message(const rapidjson::Value &json) {
    FromJson(json);
  }

  /*!
  */
  inline void Message::FromJson(const rapidjson::Value & json) {
    if (json.IsNull()) { return; }
    if (!json.IsObject()) { return; }

    if (json.HasMember(JSON_ELEM_MESSAGE)) {
      const rapidjson::Value &message = json[JSON_ELEM_MESSAGE];
      if (!message.IsNull()) {
        if (message.IsString()) {
          _message = message.GetString();
        }
      }
    }

    if (json.HasMember(JSON_ELEM_FORMAT_PARAMS)) {
      const rapidjson::Value &format_params = json[JSON_ELEM_FORMAT_PARAMS];
      if (!format_params.IsNull()) {
        if (format_params.IsString()) {
          _format_params = format_params.GetString();
        }
      }
    }
  }

  /*!
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
  }

} // namespace Sentry

#endif // SENTRY_MESSAGE_H_