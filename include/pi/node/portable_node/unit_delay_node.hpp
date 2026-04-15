#pragma once

#include <pi/node/portable_node/portable_node.hpp>

namespace pi::node {

struct UnitDelay final : PortableNode {
  UnitDelay(int sid, const std::string& name, int port_sid);

  virtual void getUnitDelayEvalOrder(
      const std::function<PINode*(int)>& mapper, std::unordered_set<int>& visited,
      std::vector<int>& unit_delay_sids_order
  ) const override;
};

} // namespace pi::node
