#pragma once

#include <pi/generator/pi-code-generator.hpp>

namespace pi::generator {

struct PICGenerator final : PICodeGenerator {
  ~PICGenerator() override = default;

  virtual std::string generate(const pi::PIController& controller) const override;
};

} // namespace pi::generator
