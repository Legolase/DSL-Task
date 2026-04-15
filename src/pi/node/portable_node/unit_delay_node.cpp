#include <pi/node/portable_node/unit_delay_node.hpp>

namespace pi::node {

UnitDelay::UnitDelay(int sid, const std::string& name, int port_sid) :
    PortableNode(sid, name, true, port_sid)
{}

void UnitDelay::getUnitDelayEvalOrder(
    const std::function<PINode*(int)>& mapper, std::unordered_set<int>& visited,
    std::vector<int>& unit_delay_sids_order
) const
{
  PortableNode::getUnitDelayEvalOrder(mapper, visited, unit_delay_sids_order);

  unit_delay_sids_order.push_back(sid());
}

} // namespace pi::node