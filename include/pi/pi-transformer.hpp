#pragma once

#include <pi/generator/pi-code-generator.hpp>
#include <pi/parser/pi-parser.hpp>

namespace pi::transformer {

struct Transformer {
  Transformer(
      const pi::parser::PIParser& parser, const pi::generator::PICodeGenerator& generator
  ) noexcept;

  std::string transform(const char* file_path) const;

private:
  const pi::parser::PIParser& parser_;
  const pi::generator::PICodeGenerator& generator_;
};

} // namespace pi::transformer