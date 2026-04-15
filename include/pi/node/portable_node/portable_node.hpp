#pragma once

#include <defines.hpp>
#include <pi/node/pi_node.hpp>

#include <concepts>

namespace pi::node {

struct PortableNode : PINode {
  template<std::convertible_to<int>... Args>
  PortableNode(int sid, const std::string& name, bool is_fence, Args&&... args) :
      PINode(sid, name, is_fence)
  {
    children_sids_ = {std::forward<Args>(args)...};
    LOG_DEBUG() << children_sids_.size();
  }

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

  const std::vector<int>& childrenSids() const noexcept;

protected:
  std::vector<int> children_sids_;
};

} // namespace pi::node
