#pragma once

#include <defines.hpp>
#include <pi/node/pi_node.hpp>

#include <memory>
#include <unordered_map>

namespace pi {

DECLARE_EXCEPTION(PIControllerGetterException);

struct PIController {

  struct Mapper {
    explicit Mapper(const PIController& controller) noexcept;

    node::PINode* operator()(const int sid) const;

  private:
    const PIController& controller_;
  };

  Mapper mapper() const;

  std::unordered_map<int, std::unique_ptr<node::PINode>> sid2Node;
  std::vector<int> start_node_sids;
  std::vector<int> unit_delay_sids;
  std::vector<int> end_node_sids;
};

} // namespace pi
