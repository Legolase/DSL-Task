#include <pi/node/portable_node/portable_node.hpp>

#include <concepts>

namespace pi::node {

void PortableNode::getEvalOrder(
    const std::function<PINode*(int)>& mapper, std::unordered_set<int>& visited,
    std::vector<int>& order
) const
{
  visited.insert(sid());
  for (auto child_sid : children_sids_) {
    if (!visited.contains(child_sid)) {
      auto* child_node = mapper(child_sid);

      if (!child_node->isFence()) {
        child_node->getEvalOrder(mapper, visited, order);
      }
    }
  }

  order.push_back(sid());
}

void PortableNode::getUnitDelayEvalOrder(
    const std::function<PINode*(int)>& mapper, std::unordered_set<int>& visited,
    std::vector<int>& unit_delay_sids_order
) const
{
  visited.insert(sid());
  for (auto child_sid : children_sids_) {
    if (!visited.contains(child_sid)) {
      mapper(child_sid)->getUnitDelayEvalOrder(mapper, visited, unit_delay_sids_order);
    }
  }
}

bool PortableNode::hasCyclesImpl(
    const std::function<PINode*(int)>& mapper,
    std::unordered_map<int, DFSNodeState>& visited
) const
{
  visited[sid()] = DFSNodeState::CAME_IN;

  for (auto child_sid : children_sids_) {
    auto it_visited = visited.find(child_sid);

    auto* child_node = mapper(child_sid);
    const auto is_fence_child = child_node->isFence();

    if (it_visited != visited.end() && !is_fence_child &&
        it_visited->second == DFSNodeState::CAME_IN)
    {
      return true;
    }
    if (it_visited == visited.end()) {
      if (is_fence_child) {
        visited[child_node->sid()] = DFSNodeState::LEFT;
      } else if (child_node->hasCyclesImpl(mapper, visited)) {
        return true;
      }
    }
  }

  visited[sid()] = DFSNodeState::LEFT;
  return false;
}

const std::vector<int>& PortableNode::childrenSids() const noexcept
{
  return children_sids_;
}

} // namespace pi::node