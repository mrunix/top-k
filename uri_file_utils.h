//
// Created by Michael on 2019/2/23.
//

#ifndef TOP_K_URI_FILE_UTILS_H
#define TOP_K_URI_FILE_UTILS_H

#include <fstream>
#include <string>

// --------------------------------------------------------------------------------------------------------------------
// UriFileReader
// --------------------------------------------------------------------------------------------------------------------
class UriFileReader {
 public:
  explicit UriFileReader(const std::string &filename);
  ~UriFileReader();

  bool HasNextUri();
  void GetNextUri(std::string *uri);

  UriFileReader(const UriFileReader &) = delete;
  UriFileReader &operator=(const UriFileReader &) = delete;

 private:
  std::string filename_;
  std::ifstream file_stream_;
  std::streampos read_uri_offset_;  // read offset
};

// --------------------------------------------------------------------------------------------------------------------
// UriFileWriter
// --------------------------------------------------------------------------------------------------------------------
class UriFileWriter {
 public:
  UriFileWriter();
  explicit UriFileWriter(const std::string &filename);
  ~UriFileWriter();

  void Open(const std::string &name);
  void Close();
  void AddUri(const std::string &uri);

  UriFileWriter(const UriFileWriter &) = delete;
  UriFileWriter &operator=(const UriFileWriter &) = delete;

 private:
  std::string filename_;
  std::ofstream file_stream_;
};

#endif //TOP_K_URI_FILE_UTILS_H
