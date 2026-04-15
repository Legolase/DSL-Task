#pragma once

#include <pi/pi-controller.hpp>

#include <defines.hpp>

namespace pi::parser {

DECLARE_EXCEPTION(PIParserException);

struct PIParser {
  virtual ~PIParser() = default;

  virtual PIController parse(const char* file_path) const = 0;
};

} // namespace pi::parser
