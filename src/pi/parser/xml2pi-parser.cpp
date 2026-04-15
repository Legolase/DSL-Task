#include <defines.hpp>
#include <pi/node/portable_node/end_node.hpp>
#include <pi/node/portable_node/mul_node.hpp>
#include <pi/node/portable_node/sum_node.hpp>
#include <pi/node/portable_node/unit_delay_node.hpp>
#include <pi/node/start_node.hpp>
#include <pi/parser/xml2pi-parser.hpp>

#include <pugixml.hpp>

#include <algorithm>
#include <array>
#include <cstdlib>
#include <cstring>

namespace {

using StartNode = pi::node::StartNode;
using SumNode = pi::node::SumNode;
using GainNode = pi::node::GainNode;
using UnitDelay = pi::node::UnitDelay;
using EndNode = pi::node::EndNode;

struct SourcePorts {
  std::array<int, 2> port_sids = {-1, -1};
};

std::pair<int, int> parsePortData(const std::string& data)
{
  std::stringstream stream;
  stream << data;

  std::pair<int, int> result;
  stream >> result.first;
  stream.ignore(40, ':');
  stream >> result.second;

  LOG_DEBUG() << "Result: " << result.first << ' ' << result.second;

  return result;
}

void emplaceInportBlock(
    pi::PIController& controller, const pugi::xml_node& node, const int sid,
    const std::string& name
)
{
  auto pi_node = std::make_unique<StartNode>(sid, name);

  controller.start_node_sids.push_back(sid);

  if (controller.sid2Node.emplace(sid, pi_node.get()).second) {
    pi_node.release();
  }
}

void emplaceSumBlock(
    pi::PIController& controller, const std::unordered_map<int, SourcePorts>& dst2src,
    const pugi::xml_node& node, const int sid, const std::string& name
)
{
  const auto& inputs_node = node.find_child_by_attribute("P", "Name", "Inputs");
  const auto& inputs = inputs_node.empty() ? "++" : inputs_node.text().get();

  const auto& source_ports = dst2src.at(sid);

  auto pi_node = std::make_unique<SumNode>(
      sid, name, source_ports.port_sids[0], source_ports.port_sids[1], inputs[0] == '+',
      inputs[1] == '+'
  );

  if (controller.sid2Node.emplace(sid, pi_node.get()).second) {
    pi_node.release();
  }
}

void emplaceGainBlock(
    pi::PIController& controller, const std::unordered_map<int, SourcePorts>& dst2src,
    const pugi::xml_node& node, const int sid, const std::string& name
)
{
  const auto& gain_value = node.find_child_by_attribute("P", "Name", "Gain").text().get();

  const auto& source_ports = dst2src.at(sid);

  auto pi_node =
      std::make_unique<GainNode>(sid, name, source_ports.port_sids[0], gain_value);

  if (controller.sid2Node.emplace(sid, pi_node.get()).second) {
    pi_node.release();
  }
}

void emplaceUnitDelayBlock(
    pi::PIController& controller, const std::unordered_map<int, SourcePorts>& dst2src,
    const pugi::xml_node& node, const int sid, const std::string& name
)
{
  const auto& sample_time_node = node.find_child_by_attribute("P", "Name", "SampleTime");

  const auto& source_ports = dst2src.at(sid);

  if (std::strcmp(sample_time_node.text().get(), "-1") != 0) {
    LOG_ERROR() << "Unknown SampleTime value";
    return;
  }

  auto pi_node = std::make_unique<UnitDelay>(sid, name, source_ports.port_sids[0]);

  controller.unit_delay_sids.push_back(sid);

  if (controller.sid2Node.emplace(sid, pi_node.get()).second) {
    pi_node.release();
  }
}

void emplaceOutportBlock(
    pi::PIController& controller, const std::unordered_map<int, SourcePorts>& dst2src,
    const pugi::xml_node& node, const int sid, const std::string& name
)
{
  const auto& source_ports = dst2src.at(sid);

  auto pi_node = std::make_unique<EndNode>(sid, name, source_ports.port_sids[0]);

  controller.end_node_sids.push_back(sid);

  if (controller.sid2Node.emplace(sid, pi_node.get()).second) {
    pi_node.release();
  }
}

void expectValidController(const char* file_path, const pi::PIController& controller)
{
  if (controller.start_node_sids.empty()) {
    THROW(
        pi::parser::PIParserException,
        fmt::format(
            R"x(Error in format. Expected at least one inport in schema "{}")x", file_path
        )
    );
  }
  if (controller.end_node_sids.empty()) {
    THROW(
        pi::parser::PIParserException,
        fmt::format(
            R"x(Error in format. Expected at least one outport in schema "{}")x",
            file_path
        )
    );
  }

  const auto mapper = controller.mapper();

  const auto expect_no_cycles = [&](const int sid) {
    if (mapper(sid)->hasCycles(mapper)) {
      THROW(
          pi::parser::PIParserException,
          fmt::format(R"x(Detected raw cycles in schema "{}")x", file_path)
      );
    }
  };

  for (const auto end_sid : controller.end_node_sids) {
    expect_no_cycles(end_sid);
  }

  for (const auto unit_sid : controller.unit_delay_sids) {
    expect_no_cycles(unit_sid);
  }
}

} // namespace

