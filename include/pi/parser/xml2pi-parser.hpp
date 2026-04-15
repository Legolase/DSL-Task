#pragma once

#include <pi/parser/pi-parser.hpp>

namespace pi::parser {

struct XML2PIParser final : PIParser {
  ~XML2PIParser() override = default;

  virtual PIController parse(const char* file_path) const override;
};

} // namespace pi::parser
