/********************************************//**
* @file SentryFrame.h
* @brief Interface for Sentry Frames
* @details https://docs.sentry.io/clientdev/interfaces/stacktrace/
* @author James Sullivan
* @version
* @copyright CadActive Technologies, LLC
***********************************************/
#ifndef SENTRY_FRAME_H_
#define SENTRY_FRAME_H_
#include <string>
#include <vector>
#include <map>

#include "rapidjson\rapidjson.h"
#include "rapidjson\document.h"

/***********************************************
*	Constants
***********************************************/
namespace Sentry {

  const char * const JSON_ELEM_STACKTRACE = "stacktrace";

  // Required Members
  const char * const JSON_ELEM_FILENAME = "filename";
  const char * const JSON_ELEM_FUNCTION = "function";
  const char * const JSON_ELEM_MODULE = "module";

  // Optional Members
  const char * const JSON_ELEM_ABS_PATH = "abs_path";
  const char * const JSON_ELEM_VARS = "vars";
  const char * const JSON_ELEM_LINE_NO = "lineno";
  const char * const JSON_ELEM_IN_APP = "in_app";
  const char * const JSON_ELEM_CONTEXT_LINE = "context_line";
  const char * const JSON_ELEM_PRE_CONTEXT = "pre_context";
  const char * const JSON_ELEM_POST_CONTEXT = "post_context";

  const char * const JSON_ELEM_PACKAGE = "package";
  const char * const JSON_ELEM_PLATFORM = "platform";
  const char * const JSON_ELEM_IMAGE_ADDR = "image_addr";
  const char * const JSON_ELEM_INSTRUCTION_ADDR = "instruction_addr";
  const char * const JSON_ELEM_SYMBOL_ADDR = "symbol_addr";
  const char * const JSON_ELEM_INSTRUCTION_OFFSET = "instruction_offset";

} // namespace Sentry

/***********************************************
*	Classes
***********************************************/
namespace Sentry {

  /*! @brief An Frame in Sentry
  */
  class Frame {
  public:
    Frame(const std::string &filename = std::string(), const std::string &function = std::string(), const std::string &module = std::string());
    Frame(const rapidjson::Value &json);

    bool IsValid() const;

    // Required Members
    const std::string& GetFilename() const;
    const std::string& GetFunction() const;
    const std::string& GetModule() const;

    // Optional Members
    bool IsInApp() const;
    void SetIsInApp(const bool &in_app);

    void ToJson(rapidjson::Document &doc) const;

  protected:
    void FromJson(const rapidjson::Value &json);

  private:
    // Required Members     // Each frame must contain at least one of the following attributes:
    std::string _filename;  // The relative filepath to the call
    std::string _function;  // The name of the function being called
    std::string _module;    // Platform-specific module path (e.g. sentry.interfaces.Stacktrace)

    // Optional Members
    int _lineno;            // The line number of the call
    int _colno;             // The column number of the call
    std::string _abs_path;  // The absolute path to filename 
    std::string _context_line;  // Source code in filename at lineno
    std::vector<std::string> _pre_context;   // A list of source code lines before context_line(in order) – usually[lineno - 5:lineno]
    std::vector<std::string> _post_context;  // A list of source code lines after context_line(in order) – usually[lineno + 1:lineno + 5]
    bool _in_app;           // Signifies whether this frame is related to the execution of the relevant code in this stacktrace.
    std::map<std::string, std::string> _vars; // A mapping of variables which were available within this frame(usually context - locals).

    std::string _package;
    std::string _platform;
    std::string _image_addr;
    std::string _instruction_addr;
    std::string _symbol_addr;
    std::string _instruction_offset;

  }; // class Frame

} // namespace Sentry

/***********************************************
*	Method Definitions
***********************************************/
namespace Sentry {

  /*!
  */
  inline Frame::Frame(const std::string &filename, const std::string &function, const std::string &module) :
    _filename(filename), _function(function), _module(module),
    _lineno(-1), _colno(-1), _in_app(false) {
  }

  /*!
  */
  inline Frame::Frame(const rapidjson::Value &json) :
    _lineno(-1), _colno(-1), _in_app(false) {
    FromJson(json);
  }

