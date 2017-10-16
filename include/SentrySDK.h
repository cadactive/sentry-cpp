/********************************************//**
* @file SentrySDK.h
* @brief Interface for Sentry SDKs
* @details https://docs.sentry.io/clientdev/interfaces/message/
* @author James Sullivan
* @version
* @copyright CadActive Technologies, LLC
***********************************************/
#ifndef SENTRY_SDK_H_
#define SENTRY_SDK_H_
#include <string>

#include "rapidjson\rapidjson.h"
#include "rapidjson\document.h"

/***********************************************
*	Constants
***********************************************/
namespace sentry {

  const char * const SDK_NAME = "sentry_cpp";
  const char * const SDK_VERSION = "0.0.1.0";

  const char * const JSON_ELEM_SDK = "sdk";
  const char * const JSON_ELEM_SDK_NAME = "name";
  const char * const JSON_ELEM_SDK_VERSION = "version";

} // namespace sentry

/***********************************************
*	Classes
***********************************************/
namespace sentry {

  /*! @brief An SDK in Sentry
  */
  class SDK {
  public:
    SDK();
    SDK(const rapidjson::Value &json);

    bool IsValid() const;

    const std::string& GetName() const;
    const std::string& GetVersion() const;

    void AddToJson(rapidjson::Document &doc) const;

  protected:
    void ToJson(rapidjson::Document &doc) const;
    void FromJson(const rapidjson::Value &json);

  private:
    std::string _name;
    std::string _version;

  }; // class SDK

} // namespace sentry

/***********************************************
*	Method Definitions
***********************************************/
namespace sentry {
  /*!
  */
  inline SDK::SDK() : _name(SDK_NAME), _version(SDK_VERSION) {}

  inline const std::string & SDK::GetName() const {
    return _name;
  }

  inline const std::string & SDK::GetVersion() const {
    return _version;
  }

  /*!
  */
  inline SDK::SDK(const rapidjson::Value &json) {
    FromJson(json);
  }

  inline bool SDK::IsValid() const {
    if (_name.empty() || _version.empty()) {
      return false;
    }
    return true;
  }

  inline void SDK::AddToJson(rapidjson::Document & doc) const {
    rapidjson::Document sdk_doc(&doc.GetAllocator());
    ToJson(sdk_doc);
    doc.AddMember(rapidjson::StringRef(JSON_ELEM_SDK), sdk_doc, doc.GetAllocator());
  }

  /*! @brief Construct from a JSON object
  */
  inline void SDK::FromJson(const rapidjson::Value & json) {
    if (json.IsNull()) { return; }
    if (!json.IsObject()) { return; }

    if (json.HasMember(JSON_ELEM_SDK_NAME)) {
      const rapidjson::Value &name = json[JSON_ELEM_SDK_NAME];
      if (!name.IsNull()) {
        if (name.IsString()) {
          _name = name.GetString();
        }
      }
    }

    if (json.HasMember(JSON_ELEM_SDK_VERSION)) {
      const rapidjson::Value &version = json[JSON_ELEM_SDK_VERSION];
      if (!version.IsNull()) {
        if (version.IsString()) {
          _version = version.GetString();
        }
      }
    }
  }

  /*! @brief Convert to a JSON object
  */
  inline void SDK::ToJson(rapidjson::Document &doc) const {
    doc.SetObject();
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

    if (!_name.empty()) {
      rapidjson::Value name(rapidjson::kStringType);
      name.SetString(_name.data(), static_cast<rapidjson::SizeType>(_name.size()), allocator);
      doc.AddMember(rapidjson::StringRef(JSON_ELEM_SDK_NAME), name, allocator);
    }

    if (!_version.empty()) {
      rapidjson::Value version(rapidjson::kStringType);
      version.SetString(_version.data(), static_cast<rapidjson::SizeType>(_version.size()), allocator);
      doc.AddMember(rapidjson::StringRef(JSON_ELEM_SDK_VERSION), version, allocator);
    }
  }

} // namespace sentry

#endif // SENTRY_SDK_H_