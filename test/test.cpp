#include <defines.hpp>
#include <pi/generator/pi-c-generator.hpp>
#include <pi/parser/xml2pi-parser.hpp>
#include <pi/pi-transformer.hpp>

#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>

namespace {

std::string getFileData(const char* file_path)
{
  namespace fs = std::filesystem;

  std::ifstream file(file_path, std::ios::binary);
  const auto size = fs::file_size(file_path);

  std::string result(size, '\0');
  file.read(result.data(), size);

  return result;
}

std::string generateCode(const char* file_path)
{
  auto parser = pi::parser::XML2PIParser{};
  auto generator = pi::generator::PICGenerator{};
  pi::transformer::Transformer tran(parser, generator);

  return tran.transform(file_path);
}

bool isFileExists(const char* file_path)
{
  struct stat buffer;
  return (stat(file_path, &buffer) == 0);
}

} // namespace

TEST(BASE_XML, Example)
{
  try {
    EXPECT_EQ(
        generateCode("./static/test/system1.xml"), getFileData("./static/test/system1.c")
    );
  } catch (const std::runtime_error& e) {
    FAIL() << "Unexpected error: " << e.what();
  }
}

TEST(BASE_XML, SuccesResult)
{
  constexpr int test_ids[] = {2, 3, 4, 9};
  for (const auto i : test_ids) {
    auto input_file = fmt::format("./static/test/system{}.xml", i);
    auto output_file = fmt::format("./static/test/system{}.c", i);

    LOG_INFO() << "Test file: " << input_file;

    if (!isFileExists(input_file.c_str()) || !isFileExists(output_file.c_str())) {
      FAIL() << "File " << input_file << " or " << output_file << " doesn't exists.";
    }

    try {
      EXPECT_EQ(generateCode(input_file.c_str()), getFileData(output_file.c_str()));
    } catch (const std::runtime_error& e) {
      FAIL() << fmt::format(
          R"x(Unexpected error while testing file "{}": {})x", input_file, e.what()
      );
    }
  }
}

TEST(BASE_XML, ErrorResult)
{
  constexpr int test_ids[] = {5, 6, 7, 8, 10, 11};
  for (const auto i : test_ids) {
    auto input_file = fmt::format("./static/test/system{}.xml", i);

    LOG_INFO() << "Test file: " << input_file;

    if (!isFileExists(input_file.c_str())) {
      FAIL() << "File " << input_file << " doesn't exists.";
    }

    try {
      const auto& result = generateCode(input_file.c_str());
      LOG_INFO() << "Result: ";
      LOG_INFO() << "\n" << result << "\n===========================";
    } catch (const std::runtime_error& e) {
      LOG_INFO() << "Expected error catched: " << e.what();
      continue;
    }
    FAIL() << fmt::format(
        R"x(Expected error not occured while testing file "{}")x", input_file
    );
  }
}

TEST(BASE_XML, InvalidInputFile)
{
  std::string input_file = "./static/test/system1.c";

  if (!isFileExists(input_file.c_str())) {
    FAIL() << "File " << input_file << " doesn't exists.";
  }

  try {
    const auto& result = generateCode(input_file.c_str());
    LOG_INFO() << "Result: ";
    LOG_INFO() << "\n" << result << "\n===========================";
  } catch (const std::runtime_error& e) {
    LOG_INFO() << "Expected error catched: " << e.what();
    return;
  }
  FAIL(
  ) << fmt::format(R"x(Expected error not occured while testing file "{}")x", input_file);
}

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}