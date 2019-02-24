//
// Created by Michael on 2019/2/23.
//

#include <fstream>
#include <iostream>
#include "uri_file_utils.h"
#include "uri_stats_types.h"

// --------------------------------------------------------------------------------------------------------------------
// UriFileReader
// --------------------------------------------------------------------------------------------------------------------
UriFileReader::UriFileReader(const std::string &filename)
    : filename_(filename), file_stream_(filename), read_uri_offset_(0) {
}

UriFileReader::~UriFileReader() {}

bool UriFileReader::HasNextUri() {
  file_stream_.seekg(read_uri_offset_);
  if (file_stream_.good() && !file_stream_.eof()) {
    return true;
  }

  return false;
}

void UriFileReader::GetNextUri(std::string *uri) {
  file_stream_.seekg(read_uri_offset_);
  if (file_stream_.good() && !file_stream_.eof()) {
    std::getline(file_stream_, *uri);
    read_uri_offset_ = file_stream_.tellg();
  }
}

// --------------------------------------------------------------------------------------------------------------------
// UriFileWriter
// --------------------------------------------------------------------------------------------------------------------
UriFileWriter::UriFileWriter() {}

UriFileWriter::~UriFileWriter() {
  Close();
}

UriFileWriter::UriFileWriter(const std::string &filename)
    : filename_(filename), file_stream_(filename) {
}

void UriFileWriter::Open(const std::string &name) {
  filename_ = name;
  file_stream_.open(name);
}

void UriFileWriter::Close() {
  file_stream_.close();
}

void UriFileWriter::AddUri(const std::string &uri) {
  file_stream_ << uri << std::endl;
}