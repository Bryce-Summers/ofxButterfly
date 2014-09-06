#ifndef __ERROR_HPP
#define __ERROR_HPP
#include <string>
#include <stdexcept>

class RuntimeError : private virtual std::runtime_error
{
public:
  RuntimeError() : std::runtime_error("Runtime error") { }
  RuntimeError(const std::string& message) : 
    std::runtime_error(message.c_str()) { }
  std::string Message() const { return what(); }
};
#endif
