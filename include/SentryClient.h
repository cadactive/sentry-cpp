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
#include <string>

#include "rapidjson\rapidjson.h"
#include "rapidjson\document.h"

/***********************************************
*	Constants
***********************************************/
namespace Sentry {

  const char * const CLIENT_NAME = "sentry-cpp";
  const char * const CLIENT_VERSION = "0.0.1.0";

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
    static const std::string GenerateTimestampString(const std::time_t &time);

  private:
    DSN _dsn;
    int _timeout;

  }; // class Client

} // namespace Sentry

/***********************************************
*	Method Definitions
***********************************************/
namespace Sentry {

  inline DSN::DSN(const std::string &dsn) {
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
  inline void Sentry::DSN::ParseDSN(const std::string & dsn) {
    std::string tmp_str;

    // Protocol
    std::string rest = dsn;

    size_t found = rest.find_first_of("://");
    if (found == std::string::npos) { return; }

    std::string protocol = rest.substr(0, found);
    rest = rest.substr(found + 3);

    // Public and Secret key
    found = rest.find_first_of(':');
    if (found == std::string::npos) { return; }

    std::string public_key = rest.substr(0, found);
    rest = rest.substr(found + 1);

    found = rest.find_first_of('@');
    if (found == std::string::npos) { return; }

    std::string secret_key = rest.substr(0, found);
    rest = rest.substr(found + 1);

    // URL
    found = rest.find_first_of('/');
    if (found == std::string::npos) { return; }
    std::string host = rest.substr(0, found);
    rest = rest.substr(found + 1);

    std::string project_id = rest;

    if (!protocol.empty() && !host.empty() && !project_id.empty()) {
      std::string url = protocol;
      url.append("://");
      url.append(host);
      url.append("/api/");
      url.append(project_id);
      url.append("/store/");

      _protocol = protocol;
      _project_id = project_id;
      _url = url;
      _public_key = public_key;
      _secret_key = secret_key;
    }
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
    auth += "sentry_version=7";
    auth += ",";

    auth += "sentry_client=";
    auth += GetClientInfo();
    auth += ",";

    auth += "sentry_timestamp=";
    auth += std::to_string(std::time(nullptr));
    auth += ",";

    auth += "sentry_key=";
    auth += _dsn.GetPublicKey();
    auth += ",";

    auth += "sentry_secret=";
    auth += _dsn.GetSecretKey();

    return auth;
  }

  inline const std::string Client::GenerateTimestampString(const std::time_t &time) {
    struct tm buf;
    localtime_s(&buf, &time);
    char timestamp[500];
    asctime_s(timestamp, &buf);
    return timestamp;
  }

} // namespace Sentry

#endif // SENTRY_CLIENT_H_