#pragma once

#include <pi/node/portable_node/portable_node.hpp>

namespace pi::node {

struct SumNode : PortableNode {
  SumNode(
      int sid, const std::string& name, int port1_sid, int port2_sid, bool positive_port1,
      bool positive_port2
  );

  bool isPositivePort1() const noexcept;
  bool isPositivePort2() const noexcept;

private:
  const bool positive_port1_;
  const bool positive_port2_;
};

} // namespace pi::node
