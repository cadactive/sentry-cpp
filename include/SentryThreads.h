/********************************************//**
* @file SentryThreads.h
* @brief Interface for Sentry Threads
* @details https://docs.sentry.io/clientdev/interfaces/thread/
* @author James Sullivan
* @version
* @copyright CadActive Technologies, LLC
***********************************************/
#ifndef SENTRY_THREADS_H_
#define SENTRY_THREADS_H_
#include "SentryStacktrace.h"

#include "rapidjson\rapidjson.h"
#include "rapidjson\document.h"

/***********************************************
*	Constants
***********************************************/
namespace Sentry {

  const char * const JSON_ELEM_THREADS = "threads";
  const char * const JSON_ELEM_THREADS_VALUES = "values";

  const char * const JSON_ELEM_THREAD_CURRENT = "current";
  const char * const JSON_ELEM_THREAD_CRASHED = "crashed";
  const char * const JSON_ELEM_THREAD_NAME = "name";

} // namespace Sentry

/***********************************************
*	Classes
***********************************************/
namespace Sentry {

  /*! @brief An Thread in Sentry
  */
  class Thread {
  public:
    Thread();
    Thread(const int &thread_id, const bool &is_crashed, const bool &is_current,
      const Stacktrace &stacktrace = Stacktrace(), const std::string &name = std::string());
    Thread(const rapidjson::Value &json);
    
    bool operator == (const Thread& other) const;
    bool operator != (const Thread& other) const;
    bool operator < (const Thread& other) const;

    bool IsValid() const;

    const int& GetThreadID() const;
    const bool& IsCrashed() const;
    const bool& IsCurrent() const;
    void SetIsCrashed(const bool &is_crashed);
    void SetIsCurrent(const bool &is_current);

    const std::string& GetName() const;
    const Stacktrace& GetStacktrace() const;

    void ToJson(rapidjson::Document &doc) const;

  protected:
    void FromJson(const rapidjson::Value &json);

  private:
    int _thread_id;
    bool _is_crashed;
    bool _is_current;
    Stacktrace _stacktrace;
    std::string _name;

  }; // class Thread

  /*! @brief A list of threads
  */
  class Threads {
  public: 
    Threads();
    Threads(const std::vector<Thread>& threads);
    Threads(const rapidjson::Value &json);

    bool IsValid() const;

    const std::vector<Thread>& GetThreads() const;

    void ToJson(rapidjson::Document &doc) const;

  protected:
    void FromJson(const rapidjson::Value &json);

  private: 
    std::vector<Thread> _threads;
  }; // class Threads

} // namespace Sentry

/***********************************************
*	Method Definitions
***********************************************/
namespace Sentry {

  /*!
  */
  inline Thread::Thread() : 
    _thread_id(-1), _is_crashed(false), _is_current(false),
    _stacktrace(Stacktrace()), _name(std::string()) {
  }

  /*!
  */
  inline Thread::Thread(
    const int &thread_id, const bool &is_crashed, const bool &is_current,
    const Stacktrace &stacktrace, const std::string &name) :
    _thread_id(thread_id), _is_crashed(is_crashed), _is_current(is_current),
    _stacktrace(stacktrace), _name(name) {

  }

  /*!
  */
  inline Thread::Thread(const rapidjson::Value &json) :
    _thread_id(-1) {
    FromJson(json);
  }

  inline bool Thread::operator == (const Thread& other) const {
    return (_thread_id == other._thread_id);
  }

  inline bool Thread::operator != (const Thread& other) const {
    return !(operator==(other));
  }

  inline bool Thread::operator < (const Thread& other) const {
    return (_thread_id < other._thread_id);
  }

  inline bool Thread::IsValid() const {
    return (_thread_id >= 0);
  }

  inline const int & Thread::GetThreadID() const {
    return _thread_id;
  }

  inline const bool& Thread::IsCrashed() const {
    return _is_crashed;
  }

  inline const bool& Thread::IsCurrent() const {
    return _is_current;
  }

  inline void Thread::SetIsCrashed(const bool &is_crashed) {
    _is_crashed = is_crashed;
  }

  inline void Thread::SetIsCurrent(const bool &is_current) {
    _is_current = is_current;
  }

