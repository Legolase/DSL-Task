#pragma once
#include <pi/node/portable_node/portable_node.hpp>

#include <concepts>

namespace pi::node {

struct EndNode : PortableNode {
  EndNode(int sid, const std::string& name, int port_sid);
};

} // namespace pi::node
