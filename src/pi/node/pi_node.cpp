#include <pi/node/pi_node.hpp>

namespace pi::node {

PINode::PINode(const int sid, const std::string& name, bool is_fence) :
    name_(name),
    sid_(sid),
    is_fence_(is_fence)
{}

int PINode::sid() const
{
  return sid_;
}

bool PINode::isFence() const
{
  return is_fence_;
}

std::string PINode::name() const
{
  return name_;
}

bool PINode::hasCycles(const std::function<PINode*(int)>& mapper) const
{
  std::unordered_map<int, DFSNodeState> visited;
  return hasCyclesImpl(mapper, visited);
}

} // namespace pi::node