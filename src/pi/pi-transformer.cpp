#include <pi/pi-transformer.hpp>

namespace pi::transformer {

Transformer::Transformer(
    const pi::parser::PIParser& parser, const pi::generator::PICodeGenerator& generator
) noexcept :
    parser_(parser),
    generator_(generator)
{}

std::string Transformer::transform(const char* file_path) const
{
  return generator_.generate(parser_.parse(file_path));
}

} // namespace pi::transformer