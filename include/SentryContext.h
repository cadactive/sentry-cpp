/********************************************//**
* @file SentryContext.h
* @brief Interface for Sentry Contexts
* @details https://docs.sentry.io/clientdev/interfaces/contexts/
* @author James Sullivan
* @version
* @copyright CadActive Technologies, LLC
***********************************************/
#ifndef SENTRY_CONTEXT_H_
#define SENTRY_CONTEXT_H_
#include <string>

#include "rapidjson\rapidjson.h"
#include "rapidjson\document.h"

/***********************************************
*	Constants
***********************************************/
namespace sentry {

  const char * const JSON_ELEM_CONTEXTS = "contexts";

  const char * const JSON_ELEM_CONTEXT_NAME = "name";
  const char * const JSON_ELEM_CONTEXT_TYPE = "type";

  const char * const JSON_ELEM_CONTEXT_OS = "os";
  const char * const JSON_ELEM_OS_VERSION = "version";
  const char * const JSON_ELEM_OS_BUILD = "build";
  const char * const JSON_ELEM_OS_KERNEL_VERSION = "kernel_version";
  const char * const JSON_ELEM_OS_ROOTED = "rooted";

  const char * const JSON_ELEM_CONTEXT_RUNTIME = "runtime";
  const char * const JSON_ELEM_RUNTIME_VERSION = "version";

} // namespace sentry

/***********************************************
*	Classes
***********************************************/
namespace sentry {

  /*! @brief A Context in Sentry
  */
  class ContextGeneral {
  public:
    ContextGeneral();
    ContextGeneral(const std::string &type, const std::string &name);
    ContextGeneral(const rapidjson::Value &json);

    bool IsValid() const;

    const std::string& GetType() const;
    const std::string& GetName() const;

    void ToJson(rapidjson::Document &doc) const;

  protected:
    void FromJson(const rapidjson::Value &json);

  private:
    std::string _type;
    std::string _name;

  }; // class ContextGeneral

  /*! @brief
  */
  class ContextOS : public ContextGeneral {
  public:
    ContextOS(const std::string &name, const std::string &version, const std::string &build = std::string(), const std::string &kernel_version = std::string(), const bool &rooted = false);
    ContextOS(const rapidjson::Value &json);

    const std::string& GetVersion() const;
    const std::string& GetBuild() const;
    const std::string& GetKernelVersion() const;
    const bool IsRooted() const;

    void ToJson(rapidjson::Document &doc) const;

  protected:
    void FromJson(const rapidjson::Value &json);

  private:
    std::string _version;
    std::string _build;
    std::string _kernel_version;
    bool _is_rooted;
  }; // class ContextOS

  /*! @brief
  */
  class ContextRuntime : public ContextGeneral {
  public:
    ContextRuntime(const std::string &name, const std::string &version);
    ContextRuntime(const rapidjson::Value &json);

    const std::string& GetVersion() const;

    void ToJson(rapidjson::Document &doc) const;

  protected:
    void FromJson(const rapidjson::Value &json);

  private:
    std::string _version;

  }; // class ContextRuntime

} // namespace sentry

/***********************************************
*	Method Definitions
***********************************************/
namespace sentry {
  /*!
  */
  inline ContextGeneral::ContextGeneral() {}

  inline ContextGeneral::ContextGeneral(const std::string &type, const std::string &name) :
   _type(type), _name(name) {
  
  }

  inline const std::string & ContextGeneral::GetType() const {
    return _type;
  }

  inline const std::string & ContextGeneral::GetName() const {
    return _name;
  }

  /*!
  */
  inline ContextGeneral::ContextGeneral(const rapidjson::Value &json) {
    FromJson(json);
  }

  inline bool ContextGeneral::IsValid() const {
    if (_name.empty() || _type.empty()) {
      return false;
    }
    return true;
  }

  /*! @brief Construct from a JSON object
  */
  inline void ContextGeneral::FromJson(const rapidjson::Value & json) {
    if (json.IsNull()) { return; }
    if (!json.IsObject()) { return; }

    if (json.HasMember(JSON_ELEM_CONTEXT_NAME)) {
      const rapidjson::Value &name = json[JSON_ELEM_CONTEXT_NAME];
      if (!name.IsNull()) {
        if (name.IsString()) {
          _name = name.GetString();
        }
      }
    }

    if (json.HasMember(JSON_ELEM_CONTEXT_TYPE)) {
      const rapidjson::Value &type = json[JSON_ELEM_CONTEXT_TYPE];
      if (!type.IsNull()) {
        if (type.IsString()) {
          _type = type.GetString();
        }
      }
    }
  }

  /*! @brief Convert to a JSON object
  */
  inline void ContextGeneral::ToJson(rapidjson::Document &doc) const {
    doc.SetObject();
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

    if (!_name.empty()) {
      rapidjson::Value name(rapidjson::kStringType);
      name.SetString(_name.data(), static_cast<rapidjson::SizeType>(_name.size()), allocator);
      doc.AddMember(rapidjson::StringRef(JSON_ELEM_CONTEXT_NAME), name, allocator);
    }

    if (!_type.empty()) {
      rapidjson::Value type(rapidjson::kStringType);
      type.SetString(_type.data(), static_cast<rapidjson::SizeType>(_type.size()), allocator);
      doc.AddMember(rapidjson::StringRef(JSON_ELEM_CONTEXT_TYPE), type, allocator);
    }
  }

