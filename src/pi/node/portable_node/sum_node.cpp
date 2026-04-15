#include <pi/node/portable_node/sum_node.hpp>

namespace pi::node {
SumNode::SumNode(
    int sid, const std::string& name, int port1_sid, int port2_sid, bool positive_port1,
    bool positive_port2
) :
    PortableNode(sid, name, false, port1_sid, port2_sid),
    positive_port1_(positive_port1),
    positive_port2_(positive_port2)
{}

bool SumNode::isPositivePort1() const noexcept
{
  return positive_port1_;
}

bool SumNode::isPositivePort2() const noexcept
{
  return positive_port2_;
}

} // namespace pi::node