  /*! @brief Determine if the frame has the required information
  *   @details From API documentation - one of the three must exist
  */
  inline bool Frame::IsValid() const {
    if (_filename.empty() && _function.empty() && _module.empty()) {
      return false;
    }

    return true;
  }

  inline const std::string & Frame::GetFilename() const {
    return _filename;
  }

  inline const std::string & Frame::GetFunction() const {
    return _function;
  }

  inline const std::string & Frame::GetModule() const {
    return _module;
  }

  /*! @brief Construct from a JSON object
  */
  inline void Frame::FromJson(const rapidjson::Value & json) {
    if (json.IsNull()) { return; }
    if (!json.IsObject()) { return; }

    // Required Members
    if (json.HasMember(JSON_ELEM_FILENAME)) {
      const rapidjson::Value &filename = json[JSON_ELEM_FILENAME];
      if (!filename.IsNull()) {
        if (filename.IsString()) {
          _filename = filename.GetString();
        }
      }
    } // filename

    if (json.HasMember(JSON_ELEM_FUNCTION)) {
      const rapidjson::Value &function = json[JSON_ELEM_FUNCTION];
      if (!function.IsNull()) {
        if (function.IsString()) {
          _function = function.GetString();
        }
      }
    } // function

    if (json.HasMember(JSON_ELEM_MODULE)) {
      const rapidjson::Value &module = json[JSON_ELEM_MODULE];
      if (!module.IsNull()) {
        if (module.IsString()) {
          _module = module.GetString();
        }
      }
    } // module

    // Optional Members
    if (json.HasMember(JSON_ELEM_ABS_PATH)) {
      const rapidjson::Value &abs_path = json[JSON_ELEM_ABS_PATH];
      if (!abs_path.IsNull()) {
        if (abs_path.IsString()) {
          _abs_path = abs_path.GetString();
        }
      }
    } // abs_path

    if (json.HasMember(JSON_ELEM_VARS)) {
      const rapidjson::Value &vars = json[JSON_ELEM_VARS];
      if (!vars.IsNull()) {
        if (vars.IsObject()) {
          for (rapidjson::Value::ConstMemberIterator var = vars.MemberBegin(); var != vars.MemberEnd(); ++var) {
            std::string key = var->name.GetString();
            std::string value;

            if (var->value.IsString()) {
              value = var->value.GetString();
            
            } else if (var->value.IsInt()) {
              value = std::to_string(var->value.GetInt());
            }

            _vars[key] = value;
          }
        }
      }
    } // vars

    if (json.HasMember(JSON_ELEM_LINE_NO)) {
      const rapidjson::Value &lineno = json[JSON_ELEM_LINE_NO];
      if (!lineno.IsNull()) {
        if (lineno.IsInt()) {
          _lineno = lineno.GetInt();
        }
      }
    } // lineno

    if (json.HasMember(JSON_ELEM_IN_APP)) {
      const rapidjson::Value &in_app = json[JSON_ELEM_IN_APP];
      if (!in_app.IsNull()) {
        if (in_app.IsBool()) {
          _in_app = in_app.GetBool();
        }
      }
    } // in_app

    if (json.HasMember(JSON_ELEM_CONTEXT_LINE)) {
      const rapidjson::Value &context_line = json[JSON_ELEM_CONTEXT_LINE];
      if (!context_line.IsNull()) {
        if (context_line.IsString()) {
          _context_line = context_line.GetString();
        }
      }
    } // context_line

    if (json.HasMember(JSON_ELEM_PRE_CONTEXT)) {
      const rapidjson::Value &vars = json[JSON_ELEM_PRE_CONTEXT];
      if (!vars.IsNull()) {
        if (vars.IsArray()) {
          for (rapidjson::Value::ConstValueIterator var = vars.Begin(); var != vars.End(); ++var) {
            if (var->IsString()) {
              _pre_context.push_back(var->GetString());
            }
          }
        }
      }
    } // pre_context

    if (json.HasMember(JSON_ELEM_POST_CONTEXT)) {
      const rapidjson::Value &vars = json[JSON_ELEM_POST_CONTEXT];
      if (!vars.IsNull()) {
        if (vars.IsArray()) {
          for (rapidjson::Value::ConstValueIterator var = vars.Begin(); var != vars.End(); ++var) {
            if (var->IsString()) {
              _post_context.push_back(var->GetString());
            }
          }
        }
      }
    } // post_context

    if (json.HasMember(JSON_ELEM_PACKAGE)) {
      const rapidjson::Value &package = json[JSON_ELEM_PACKAGE];
      if (!package.IsNull()) {
        if (package.IsString()) {
          _package = package.GetString();
        }
      }
    } // package

    if (json.HasMember(JSON_ELEM_PLATFORM)) {
      const rapidjson::Value &platform = json[JSON_ELEM_PLATFORM];
      if (!platform.IsNull()) {
        if (platform.IsString()) {
          _platform = platform.GetString();
        }
      }
    } // package
    
    if (json.HasMember(JSON_ELEM_IMAGE_ADDR)) {
      const rapidjson::Value &image_addr = json[JSON_ELEM_IMAGE_ADDR];
      if (!image_addr.IsNull()) {
        if (image_addr.IsString()) {
          _image_addr = image_addr.GetString();
        }
      }
    } // image_addr

    if (json.HasMember(JSON_ELEM_INSTRUCTION_ADDR)) {
      const rapidjson::Value &instruction_addr = json[JSON_ELEM_INSTRUCTION_ADDR];
      if (!instruction_addr.IsNull()) {
        if (instruction_addr.IsString()) {
          _instruction_addr = instruction_addr.GetString();
        }
      }
    } // instruction_addr

    if (json.HasMember(JSON_ELEM_SYMBOL_ADDR)) {
      const rapidjson::Value &symbol_addr = json[JSON_ELEM_SYMBOL_ADDR];
      if (!symbol_addr.IsNull()) {
        if (symbol_addr.IsString()) {
          _symbol_addr = symbol_addr.GetString();
        }
      }
    } // symbol_addr
    
    if (json.HasMember(JSON_ELEM_INSTRUCTION_OFFSET)) {
      const rapidjson::Value &instruction_offset = json[JSON_ELEM_INSTRUCTION_OFFSET];
      if (!instruction_offset.IsNull()) {
        if (instruction_offset.IsString()) {
          _instruction_offset = instruction_offset.GetString();
        }
      }
    } // instruction_offset
  }