  inline const Stacktrace & Thread::GetStacktrace() const {
    return _stacktrace;
  }

  inline const std::string& Thread::GetName() const {
    return _name;
  }

  /*! @brief Construct from a JSON object
  */
  inline void Thread::FromJson(const rapidjson::Value & json) {
    if (json.IsNull()) { return; }
    if (!json.IsObject()) { return; }

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
    
    if (json.HasMember(JSON_ELEM_THREAD_CURRENT)) {
      const rapidjson::Value &is_current = json[JSON_ELEM_THREAD_CURRENT];
      if (!is_current.IsNull()) {
        if (is_current.IsBool()) {
          _is_current = is_current.GetBool();
        }
      }
    }

    if (json.HasMember(JSON_ELEM_THREAD_CRASHED)) {
      const rapidjson::Value &is_crashed = json[JSON_ELEM_THREAD_CRASHED];
      if (!is_crashed.IsNull()) {
        if (is_crashed.IsBool()) {
          _is_crashed = is_crashed.GetBool();
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

    if (json.HasMember(JSON_ELEM_THREAD_NAME)) {
      const rapidjson::Value &name = json[JSON_ELEM_THREAD_NAME];
      if (!name.IsNull()) {
        if (name.IsString()) {
          _name = name.GetString();
        }
      }
    }
  }

  /*! @brief Convert to a JSON object
  */
  inline void Thread::ToJson(rapidjson::Document &doc) const {
    doc.SetObject();
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

    if (_thread_id > 0) {
      doc.AddMember(rapidjson::StringRef(JSON_ELEM_THREAD_ID), _thread_id, allocator);
    }

    doc.AddMember(rapidjson::StringRef(JSON_ELEM_THREAD_CURRENT), _is_current, allocator);
    doc.AddMember(rapidjson::StringRef(JSON_ELEM_THREAD_CRASHED), _is_crashed, allocator);

    if (!_stacktrace.IsValid()) {
      rapidjson::Document subdoc;
      _stacktrace.ToJson(subdoc);
      doc.AddMember(rapidjson::StringRef(JSON_ELEM_STACKTRACE), subdoc, allocator);
    }

    if (!_name.empty()) {
      rapidjson::Value name(rapidjson::kStringType);
      name.SetString(_name.data(), static_cast<rapidjson::SizeType>(_name.size()), allocator);
      doc.AddMember(rapidjson::StringRef(JSON_ELEM_THREAD_NAME), name, allocator);
    }
  }

  inline Threads::Threads() { }

  inline Threads::Threads(const std::vector<Thread>& threads) :
    _threads(threads) {
  
  }

  inline Threads::Threads(const rapidjson::Value & json) {
    FromJson(json);
  }

  inline bool Threads::IsValid() const {
    return (!_threads.empty());
  }

  inline const std::vector<Thread>& Threads::GetThreads() const {
    return _threads;
  }

  /*! @brief Construct from a JSON object
  */
  inline void Threads::FromJson(const rapidjson::Value & json) {
    if (json.IsNull()) { return; }
    if (!json.IsObject()) { return; }

    if (json.HasMember(JSON_ELEM_THREADS_VALUES)) {
      const rapidjson::Value &threads = json[JSON_ELEM_THREADS_VALUES];
      for (rapidjson::Value::ConstValueIterator thread = threads.Begin(); thread != threads.End(); ++thread) {
        Thread found_thread(*thread);
        if (found_thread.IsValid()) {
          _threads.push_back(found_thread);
        }
      }
    }
  }

  /*! @brief Convert to a JSON object
  */
  inline void Threads::ToJson(rapidjson::Document & doc) const {
    doc.SetObject();
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

    rapidjson::Value threads(rapidjson::kArrayType);
    for (auto thread = _threads.cbegin(); thread != _threads.end(); ++thread) {
      rapidjson::Document subdoc;
      thread->ToJson(subdoc);

      threads.PushBack(subdoc, allocator);
    }
    doc.AddMember(rapidjson::StringRef(JSON_ELEM_THREADS_VALUES), threads, allocator);
  }

} // namespace Sentry

#endif // SENTRY_THREADS_H_