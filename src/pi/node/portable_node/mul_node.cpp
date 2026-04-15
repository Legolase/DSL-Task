#include <pi/node/portable_node/mul_node.hpp>

namespace pi::node {

GainNode::GainNode(
    int sid, const std::string& name, int port_sid, const std::string& mul_value
) :
    PortableNode(sid, name, false, port_sid),
    mul_value_(mul_value)
{}

const std::string& GainNode::mulValue() const noexcept
{
  return mul_value_;
}

} // namespace pi::node