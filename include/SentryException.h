/********************************************//**
* @file SentryException.h
* @brief Interface for Sentry Exceptions
* @details https://docs.sentry.io/clientdev/interfaces/exception/
* @author James Sullivan
* @version
* @copyright CadActive Technologies, LLC
***********************************************/
#ifndef SENTRY_EXCEPTION_H_
#define SENTRY_EXCEPTION_H_
#include "SentryStacktrace.h"

#include "rapidjson\rapidjson.h"
#include "rapidjson\document.h"

/***********************************************
*	Constants
***********************************************/
namespace sentry {

  const char * const JSON_ELEM_EXCEPTION = "exception";

  const char * const JSON_ELEM_EXCEPTION_TYPE = "type";
  const char * const JSON_ELEM_EXCEPTION_VALUE = "value";
  const char * const JSON_ELEM_EXCEPTION_MODULE = "module";

} // namespace sentry

/***********************************************
*	Classes
***********************************************/
namespace sentry {

  /*! @brief An Exception in Sentry
  */
  class Exception {
  public:
    Exception();
    Exception(const std::string &type, const std::string &value, const std::string  &module,
      const Stacktrace &stacktrace = Stacktrace(), const int &thread_id = -1);
    Exception(const rapidjson::Value &json);

    bool IsValid() const;

    const std::string& GetType() const;
    const std::string& GetValue() const;
    const std::string& GetModule() const;
    const Stacktrace& GetStacktrace() const;
    const int& GetThreadId() const;

    void ToJson(rapidjson::Document &doc) const;

  protected:
    void FromJson(const rapidjson::Value &json);

  private:
    std::string _type;
    std::string _value;
    std::string  _module;
    int _thread_id;
    Stacktrace _stacktrace;

  }; // class Exception

} // namespace sentry

/***********************************************
*	Method Definitions
***********************************************/
namespace sentry {

  /*!
  */
  inline Exception::Exception() :
    _thread_id(-1) {

  }

  inline Exception::Exception(
    const std::string &type, const std::string &value, const std::string  &module,
    const Stacktrace &stacktrace, const int &thread_id) :
    _type(type), _value(value), _module(module), _stacktrace(stacktrace), _thread_id(thread_id) {

  }

  inline const std::string & Exception::GetType() const {
    return _type;
  }

  inline const std::string & Exception::GetValue() const {
    return _value;
  }

  inline const std::string & Exception::GetModule() const {
    return _module;
  }

  inline const Stacktrace & Exception::GetStacktrace() const {
    return _stacktrace;
  }

  inline const int & Exception::GetThreadId() const {
    return _thread_id;
  }

  /*!
  */
  inline Exception::Exception(const rapidjson::Value &json) :
    _thread_id(-1) {
    FromJson(json);
  }

  inline bool Exception::IsValid() const {
    if (_type.empty() || _value.empty()) {
      return false;
    }
    return true;
  }

  /*!
  */
  inline void Exception::FromJson(const rapidjson::Value & json) {
    if (json.IsNull()) { return; }
    if (!json.IsObject()) { return; }

    if (json.HasMember(JSON_ELEM_EXCEPTION_TYPE)) {
      const rapidjson::Value &type = json[JSON_ELEM_EXCEPTION_TYPE];
      if (!type.IsNull()) {
        if (type.IsString()) {
          _type = type.GetString();
        }
      }
    }

    if (json.HasMember(JSON_ELEM_EXCEPTION_VALUE)) {
      const rapidjson::Value &value = json[JSON_ELEM_EXCEPTION_VALUE];
      if (!value.IsNull()) {
        if (value.IsString()) {
          _value = value.GetString();
        }
      }
    }

    if (json.HasMember(JSON_ELEM_EXCEPTION_MODULE)) {
      const rapidjson::Value &module = json[JSON_ELEM_EXCEPTION_MODULE];
      if (!module.IsNull()) {
        if (module.IsString()) {
          _module = module.GetString();
        }
      }
    }

    if (json.HasMember(JSON_ELEM_THREAD_ID)) {
      const rapidjson::Value &thread_id = json[JSON_ELEM_THREAD_ID];
      if (!thread_id.IsNull()) {
        if (thread_id.IsInt()) {
          _thread_id = thread_id.GetInt();

        } else if (thread_id.IsString()) {
          std::string thread_id_str = thread_id.GetString();
          _thread_id = atoi(thread_id_str.data());

        }
      }
    }

    if (json.HasMember(JSON_ELEM_STACKTRACE)) {
      const rapidjson::Value &stacktrace = json[JSON_ELEM_STACKTRACE];
      if (!stacktrace.IsNull()) {
        if (stacktrace.IsObject()) {
          _stacktrace = stacktrace;
        }
      }
    }
  }

	/*!
	*/
  inline void Exception::ToJson(rapidjson::Document &doc) const {
    doc.SetObject();
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

    if (!_type.empty()) {
      rapidjson::Value type(rapidjson::kStringType);
      type.SetString(_type.data(), static_cast<rapidjson::SizeType>(_type.size()), allocator);
      doc.AddMember(rapidjson::StringRef(JSON_ELEM_EXCEPTION_TYPE), type, allocator);
    }

	  if (!_value.empty()) {
		  rapidjson::Value value(rapidjson::kStringType);
		  value.SetString(_value.data(), static_cast<rapidjson::SizeType>(_value.size()), allocator);
		  doc.AddMember(rapidjson::StringRef(JSON_ELEM_EXCEPTION_VALUE), value, allocator);
	  }

	  if (!_module.empty()) {
		  rapidjson::Value module(rapidjson::kStringType);
		  module.SetString(_module.data(), static_cast<rapidjson::SizeType>(_module.size()), allocator);
		  doc.AddMember(rapidjson::StringRef(JSON_ELEM_EXCEPTION_MODULE), module, allocator);
	  }

	  if (_thread_id > 0) {
		  doc.AddMember(rapidjson::StringRef(JSON_ELEM_EXCEPTION_VALUE), _thread_id, allocator);
	  }

	  if (!_stacktrace.IsValid()) {
      rapidjson::Document subdoc(&allocator);
      _stacktrace.ToJson(subdoc);
		  doc.AddMember(rapidjson::StringRef(JSON_ELEM_STACKTRACE), subdoc, allocator);
	  }
	}

} // namespace sentry

#endif // SENTRY_EXCEPTION_H_