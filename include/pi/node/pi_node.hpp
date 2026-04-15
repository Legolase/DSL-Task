#pragma once

#include <functional>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace pi::node {

enum class DFSNodeState : char {
  CAME_IN,
  LEFT
};

struct PINode {
  PINode(const int sid, const std::string& name, bool is_fence);
  virtual ~PINode() = default;

  std::string name() const;
  int sid() const;
  bool isFence() const;

  virtual void getEvalOrder(
      const std::function<PINode*(int)>& mapper, std::unordered_set<int>& visited,
      std::vector<int>& order
  ) const = 0;

  virtual void getUnitDelayEvalOrder(
      const std::function<PINode*(int)>& mapper, std::unordered_set<int>& visited,
      std::vector<int>& unit_delay_sids_order
  ) const = 0;

  bool hasCycles(const std::function<PINode*(int)>& mapper) const;

  virtual bool hasCyclesImpl(
      const std::function<PINode*(int)>& mapper,
      std::unordered_map<int, DFSNodeState>& visited
  ) const = 0;

private:
  const std::string name_;
  const int sid_;
  bool is_fence_;
};

} // namespace pi::node
