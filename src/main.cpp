#include <defines.hpp>
#include <pi/generator/pi-c-generator.hpp>
#include <pi/parser/xml2pi-parser.hpp>
#include <pi/pi-transformer.hpp>
#include <return_codes.h>

#include <pugixml.hpp>

int main(int argc, char* argv[])
{
  if (argc != 2) {
    LOG_ERROR() << "Expected: <program> <source-xml>";
    return ERROR_INVALID_PARAMETER;
  }

  const char* xml_file_path = argv[1];

  try {
    auto parser = pi::parser::XML2PIParser{};
    auto generator = pi::generator::PICGenerator{};
    auto tran = pi::transformer::Transformer(parser, generator);

    std::cout << tran.transform(xml_file_path);
  } catch (const std::runtime_error& e) {
    LOG_ERROR() << "Catched exception: " << e.what();
    return ERROR_UNKNOWN;
  }

  return ERROR_SUCCESS;
}