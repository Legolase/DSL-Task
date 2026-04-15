#pragma once

#include <pi/node/pi_node.hpp>

namespace pi::node {

struct StartNode final : PINode {
  StartNode(int sid, const std::string& name);

  virtual void getEvalOrder(
      const std::function<PINode*(int)>& mapper, std::unordered_set<int>& visited,
      std::vector<int>& order
  ) const override;

  virtual void getUnitDelayEvalOrder(
      const std::function<PINode*(int)>& mapper, std::unordered_set<int>& visited,
      std::vector<int>& unit_delay_sids_order
  ) const override;

  virtual bool hasCyclesImpl(
      const std::function<PINode*(int)>& mapper,
      std::unordered_map<int, DFSNodeState>& visited
  ) const override;
};

} // namespace pi::node