namespace pi::parser {

PIController XML2PIParser::parse(const char* file_path) const
{
  PIController result;

  pugi::xml_document doc;
  auto parse_result = doc.load_file(file_path);
  if (!parse_result) {
    THROW(PIParserException, fmt::format(R"x(Invalid type if file {})x", file_path));
  }

  auto system_node = doc.child("System");

  std::unordered_map<int, SourcePorts> dst2src;

  auto addDest2Source = [&](const pugi::xml_node& line, const pugi::xml_node& source_node,
                            int source_sid) {
    auto p_dst_node = line.find_child_by_attribute("P", "Name", "Dst");
    auto dst_port_sid = parsePortData(p_dst_node.text().get());

    dst2src[dst_port_sid.first].port_sids[dst_port_sid.second - 1] = source_sid;
  };

  for (const auto& line : system_node.children("Line")) {
    auto p_src_node = line.find_child_by_attribute("P", "Name", "Src");
    auto src_port_sid = parsePortData(p_src_node.text().get());

    bool has_branch = false;
    for (const auto& branch_dest : line.children("Branch")) {
      has_branch = true;

      addDest2Source(branch_dest, p_src_node, src_port_sid.first);
    }

    if (!has_branch) {
      addDest2Source(line, p_src_node, src_port_sid.first);
    }
  }

  for (const auto& block : system_node.children("Block")) {
    const auto& type = block.attribute("BlockType");
    const auto sid = std::atoi(block.attribute("SID").value());
    std::string name = block.attribute("Name").value();
    std::replace_if(
        name.begin(), name.end(),
        [](const char value) {
          return value == ' ';
        },
        '_'
    );

    switch (type.value()[0]) {
    case 'I':
      emplaceInportBlock(result, block, sid, name);
      break;
    case 'S':
      emplaceSumBlock(result, dst2src, block, sid, name);
      break;
    case 'G':
      emplaceGainBlock(result, dst2src, block, sid, name);
      break;
    case 'U':
      emplaceUnitDelayBlock(result, dst2src, block, sid, name);
      break;
    case 'O':
      emplaceOutportBlock(result, dst2src, block, sid, name);
      break;
    default: {
      LOG_ERROR() << "Unknown block type: " << type.value();
      return result;
    }
    }
  }

  {
    std::unordered_set<int> visited;
    visited.reserve(result.sid2Node.size());

    decltype(result.unit_delay_sids) unit_delay_sids_ordered;
    unit_delay_sids_ordered.reserve(result.unit_delay_sids.size());

    const auto mapper = result.mapper();

    for (const auto end_node_index : result.end_node_sids) {
      const auto& pi_node = mapper(end_node_index);

      pi_node->getUnitDelayEvalOrder(mapper, visited, unit_delay_sids_ordered);
    }

    std::ranges::reverse(unit_delay_sids_ordered);

    result.unit_delay_sids = std::move(unit_delay_sids_ordered);
  }

  expectValidController(file_path, result);

  return result;
}

} // namespace pi::parser