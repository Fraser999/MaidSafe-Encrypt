/*******************************************************************************
 *  Copyright 2009 maidsafe.net limited                                        *
 *                                                                             *
 *  The following source code is property of maidsafe.net limited and is not   *
 *  meant for external use.  The use of this code is governed by the license   *
 *  file LICENSE.TXT found in the root of this directory and also on           *
 *  www.maidsafe.net.                                                          *
 *                                                                             *
 *  You are not free to copy, amend or otherwise use this source code without  *
 *  the explicit written permission of the board of directors of maidsafe.net. *
 ***************************************************************************//**
 * @file  test_file_io_handler.cc
 * @brief Tests for the interface to handle file IO operations.
 * @date  2009-10-25
 */

#include <cstdint>
#include <limits>
#include <memory>

#include "gtest/gtest.h"
#include "boost/filesystem/fstream.hpp"
#include "boost/filesystem.hpp"
#include "maidsafe-dht/common/utils.h"
#include "maidsafe-encrypt/data_io_handler.h"

namespace maidsafe {

namespace encrypt {

namespace test {

namespace test_file_io_handler {
// TODO(Fraser#5#): Replace with fs::temp_directory_path() from boost 1.45
fs::path TempDir() {
#if defined(PD_WIN32)
  fs::path temp_dir("");
  if (std::getenv("TEMP"))
    temp_dir = std::getenv("TEMP");
  else if (std::getenv("TMP"))
    temp_dir = std::getenv("TMP");
#elif defined(P_tmpdir)
  fs::path temp_dir(P_tmpdir);
#else
  fs::path temp_dir("");
  if (std::getenv("TMPDIR")) {
    temp_dir = std::getenv("TMPDIR");
  } else {
    temp_dir = fs::path("/tmp");
    try {
      if (!fs::exists(temp_dir))
        temp_dir.clear();
    }
    catch(const std::exception &e) {
#ifdef DEBUG
      printf("In TempDir: %s\n", e.what());
#endif
      temp_dir.clear();
    }
  }
#endif
  size_t last_char = temp_dir.string().size() - 1;
  if (temp_dir.string()[last_char] == '/' ||
      temp_dir.string()[last_char] == '\\') {
    std::string temp_str = temp_dir.string();
    temp_str.resize(last_char);
    temp_dir = fs::path(temp_str);
  }
  return temp_dir;
}
}  // namespace test_file_io_handler

class FileIOHandlerTest : public testing::Test {
 public:
  FileIOHandlerTest()
      : kRootDir_(test_file_io_handler::TempDir() /
            ("maidsafe_TestIO_" + RandomAlphaNumericString(6))),
        kInputFile_(kRootDir_ / "In.txt"),
        kOutputFile_(kRootDir_ / "Out.txt"),
        kMinSize_(10),
        kDataSize_((RandomUint32() % 249) + kMinSize_),
        // ensure input contains null chars
        kData_(std::string(10, 0) + RandomString(kDataSize_ - 10)) {}
 protected:
  void SetUp() {
    try {
      if (fs::exists(kRootDir_))
        fs::remove_all(kRootDir_);
      fs::create_directories(kRootDir_);
    }
    catch(const std::exception& e) {
      printf("%s\n", e.what());
    }
  }
  void TearDown() {
    try {
      if (fs::exists(kRootDir_))
        fs::remove_all(kRootDir_);
    }
    catch(const std::exception& e) {
      printf("%s\n", e.what());
    }
  }
  void WriteDataToInputFile(bool empty_file) {
    try {
      fs::ofstream out_file(kInputFile_,
                            fs::ofstream::binary | fs::ofstream::trunc);
      if (!empty_file)
        out_file.write(kData_.c_str(), kDataSize_);
      out_file.close();
    }
    catch(const std::exception&) {
    }
  }
  std::string ReadDataFromOutputFile() {
    std::uint64_t file_size(fs::file_size(kOutputFile_));
    if (file_size > std::numeric_limits<size_t>::max())
      return "";
    size_t size = static_cast<size_t>(file_size);
    std::shared_ptr<char> data(new char[size]);
    try {
      fs::ifstream in_file(kOutputFile_, fs::ofstream::binary);
      in_file.read(data.get(), size);
      in_file.close();
    }
    catch(const std::exception&) {
    }
    return std::string(data.get(), size);
  }
  const fs::path kRootDir_, kInputFile_, kOutputFile_;
  const size_t kMinSize_, kDataSize_;
  const std::string kData_;
};

TEST_F(FileIOHandlerTest, BEH_ENCRYPT_TestReadFromFile) {
  WriteDataToInputFile(false);

  // Check using non-existant file
  FileIOHandler nef_input_handler(fs::path("k.txt"), true);
  EXPECT_FALSE(nef_input_handler.Open());
  std::uint64_t tempsize(999);
  EXPECT_FALSE(nef_input_handler.Size(&tempsize));
  EXPECT_EQ(0U, tempsize);
  EXPECT_FALSE(nef_input_handler.Write("a"));
  size_t test_size(kMinSize_ / 10);
  std::string read_data("Test");
  EXPECT_FALSE(nef_input_handler.Read(test_size, &read_data));
  EXPECT_TRUE(read_data.empty());
  EXPECT_FALSE(nef_input_handler.SetGetPointer(test_size));

  // Check before opening
  FileIOHandler input_handler(kInputFile_, true);
  EXPECT_TRUE(input_handler.Size(&tempsize));
  EXPECT_EQ(kDataSize_, tempsize);
  EXPECT_FALSE(input_handler.Write("a"));
  read_data = "Test";
  EXPECT_FALSE(input_handler.Read(test_size, &read_data));
  EXPECT_TRUE(read_data.empty());
  EXPECT_FALSE(input_handler.SetGetPointer(test_size));

  // Check after opening
  EXPECT_TRUE(input_handler.Open());
  EXPECT_FALSE(input_handler.Write("a"));
  read_data = "Test";
  EXPECT_TRUE(input_handler.Read(test_size, &read_data));
  EXPECT_EQ(kData_.substr(0, test_size), read_data);

  // Read again
  read_data.clear();
  EXPECT_TRUE(input_handler.Read(test_size, &read_data));
  EXPECT_EQ(kData_.substr(test_size, test_size), read_data);

  // Read past eof
  EXPECT_TRUE(input_handler.Read(kDataSize_, &read_data));
  EXPECT_EQ(kData_.substr(2 * test_size), read_data);

  // Read again
  EXPECT_TRUE(input_handler.Read(test_size, &read_data));
  EXPECT_TRUE(read_data.empty());

  // Check re-opening while open has no effect
  EXPECT_TRUE(input_handler.Open());
  read_data = "Test";
  EXPECT_TRUE(input_handler.Read(test_size, &read_data));
  EXPECT_TRUE(read_data.empty());

  // Check after closing
  input_handler.Close();
  read_data = "Test";
  EXPECT_FALSE(input_handler.Read(test_size, &read_data));
  EXPECT_TRUE(read_data.empty());

  // Check re-opening sets get pointer back to 0
  EXPECT_TRUE(input_handler.Open());
  EXPECT_TRUE(input_handler.Read(test_size, &read_data));
  EXPECT_EQ(kData_.substr(0, test_size), read_data);

  // Check empty string handling
  WriteDataToInputFile(true);
  FileIOHandler empty_input_handler(kInputFile_, true);
  EXPECT_TRUE(empty_input_handler.Size(&tempsize));
  EXPECT_EQ(0U, tempsize);
  EXPECT_TRUE(empty_input_handler.Open());
  read_data = "Test";
  EXPECT_TRUE(empty_input_handler.Read(test_size, &read_data));
  EXPECT_TRUE(read_data.empty());
}

TEST_F(FileIOHandlerTest, BEH_ENCRYPT_TestSetGetPointerFile) {
  WriteDataToInputFile(false);

  // Open and apply offset
  FileIOHandler input_handler(kInputFile_, true);
  size_t test_size(kMinSize_ / 10), offset(kMinSize_ / 2);
  EXPECT_FALSE(input_handler.SetGetPointer(test_size));
  std::string read_data;
  EXPECT_TRUE(input_handler.Open());
  EXPECT_TRUE(input_handler.SetGetPointer(offset));
  EXPECT_TRUE(input_handler.Read(test_size, &read_data));
  EXPECT_EQ(kData_.substr(offset, test_size), read_data);

  // Retry with different offset
  offset = kMinSize_ / 3;
  EXPECT_TRUE(input_handler.SetGetPointer(offset));
  EXPECT_TRUE(input_handler.Read(test_size, &read_data));
  EXPECT_EQ(kData_.substr(offset, test_size), read_data);

  // Retry with offset > file size
  offset = kDataSize_ + 1;
  EXPECT_TRUE(input_handler.SetGetPointer(offset));
  EXPECT_TRUE(input_handler.Read(test_size, &read_data));
  EXPECT_TRUE(read_data.empty());
}

TEST_F(FileIOHandlerTest, BEH_ENCRYPT_WriteToFile) {
  // Check using non-existant directory
  FileIOHandler nef_output_handler(fs::path("not/o/k.txt"), false);
  EXPECT_FALSE(nef_output_handler.Open());
  std::uint64_t tempsize(999);
  EXPECT_FALSE(nef_output_handler.Size(&tempsize));
  EXPECT_EQ(0U, tempsize);
  EXPECT_FALSE(nef_output_handler.Write("a"));
  size_t test_size(kMinSize_ / 10);
  std::string read_data("Test");
  EXPECT_FALSE(nef_output_handler.Read(test_size, &read_data));
  EXPECT_TRUE(read_data.empty());
  EXPECT_FALSE(nef_output_handler.SetGetPointer(test_size));

  // Check before opening
  FileIOHandler output_handler(kOutputFile_, false);
  EXPECT_FALSE(output_handler.Size(&tempsize));
  EXPECT_EQ(0U, tempsize);
  EXPECT_FALSE(output_handler.Write("a"));
  read_data = "Test";
  EXPECT_FALSE(output_handler.Read(test_size, &read_data));
  EXPECT_TRUE(read_data.empty());
  EXPECT_FALSE(output_handler.SetGetPointer(test_size));

  // Check after opening
  EXPECT_TRUE(output_handler.Open());
  EXPECT_TRUE(output_handler.Size(&tempsize));
  EXPECT_EQ(0U, tempsize);
  read_data = "Test";
  EXPECT_FALSE(output_handler.Read(test_size, &read_data));
  EXPECT_TRUE(read_data.empty());
  size_t split(RandomUint32() % kDataSize_);
  std::string part1(kData_.substr(0, split)), part2(kData_.substr(split));
  EXPECT_TRUE(output_handler.Write(part1));
//  EXPECT_EQ(part1, ReadDataFromOutputFile());
  EXPECT_TRUE(output_handler.Size(&tempsize));
  EXPECT_EQ(split, tempsize);

  // Write again
  EXPECT_TRUE(output_handler.Write(part2));
//  EXPECT_EQ(kData_, ReadDataFromOutputFile());
  EXPECT_TRUE(output_handler.Size(&tempsize));
  EXPECT_EQ(kDataSize_, tempsize);

  // Write with empty string
  EXPECT_TRUE(output_handler.Write(""));
//  EXPECT_EQ(kData_, ReadDataFromOutputFile());
  EXPECT_TRUE(output_handler.Size(&tempsize));
  EXPECT_EQ(kDataSize_, tempsize);

  // Check re-opening while open has no effect
  EXPECT_TRUE(output_handler.Open());
//  EXPECT_EQ(kData_, ReadDataFromOutputFile());
  EXPECT_TRUE(output_handler.Size(&tempsize));
  EXPECT_EQ(kDataSize_, tempsize);
  EXPECT_TRUE(output_handler.Write("a"));
//  EXPECT_EQ(kData_ + "a", ReadDataFromOutputFile());
  EXPECT_TRUE(output_handler.Size(&tempsize));
  EXPECT_EQ(kDataSize_ + 1, tempsize);

  // Check after closing
  output_handler.Close();
  EXPECT_EQ(kData_ + "a", ReadDataFromOutputFile());
  EXPECT_TRUE(output_handler.Size(&tempsize));
  EXPECT_EQ(kDataSize_ + 1, tempsize);
  EXPECT_FALSE(output_handler.Write("b"));
  EXPECT_EQ(kData_ + "a", ReadDataFromOutputFile());
  EXPECT_TRUE(output_handler.Size(&tempsize));
  EXPECT_EQ(kDataSize_ + 1, tempsize);

  // Check re-opening after closing clears data
  EXPECT_TRUE(output_handler.Open());
  EXPECT_TRUE(ReadDataFromOutputFile().empty());
  EXPECT_TRUE(output_handler.Size(&tempsize));
  EXPECT_EQ(0U, tempsize);
  EXPECT_TRUE(output_handler.Write(kData_));
  EXPECT_EQ(kData_, ReadDataFromOutputFile());
  EXPECT_TRUE(output_handler.Size(&tempsize));
  EXPECT_EQ(kDataSize_, tempsize);
}

}  // namespace test

}  // namespace encrypt

}  // namespace maidsafe