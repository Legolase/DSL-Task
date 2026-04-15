#pragma once

#include <pi/node/portable_node/portable_node.hpp>

namespace pi::node {

struct GainNode : PortableNode {
  GainNode(int sid, const std::string& name, int port_sid, const std::string& mul_value);

  const std::string& mulValue() const noexcept;

private:
  std::string mul_value_;
};

} // namespace pi::node