  /*
  */
  inline ContextOS::ContextOS(const std::string & name, const std::string & version, const std::string & build, const std::string & kernel_version, const bool & rooted) : 
    ContextGeneral(JSON_ELEM_CONTEXT_OS, name),
    _version(version), 
    _build(build),
    _kernel_version(kernel_version),
    _is_rooted(rooted) {

  }

  inline ContextOS::ContextOS(const rapidjson::Value & json) :
    _is_rooted(false),
    ContextGeneral(json) {
    ContextOS::FromJson(json);
  }

  inline const std::string & ContextOS::GetVersion() const {
    return _version;
  }

  inline const std::string & ContextOS::GetBuild() const {
    return _build;
  }

  inline const std::string & ContextOS::GetKernelVersion() const {
    return _kernel_version;
  }

  inline const bool ContextOS::IsRooted() const {
    return _is_rooted;
  }

  /*! @brief Construct from a JSON object
  */
  inline void ContextOS::FromJson(const rapidjson::Value & json) {
    if (json.IsNull()) { return; }
    if (!json.IsObject()) { return; }

    if (json.HasMember(JSON_ELEM_OS_VERSION)) {
      const rapidjson::Value &version = json[JSON_ELEM_OS_VERSION];
      if (!version.IsNull()) {
        if (version.IsString()) {
          _version = version.GetString();
        }
      }
    }

    if (json.HasMember(JSON_ELEM_OS_BUILD)) {
      const rapidjson::Value &build = json[JSON_ELEM_OS_BUILD];
      if (!build.IsNull()) {
        if (build.IsString()) {
          _build = build.GetString();
        }
      }
    }

    if (json.HasMember(JSON_ELEM_OS_KERNEL_VERSION)) {
      const rapidjson::Value &kernel_version = json[JSON_ELEM_OS_KERNEL_VERSION];
      if (!kernel_version.IsNull()) {
        if (kernel_version.IsString()) {
          _kernel_version = kernel_version.GetString();
        }
      }
    }

    if (json.HasMember(JSON_ELEM_OS_ROOTED)) {
      const rapidjson::Value &is_rooted = json[JSON_ELEM_OS_ROOTED];
      if (!is_rooted.IsNull()) {
        if (is_rooted.IsBool()) {
          _is_rooted = is_rooted.GetBool();
        }
      }
    }
  }

  /*! @brief Convert to a JSON object
  */
  inline void ContextOS::ToJson(rapidjson::Document & doc) const {
    ContextGeneral::ToJson(doc);

    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

    if (!_version.empty()) {
      rapidjson::Value version(rapidjson::kStringType);
      version.SetString(_version.data(), static_cast<rapidjson::SizeType>(_version.size()), allocator);
      doc.AddMember(rapidjson::StringRef(JSON_ELEM_OS_VERSION), version, allocator);
    }

    if (!_build.empty()) {
      rapidjson::Value build(rapidjson::kStringType);
      build.SetString(_build.data(), static_cast<rapidjson::SizeType>(_build.size()), allocator);
      doc.AddMember(rapidjson::StringRef(JSON_ELEM_OS_BUILD), build, allocator);
    }

    if (!_kernel_version.empty()) {
      rapidjson::Value kernel_version(rapidjson::kStringType);
      kernel_version.SetString(_kernel_version.data(), static_cast<rapidjson::SizeType>(_kernel_version.size()), allocator);
      doc.AddMember(rapidjson::StringRef(JSON_ELEM_OS_KERNEL_VERSION), kernel_version, allocator);
    }

    // doc.AddMember(rapidjson::StringRef(JSON_ELEM_OS_ROOTED), _is_rooted, allocator);
  }

  /*!
  */
  inline ContextRuntime::ContextRuntime(const std::string & name, const std::string & version) :
    ContextGeneral(JSON_ELEM_CONTEXT_RUNTIME, name),
    _version(version) {

  }

  inline ContextRuntime::ContextRuntime(const rapidjson::Value & json) :
    ContextGeneral(json) {
    ContextRuntime::FromJson(json);
  }

  inline const std::string & ContextRuntime::GetVersion() const {
    return _version;
  }

  /*! @brief Construct from a JSON object
  */
  inline void ContextRuntime::FromJson(const rapidjson::Value & json) {
    if (json.IsNull()) { return; }
    if (!json.IsObject()) { return; }

    if (json.HasMember(JSON_ELEM_RUNTIME_VERSION)) {
      const rapidjson::Value &version = json[JSON_ELEM_RUNTIME_VERSION];
      if (!version.IsNull()) {
        if (version.IsString()) {
          _version = version.GetString();
        }
      }
    }
  }

  /*! @brief Convert to a JSON object
  */
  inline void ContextRuntime::ToJson(rapidjson::Document & doc) const {
    ContextGeneral::ToJson(doc);

    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

    if (!_version.empty()) {
      rapidjson::Value version(rapidjson::kStringType);
      version.SetString(_version.data(), static_cast<rapidjson::SizeType>(_version.size()), allocator);
      doc.AddMember(rapidjson::StringRef(JSON_ELEM_RUNTIME_VERSION), version, allocator);
    }
  }

} // namespace sentry

#endif // SENTRY_CONTEXT_H_