  inline bool Frame::IsInApp() const {
    return _in_app;
  }

  inline void Frame::SetIsInApp(const bool & in_app) { 
    _in_app = in_app;
  }

  /*! @brief Convert to a JSON object
  */
  inline void Frame::ToJson(rapidjson::Document &doc) const {
    doc.SetObject();
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

    // Required Members
    if (!_filename.empty()) {
      rapidjson::Value filename(rapidjson::kStringType);
      filename.SetString(_filename.data(), static_cast<rapidjson::SizeType>(_filename.size()), allocator);
      doc.AddMember(rapidjson::StringRef(JSON_ELEM_FILENAME), filename, allocator);
    } // filename

    if (!_function.empty()) {
      rapidjson::Value function(rapidjson::kStringType);
      function.SetString(_function.data(), static_cast<rapidjson::SizeType>(_function.size()), allocator);
      doc.AddMember(rapidjson::StringRef(JSON_ELEM_FUNCTION), function, allocator);
    } // function

    if (!_module.empty()) {
      rapidjson::Value module(rapidjson::kStringType);
      module.SetString(_module.data(), static_cast<rapidjson::SizeType>(_module.size()), allocator);
      doc.AddMember(rapidjson::StringRef(JSON_ELEM_MODULE), module, allocator);
    } // module

    // Optional Members
    if (_abs_path.empty()) {
      rapidjson::Value abs_path(rapidjson::kStringType);
      abs_path.SetString(_abs_path.data(), static_cast<rapidjson::SizeType>(_abs_path.size()), allocator);
      doc.AddMember(rapidjson::StringRef(JSON_ELEM_ABS_PATH), abs_path, allocator);
    } // abs_path

    if (!_vars.empty()) {
      rapidjson::Value vars(rapidjson::kObjectType);
      for (auto var = _vars.cbegin(); var != _vars.cend(); ++var) {
        rapidjson::Value key(rapidjson::kStringType);
        key.SetString(var->first.data(), static_cast<rapidjson::SizeType>(var->first.size()), allocator);

        rapidjson::Value value(rapidjson::kStringType);
        value.SetString(var->second.data(), static_cast<rapidjson::SizeType>(var->second.size()), allocator);

        vars.AddMember(key, value, allocator);
      }
      doc.AddMember(rapidjson::StringRef(JSON_ELEM_VARS), vars, allocator);
    } // vars

    if (!_lineno > 0) {
      doc.AddMember(rapidjson::StringRef(JSON_ELEM_LINE_NO), _lineno, allocator);
    } // lineno

    doc.AddMember(rapidjson::StringRef(JSON_ELEM_IN_APP), _in_app, allocator); // in_app

    if (!_context_line.empty()) {
      rapidjson::Value context_line(rapidjson::kStringType);
      context_line.SetString(_context_line.data(), static_cast<rapidjson::SizeType>(_context_line.size()), allocator);
      doc.AddMember(rapidjson::StringRef(JSON_ELEM_CONTEXT_LINE), context_line, allocator);
    } // context_line

    if (!_pre_context.empty()) {
      rapidjson::Value context_line(rapidjson::kArrayType);
      for (auto context = _pre_context.cbegin(); context != _pre_context.cend(); ++context) {
        rapidjson::Value line(rapidjson::kStringType);
        line.SetString(context->data(), static_cast<rapidjson::SizeType>(context->size()), allocator);
        context_line.PushBack(line, allocator);
      }   
      doc.AddMember(rapidjson::StringRef(JSON_ELEM_PRE_CONTEXT), context_line, allocator); 
    } // pre_context

    if (!_post_context.empty()) {
      rapidjson::Value context_line(rapidjson::kArrayType);
      for (auto context = _post_context.cbegin(); context != _post_context.cend(); ++context) {
        rapidjson::Value line(rapidjson::kStringType);
        line.SetString(context->data(), static_cast<rapidjson::SizeType>(context->size()), allocator);
        context_line.PushBack(line, allocator);
      }
      doc.AddMember(rapidjson::StringRef(JSON_ELEM_POST_CONTEXT), context_line, allocator);
    } // post_context

    if (!_package.empty()) {
      rapidjson::Value package(rapidjson::kStringType);
      package.SetString(_package.data(), static_cast<rapidjson::SizeType>(_package.size()), allocator);
      doc.AddMember(rapidjson::StringRef(JSON_ELEM_PACKAGE), package, allocator);
    } // package

    if (!_platform.empty()) {
      rapidjson::Value platform(rapidjson::kStringType);
      platform.SetString(_platform.data(), static_cast<rapidjson::SizeType>(_platform.size()), allocator);
      doc.AddMember(rapidjson::StringRef(JSON_ELEM_PLATFORM), platform, allocator);
    } // platform

    if (!_image_addr.empty()) {
      rapidjson::Value image_addr(rapidjson::kStringType);
      image_addr.SetString(_image_addr.data(), static_cast<rapidjson::SizeType>(_image_addr.size()), allocator);
      doc.AddMember(rapidjson::StringRef(JSON_ELEM_IMAGE_ADDR), image_addr, allocator);
    } // image_addr

    if (!_instruction_addr.empty()) {
      rapidjson::Value instruction_addr(rapidjson::kStringType);
      instruction_addr.SetString(_instruction_addr.data(), static_cast<rapidjson::SizeType>(_instruction_addr.size()), allocator);
      doc.AddMember(rapidjson::StringRef(JSON_ELEM_INSTRUCTION_ADDR), instruction_addr, allocator);
    } // instruction_addr

    if (!_symbol_addr.empty()) {
      rapidjson::Value symbol_addr(rapidjson::kStringType);
      symbol_addr.SetString(_symbol_addr.data(), static_cast<rapidjson::SizeType>(_symbol_addr.size()), allocator);
      doc.AddMember(rapidjson::StringRef(JSON_ELEM_SYMBOL_ADDR), symbol_addr, allocator);
    } // symbol_addr

    if (!_instruction_offset.empty()) {
      rapidjson::Value instruction_offset(rapidjson::kStringType);
      instruction_offset.SetString(_instruction_offset.data(), static_cast<rapidjson::SizeType>(_instruction_offset.size()), allocator);
      doc.AddMember(rapidjson::StringRef(JSON_ELEM_INSTRUCTION_OFFSET), instruction_offset, allocator);
    } // instruction_offset
  }

} // namespace Sentry

#endif // SENTRY_FRAME_H_