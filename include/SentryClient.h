/********************************************//**
* @file SentryClient.h
* @brief Interface for Sentry Clients
* @details https://docs.sentry.io/clientdev/interfaces/message/
* @author James Sullivan
* @version
* @copyright CadActive Technologies, LLC
***********************************************/
#ifndef SENTRY_CLIENT_H_
#define SENTRY_CLIENT_H_
#include <ctime>

#include "rapidjson\rapidjson.h"
#include "rapidjson\document.h"

/***********************************************
*	Constants
***********************************************/
namespace Sentry {

  const char * const CLIENT_NAME = "sentry-cpp";
  const char * const CLIENT_VERSION = "0.0.1.0";

  const char * const JSON_ELEM_MESSAGE = "message";
  const char * const JSON_ELEM_FORMAT_PARAMS = "params";

} // namespace Sentry

/***********************************************
*	Classes
***********************************************/
namespace Sentry {

  /*! @brief An Client in Sentry
  */
  class DSN {
  public:
    DSN(const std::string &dsn);

    bool IsValid() const;

    const std::string& GetUrl() const;
    const std::string& GetPublicKey() const;
    const std::string& GetSecretKey() const;
    const std::string& GetProjectID() const;

  protected:
    void ParseDSN(const std::string &dsn);

  private:
    std::string _protocol;
    std::string _url;
    std::string _public_key;
    std::string _secret_key;
    std::string _project_id;

  }; // class DSN

  /*! @brief An Client in Sentry
  */
  class Client {
  public:
    Client(const DSN &dsn, const int &timeout = 10);

    bool IsEnabled() const;

    const DSN& GetDSN() const;

    static const std::string GetClientInfo();
    const std::string GenerateAuthentication() const;

  private:
    DSN _dsn;
    int _timeout;

  }; // class Client

} // namespace Sentry

/***********************************************
*	Method Definitions
***********************************************/
namespace Sentry {
  DSN::DSN(const std::string &dsn) {
    ParseDSN(dsn);
  }

  inline bool DSN::IsValid() const {
    if (_protocol.empty() || 
        _url.empty() ||
        _public_key.empty() ||
        _secret_key.empty() ||
        _project_id.empty()) {
      return false;
    }
    return true;
  }

  inline const std::string& DSN::GetUrl() const {
    return _url;
  }

  inline const std::string& DSN::GetPublicKey() const {
    return _public_key;
  }

  inline const std::string& DSN::GetSecretKey() const {
    return _secret_key;
  }

  inline const std::string& DSN::GetProjectID() const {
    return _project_id;
  }

  /*! @brief Parse the DSN Format (as per API):
  *   @details {PROTOCOL}://{PUBLIC_KEY}:{SECRET_KEY}@{HOST}/{PATH}{PROJECT_ID}
  */
  void Sentry::DSN::ParseDSN(const std::string & dsn) {
    std::string tmp_str;

    // Protocol
    _protocol = dsn.substr(0, dsn.find(':'));

    // Project_id
    size_t found = dsn.find_last_of("/");
    _project_id = dsn.substr(found + 1);
    tmp_str = dsn.substr(0, found);

    // URL
    found = dsn.find_last_of("@");
    _url = _protocol;
    _url.append(tmp_str.substr(found + 1));
    _url.append("/api/");
    _url.append(_project_id);
    _url.append("/store/");

    // public and secret key
    tmp_str = dsn.substr(0, found);
    found = tmp_str.find_first_of("//");
    std::string key = tmp_str.substr(found + 2);
    found = key.find_last_of(":");
    _public_key = key.substr(0, found);
    _secret_key = key.substr(found + 1);
  }

  /*!
  */
  inline Client::Client(const DSN &dsn, const int &timeout) :
    _dsn(dsn), _timeout(timeout) {
  }

  inline bool Client::IsEnabled() const {
    return _dsn.IsValid();
  }

  inline const DSN& Client::GetDSN() const {
    return _dsn;
  }

  inline const std::string Client::GetClientInfo() {
    std::string client_info;
    client_info += CLIENT_NAME;
    client_info += "/";
    client_info += CLIENT_VERSION;
    return client_info;
  }

  /*! @brief Generate the Authentication header information
  *   @details
  X-Sentry-Auth: Sentry sentry_version=5,
  sentry_client=<client version, arbitrary>,
  sentry_timestamp=<current timestamp>,
  sentry_key=<public api key>,
  sentry_secret=<secret api key>
  */
  inline const std::string Client::GenerateAuthentication() const {
    std::string auth = "X-Sentry-Auth: Sentry ";
    auth += "sentry_version = 5,";
    auth += "sentry_client=";
    auth += GetClientInfo();
    auth += "sentry_timestamp=";
    std::time_t time = std::time(nullptr);
    auth += std::asctime(std::localtime(&time));
    auth += "<current timestamp>, ";
    auth += "sentry_key=";
    auth += _dsn.GetPublicKey();
    auth += ",";
    auth += "sentry_secret=";
    auth += _dsn.GetSecretKey();

    return auth;
  }

} // namespace Sentry

#endif // SENTRY_CLIENT_H_