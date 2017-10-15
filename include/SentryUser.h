/********************************************//**
* @file SentryUser.h
* @brief Interface for Sentry Users
* @details https://docs.sentry.io/clientdev/interfaces/user/
* @author James Sullivan
* @version
* @copyright CadActive Technologies, LLC
***********************************************/
#ifndef SENTRY_USER_H_
#define SENTRY_USER_H_
#include <map>

#include "rapidjson\rapidjson.h"
#include "rapidjson\document.h"

/***********************************************
*	Constants
***********************************************/
namespace Sentry {

  const char * const JSON_ELEM_USER_ID = "id";
  const char * const JSON_ELEM_USER_EMAIL = "email";
  const char * const JSON_ELEM_USER_USERNAME = "username";
  const char * const JSON_ELEM_USER_IP_ADDRESS = "ip_address";

} // namespace Sentry

  /***********************************************
  *	Classes
  ***********************************************/
namespace Sentry {

  /*! @brief An User in Sentry
  */
  class User {
  public:
    User();
    User(const std::string &user_unique_id, const std::string &email, const std::string &username, const std::string &ip_address, const std::map<std::string, std::string> &additional_user_fields = std::map<std::string, std::string>());
    User(const rapidjson::Value &json);

    const std::string &GetUserUniqueID() const;
    const std::string &GetEmail() const;
    const std::string &GetUsername() const;
    const std::string &GetIPAddress() const;
    const std::map<std::string, std::string> &GetAdditionalFields() const;

    void ToJson(rapidjson::Document &doc);

  protected:
    void FromJson(const rapidjson::Value &json);

  private:
    std::string _user_unique_id;
    std::string _email;
    std::string _username;
    std::string _ip_address;
    std::map<std::string, std::string> _additional_user_fields;

  }; // class User

  /*!
  */
  inline User::User() {}

  inline User::User(const std::string &user_unique_id, const std::string &email, const std::string &username, const std::string &ip_address, const std::map<std::string, std::string> &additional_user_fields) :
    _user_unique_id(user_unique_id),
    _email(email),
    _username(username),
    _ip_address(ip_address),
    _additional_user_fields(additional_user_fields) {

  }

  inline const std::string & User::GetUserUniqueID() const {
    return _user_unique_id;
  }

  inline const std::string & User::GetEmail() const {
    return _email;
  }

  inline const std::string & User::GetUsername() const {
    return _username;
  }

  inline const std::string & User::GetIPAddress() const {
    return _ip_address;
  }

  inline const std::map<std::string, std::string>& User::GetAdditionalFields() const {
    return _additional_user_fields;
  }

  /*!
  */
  inline User::User(const rapidjson::Value &json) {
    FromJson(json);
  }

  /*! @brief Construct from a JSON object
  */
  inline void User::FromJson(const rapidjson::Value & json) {
    if (json.IsNull()) { return; }
    if (!json.IsObject()) { return; }

    // Looping so that we find "everything"
    for (rapidjson::Value::ConstMemberIterator member = json.MemberBegin(); member != json.MemberEnd(); ++member) {

      if (strcmp(member->name.GetString(), JSON_ELEM_USER_ID) == 0) {
        if (!member->value.IsNull()) {
          if (member->value.IsString()) {
            _user_unique_id = member->value.GetString();
            continue;
          }
        }
      } else if (strcmp(member->name.GetString(), JSON_ELEM_USER_EMAIL) == 0) {
        if (!member->value.IsNull()) {
          if (member->value.IsString()) {
            _email = member->value.GetString();
            continue;
          }
        }
      } else if (strcmp(member->name.GetString(), JSON_ELEM_USER_USERNAME) == 0) {
        if (!member->value.IsNull()) {
          if (member->value.IsString()) {
            _username = member->value.GetString();
            continue;
          }
        }
      } else if (strcmp(member->name.GetString(), JSON_ELEM_USER_IP_ADDRESS) == 0) {
        if (!member->value.IsNull()) {
          if (member->value.IsString()) {
            _ip_address = member->value.GetString();
            continue;
          }
        }
      } else {
        if (strlen(member->name.GetString()) > 0) {
          if (member->value.IsString()) {
            _additional_user_fields[member->name.GetString()] = member->value.GetString();
          }
        }
      }
    }    
  }

  /*! @brief Convert to a JSON object
  */
  inline void User::ToJson(rapidjson::Document &doc) {
    doc.SetObject();
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

    if (!_user_unique_id.empty()) {
      rapidjson::Value user_unique_id(rapidjson::kStringType);
      user_unique_id.SetString(_user_unique_id.data(), static_cast<rapidjson::SizeType>(_user_unique_id.size()), allocator);
      doc.AddMember(rapidjson::StringRef(JSON_ELEM_USER_ID), user_unique_id, allocator);
    }

    if (!_email.empty()) {
      rapidjson::Value email(rapidjson::kStringType);
      email.SetString(_email.data(), static_cast<rapidjson::SizeType>(_email.size()), allocator);
      doc.AddMember(rapidjson::StringRef(JSON_ELEM_USER_EMAIL), email, allocator);
    }

    if (!_username.empty()) {
      rapidjson::Value username(rapidjson::kStringType);
      username.SetString(_username.data(), static_cast<rapidjson::SizeType>(_username.size()), allocator);
      doc.AddMember(rapidjson::StringRef(JSON_ELEM_USER_USERNAME), username, allocator);
    }

    if (!_ip_address.empty()) {
      rapidjson::Value ip_address(rapidjson::kStringType);
      ip_address.SetString(_ip_address.data(), static_cast<rapidjson::SizeType>(_ip_address.size()), allocator);
      doc.AddMember(rapidjson::StringRef(JSON_ELEM_USER_IP_ADDRESS), ip_address, allocator);
    }

    for (auto additional = _additional_user_fields.begin(); additional != _additional_user_fields.end(); ++additional) {
      rapidjson::Value key(rapidjson::kStringType);
      key.SetString(additional->first.data(), static_cast<rapidjson::SizeType>(additional->first.size()), allocator);

      rapidjson::Value value(rapidjson::kStringType);
      value.SetString(additional->second.data(), static_cast<rapidjson::SizeType>(additional->second.size()), allocator);

      doc.AddMember(key, value, allocator);
    }
  }

} // namespace Sentry

#endif // SENTRY_MESSAGE_H_