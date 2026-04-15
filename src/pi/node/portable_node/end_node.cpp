#include <pi/node/portable_node/end_node.hpp>

namespace pi::node {

EndNode::EndNode(int sid, const std::string& name, int port_sid) :
    PortableNode(sid, name, false, port_sid)
{}

} // namespace pi::node