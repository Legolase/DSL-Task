#include <pi/generator/pi-c-generator.hpp>
#include <pi/node/portable_node/end_node.hpp>
#include <pi/node/portable_node/mul_node.hpp>
#include <pi/node/portable_node/sum_node.hpp>
#include <pi/node/portable_node/unit_delay_node.hpp>
#include <pi/node/start_node.hpp>

namespace {

namespace literals {

inline constexpr char SYSTEM_STRUCT_PREFIX[] = "nwocg.";

inline constexpr char DECLARATION[] = R"x(#include "nwocg_run.h"
#include <math.h>

static struct
{
)x";

inline constexpr char INITIALIZATION[] = R"x(} nwocg;

void nwocg_generated_init()
{
)x";

inline constexpr char GENERATED_STEPS[] = R"x(}

void nwocg_generated_step()
{
)x";

inline constexpr char PORT_POINTERS[] = R"x(}

static const nwocg_ExtPort
  ext_ports[] =
{
)x";

inline constexpr char PORTS_AND_SIZES[] = R"x(};

const nwocg_ExtPort * const
  nwocg_generated_ext_ports = ext_ports;

const size_t
  nwocg_generated_ext_ports_size = sizeof(ext_ports);
)x";

} // namespace literals

void initNwocgStruct(std::ostream& stream, const pi::PIController& controller)
{
  for (const auto& [sid, pi_node] : controller.sid2Node) {
    if (dynamic_cast<pi::node::EndNode*>(pi_node.get())) {
      continue;
    }
    stream << "  double " << pi_node->name() << ";\n";
  }
}

void initUnitDelayFields(std::ostream& stream, const pi::PIController& controller)
{
  for (const auto unit_delay_index : controller.unit_delay_sids) {
    const auto& pi_node = controller.sid2Node.at(unit_delay_index);

    stream << "  nwocg." << pi_node->name() << " = 0;\n";
  }
}

void processNode(
    std::ostream& stream, const auto& mapper, const pi::node::GainNode& gain_node
)
{
  using namespace literals;

  auto* port_node = mapper(gain_node.childrenSids()[0]);

  stream << "  " << SYSTEM_STRUCT_PREFIX << gain_node.name() << " = ";
  stream << SYSTEM_STRUCT_PREFIX << port_node->name() << " * ";
  stream << gain_node.mulValue() << ";\n";
}

void processNode(
    std::ostream& stream, const auto& mapper, const pi::node::SumNode& sum_node
)
{
  using namespace literals;

  auto* port_node1 = mapper(sum_node.childrenSids()[0]);
  auto* port_node2 = mapper(sum_node.childrenSids()[1]);

  stream << "  " << SYSTEM_STRUCT_PREFIX << sum_node.name()
         << ((sum_node.isPositivePort1()) ? " = " : " = -");
  stream << SYSTEM_STRUCT_PREFIX << port_node1->name();
  stream << ((sum_node.isPositivePort2()) ? " + " : " - ");
  stream << SYSTEM_STRUCT_PREFIX << port_node2->name() << ";\n";
}

void processNode(
    std::ostream& stream, const auto& mapper, const pi::node::UnitDelay& unit_delay_node
)
{
  using namespace literals;

  auto* port_node = mapper(unit_delay_node.childrenSids()[0]);

  stream << "  " << SYSTEM_STRUCT_PREFIX << unit_delay_node.name() << " = ";
  stream << SYSTEM_STRUCT_PREFIX << port_node->name() << ";\n";
}

void generateEvaluationSteps(std::ostream& stream, const pi::PIController& controller)
{
  std::vector<int> unit_delay_evaluation_order;
  unit_delay_evaluation_order.reserve(controller.unit_delay_sids.size());

  std::unordered_set<int> visited;
  visited.reserve(controller.sid2Node.size());

  const auto mapper = controller.mapper();

  bool is_first_node = true;

  std::vector<int> eval_order;
  eval_order.reserve(
      controller.sid2Node.size() + controller.end_node_sids.size() +
      controller.unit_delay_sids.size()
  );

  for (const auto end_node_index : controller.end_node_sids) {
    const auto& pi_node = controller.sid2Node.at(end_node_index);

    if (is_first_node) {
      is_first_node = false;
    } else {
      eval_order.push_back(-1);
    }

    pi_node->getEvalOrder(mapper, visited, eval_order);
  }

  for (const auto unit_delay_index : controller.unit_delay_sids) {
    const auto& pi_node = controller.sid2Node.at(unit_delay_index);

    eval_order.push_back(-1);

    pi_node->getEvalOrder(mapper, visited, eval_order);
  }

  for (const auto node_index : eval_order) {
    if (node_index == -1) {
      stream << '\n';
      continue;
    }

    const auto& pi_node = mapper(node_index);

    if (const auto& sum_node = dynamic_cast<pi::node::SumNode*>(pi_node)) {
      processNode(stream, mapper, *sum_node);
    } else if (const auto& gain_node = dynamic_cast<pi::node::GainNode*>(pi_node)) {
      processNode(stream, mapper, *gain_node);
    } else if (const auto& unit_delay_node = dynamic_cast<pi::node::UnitDelay*>(pi_node))
    {
      processNode(stream, mapper, *unit_delay_node);
    }
  }
}

void initExtPorts(std::ostream& stream, const pi::PIController& controller)
{
  const auto mapper = controller.mapper();

  for (const auto end_index : controller.end_node_sids) {
    const auto& pi_node = mapper(end_index);
    const auto& portable_node = dynamic_cast<pi::node::PortableNode*>(pi_node);
    const auto& source_pi_node = mapper(portable_node->childrenSids()[0]);

    stream << "  { \"" << pi_node->name() << "\", &nwocg." << source_pi_node->name()
           << ", 0 },\n";
  }

  for (const auto start_index : controller.start_node_sids) {
    const auto& name = mapper(start_index)->name();

    stream << "  { \"" << name << "\", &nwocg." << name << ", 1 },\n";
  }

  stream << "  { 0, 0, 0 }\n";
}

} // namespace

namespace pi::generator {

std::string PICGenerator::generate(const pi::PIController& controller) const
{
  std::stringstream stream;

  stream << literals::DECLARATION;

  initNwocgStruct(stream, controller);

  stream << literals::INITIALIZATION;

  initUnitDelayFields(stream, controller);

  stream << literals::GENERATED_STEPS;

  generateEvaluationSteps(stream, controller);

  stream << literals::PORT_POINTERS;

  initExtPorts(stream, controller);

  stream << literals::PORTS_AND_SIZES;

  return stream.str();
}

} // namespace pi::generator