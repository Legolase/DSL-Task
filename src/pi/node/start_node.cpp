#include <pi/node/start_node.hpp>

namespace pi::node {

StartNode::StartNode(int sid, const std::string& name) :
    PINode(sid, name, true)
{}

void StartNode::getEvalOrder(
    const std::function<PINode*(int)>& mapper, std::unordered_set<int>& visited,
    std::vector<int>& order
) const
{
  visited.insert(sid());
}

void StartNode::getUnitDelayEvalOrder(
    const std::function<PINode*(int)>& mapper, std::unordered_set<int>& visited,
    std::vector<int>& unit_delay_sids_order
) const
{
  visited.insert(sid());
}

bool StartNode::hasCyclesImpl(
    const std::function<PINode*(int)>& mapper,
    std::unordered_map<int, DFSNodeState>& visited
) const
{
  visited[sid()] = DFSNodeState::LEFT;

  return false;
}

} // namespace pi::node