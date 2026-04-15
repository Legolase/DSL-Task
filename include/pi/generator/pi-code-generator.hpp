#pragma once

#include <pi/pi-controller.hpp>

#include <ostream>

namespace pi::generator {

struct PICodeGenerator {
  virtual ~PICodeGenerator() = default;

  virtual std::string generate(const pi::PIController& controller) const = 0;
};

} // namespace pi::generator
