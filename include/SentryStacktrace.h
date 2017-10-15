/********************************************//**
* @file SentryStacktrace.h
* @brief Interface for Sentry Stacktraces
* @details https://docs.sentry.io/clientdev/interfaces/stacktrace/
* @author James Sullivan
* @version
* @copyright CadActive Technologies, LLC
***********************************************/
#ifndef SENTRY_STACKTRACE_H_
#define SENTRY_STACKTRACE_H_
#include <vector>

#include "rapidjson\rapidjson.h"
#include "rapidjson\document.h"

#include "SentryFrame.h"

/***********************************************
*	Constants
***********************************************/
namespace Sentry {

  const char * const JSON_ELEM_STACKTRACE = "stacktrace";
  const char * const JSON_ELEM_FRAMES = "frames";
  const char * const JSON_ELEM_FRAMES_OMITTED = "frames_omitted";

} // namespace Sentry

/***********************************************
*	Classes
***********************************************/
namespace Sentry {

  /*! @brief An Stacktrace in Sentry
  *   @details Per Sentry Documentation, frames are ordered oldest call first
  */
  class Stacktrace {
  public:
    Stacktrace();
    Stacktrace(const rapidjson::Value &json);

    bool IsValid() const;

    const std::vector<Frame>& GetFrames() const;

    void ToJson(rapidjson::Document &doc);

  protected:
    void FromJson(const rapidjson::Value &json);

  private:
    std::vector<Frame> _frames;

  }; // class Stacktrace

} // namespace Sentry

/***********************************************
*	Method Definitions
***********************************************/
namespace Sentry {

  /*!
  */
  inline Stacktrace::Stacktrace() { }

  /*!
  */
  inline Stacktrace::Stacktrace(const rapidjson::Value &json) {
    FromJson(json);
  }

  /*! @brief Check if the Stacktrace's frames are all valid
  */
  inline bool Stacktrace::IsValid() const {
    for (auto frame = _frames.begin(); frame != _frames.end(); ++frame) {
      if (!frame->IsValid()) {
        return false;
      }
    }
    return true;
  }

  inline const std::vector<Frame>& Stacktrace::GetFrames() const {
    return _frames;
  }

  /*! @brief Construct from a JSON object
  */
  inline void Stacktrace::FromJson(const rapidjson::Value & json) {
    if (json.IsNull()) { return; }
    if (!json.IsObject()) { return; }
    
    if (json.HasMember(JSON_ELEM_FRAMES)) {
      const rapidjson::Value &frames = json[JSON_ELEM_FRAMES];
      if (!frames.IsNull()) {
        if (frames.IsArray()) {
          for (rapidjson::Value::ConstValueIterator value = frames.Begin(); value != frames.End(); ++value) {
            Sentry::Frame frame(*value);
            if (frame.IsValid()) {
              _frames.push_back(frame);
            }
          }
        }
      }
    }
  }

  /*! @brief Convert to a JSON object
  */
  inline void Stacktrace::ToJson(rapidjson::Document &doc) {
    doc.SetObject();
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

    rapidjson::Value frames(rapidjson::kArrayType);
    for (auto frame = _frames.begin(); frame != _frames.end(); ++frame) {
      if (!frame->IsValid()) { continue; }

      rapidjson::Document frame_doc;
      frame->ToJson(frame_doc);
      
      frames.PushBack(frame_doc, allocator);
    }
    doc.AddMember(rapidjson::StringRef(JSON_ELEM_FRAMES), frames, allocator);
  }

} // namespace Sentry

#endif // SENTRY_STACKTRACE_H_