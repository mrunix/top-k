//
// Created by Michael on 2019/2/23.
//

#include <iostream>
#include <vector>
#include <queue>
#include "uri_file_stats.h"
#include "uri_stats_types.h"
#include "murmur_hash.h"

// --------------------------------------------------------------------------------------------------------------------
// UriFileStats
// --------------------------------------------------------------------------------------------------------------------
UriFileStats::UriFileStats(const std::string &filename)
    : filename_(filename), uri_file_reader_(filename) {
}

UriFileStats::~UriFileStats() {
}

void UriFileStats::Process() {
  GroupUriByHashcode();
  GetTopUriFromGroupedFile();
  PrintTopUri();
}

uint64_t UriFileStats::Hashcode(const std::string &uri) {
  return MurmurHash64A(uri.c_str(), uri.length(), 101);
}

void UriFileStats::GroupUriByHashcode() {
  for (int i = 0; i < kFileNumber; ++i) {
    std::string name = filename_ + std::to_string(i);
    uri_file_writer_[i].Open(name);
  }

  std::string uri;
  while (uri_file_reader_.HasNextUri()) {
    uri_file_reader_.GetNextUri(&uri);
    if (uri.length() <= 0 || (uri.length() == 1 && uri == "\r")) {
      continue;
    }

    uint64_t hashcode = Hashcode(uri);
    int num = hashcode % kFileNumber;
    uri_file_writer_[num].AddUri(uri);
  }

  for (int i = 0; i < kFileNumber; ++i) {
    uri_file_writer_[i].Close();
  }
}

int UriFileStats::GetTopUriFromGroupedFile() {
  std::string uri;
  UriCountsMap uri_counts_map;
  UriCountsQueue uri_counts_queue;

  for (int i = 0; i < kFileNumber; i++) {
    std::string name = filename_ + std::to_string(i);
    UriFileReader reader(name);
    uri_counts_map.clear();
    while (reader.HasNextUri()) {
      reader.GetNextUri(&uri);
      if (uri.length() <= 0 || (uri.length() == 1 && uri == "\r")) {
        continue;
      }
      AddCounts(uri, uri_counts_map);
    }

    GetTopk(uri_counts_map, kTopKNumber, uri_counts_queue);
  }

  size_t queue_size = uri_counts_queue.size();
  for (size_t i = 0; i < queue_size; i++) {
    top_uri_.push_back(uri_counts_queue.top());
    uri_counts_queue.pop();
  }
}

void UriFileStats::AddCounts(const std::string &uri, UriCountsMap &uri_counts) {
  auto it = uri_counts.find(uri);
  if (it != uri_counts.end()) {
    it->second++;
  } else {
    uri_counts[uri] = 1;
  }
}

void UriFileStats::GetTopk(const UriCountsMap &uri_counts_map, const int k, UriCountsQueue &uri_counts_queue) {
  for (auto &it : uri_counts_map) {
    if (uri_counts_queue.size() < k) {
      uri_counts_queue.push(UriCountsStats(it.first, it.second));
    } else if (k > 0 && uri_counts_queue.top().counts_ < it.second) {
      uri_counts_queue.pop();
      uri_counts_queue.push(UriCountsStats(it.first, it.second));
    }
  }
}