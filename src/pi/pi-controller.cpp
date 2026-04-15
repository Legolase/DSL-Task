#include <pi/pi-controller.hpp>

#include <fmt/format.h>

namespace pi {

PIController::Mapper::Mapper(const PIController& controller) noexcept :
    controller_(controller)
{}

node::PINode* PIController::Mapper::operator()(const int sid) const
{
  auto it = controller_.sid2Node.find(sid);

  if (it == controller_.sid2Node.end()) {
    THROW(PIControllerGetterException, "Exception occured taking node by sids. Check how you create a PI controller.");
  }

  return it->second.get();
}

PIController::Mapper PIController::mapper() const
{
  return Mapper(*this);
}

} // namespace pi