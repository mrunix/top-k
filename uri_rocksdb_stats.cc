//
// Created by Michael on 2019/2/24.
//

#include <cstdio>
#include <string>
#include <iostream>

#include "rocksdb/slice.h"
#include "rocksdb/options.h"
#include "uri_rocksdb_stats.h"

std::string kDBPath = "./data/rocksdb";

// --------------------------------------------------------------------------------------------------------------------
// UriRocksDBStats
// --------------------------------------------------------------------------------------------------------------------
UriRocksDBStats::UriRocksDBStats(const std::string &filename)
    : filename_(filename), uri_file_reader_(filename), db_(nullptr) {
  Init();
}

UriRocksDBStats::~UriRocksDBStats() {
  Uninit();
}

void UriRocksDBStats::Init() {
  rocksdb::Options options;
  // Optimize RocksDB. This is the easiest way to get RocksDB to perform well
  options.IncreaseParallelism();
  options.OptimizeLevelStyleCompaction();
  // create the DB if it's not already present
  options.create_if_missing = true;

  // open DB
  rocksdb::Status s = rocksdb::DB::Open(options, kDBPath, &db_);
  assert(s.ok());
}

void UriRocksDBStats::Uninit() {
  delete db_;
}

void UriRocksDBStats::Process() {
  std::string uri;
  while (uri_file_reader_.HasNextUri()) {
    uri_file_reader_.GetNextUri(&uri);
    if (uri.length() <= 0 || (uri.length() == 1 && uri == "\r")) {
      continue;
    }

    Add(uri, 1);
  }

  UriCountsQueue uri_counts_queue;
  rocksdb::Iterator *it = db_->NewIterator(rocksdb::ReadOptions());
  for (it->SeekToFirst(); it->Valid(); it->Next()) {
    GetTopk(it->key().ToString(), it->value().ToString(), kTopKNumber, uri_counts_queue);
  }
  assert(it->status().ok()); // Check for any errors found during the scan
  delete it;

  size_t queue_size = uri_counts_queue.size();
  for (size_t i = 0; i < queue_size; i++) {
    top_uri_.push_back(uri_counts_queue.top());
    uri_counts_queue.pop();
  }

  PrintTopUri();
}

void UriRocksDBStats::GetTopk(const std::string &uri,
                              const std::string &value,
                              const int k,
                              UriCountsQueue &uri_counts_queue) {
  uint64_t v = Deserialize(value);
  if (uri_counts_queue.size() < k) {
    uri_counts_queue.push(UriCountsStats(uri, v));
  } else if (k > 0 && uri_counts_queue.top().counts_ < v) {
    uri_counts_queue.pop();
    uri_counts_queue.push(UriCountsStats(uri, v));
  }
}

// mapped to a RocksDB Put
void UriRocksDBStats::Set(const std::string &key, uint64_t value) {
  std::string serialized = Serialize(value);
  db_->Put(rocksdb::WriteOptions(), key, serialized);
}

// mapped to a RocksDB Delete
void UriRocksDBStats::Remove(const std::string &key) {
  db_->Delete(rocksdb::WriteOptions(), key);
}

// mapped to a RocksDB Get
bool UriRocksDBStats::Get(const std::string &key, uint64_t *value) {
  std::string str;
  auto s = db_->Get(rocksdb::ReadOptions(), key, &str);
  if (s.ok()) {
    *value = Deserialize(str);
    return true;
  } else {
    return false;
  }
}

// implemented as get -> modify -> set
void UriRocksDBStats::Add(const std::string &key, uint64_t value) {
  uint64_t base;
  if (!Get(key, &base)) {
    base = kDefaultCount;
  }
  Set(key, base + value);
}

std::string UriRocksDBStats::Serialize(uint64_t value) {
  std::string s;
  s = std::to_string(value);
  return std::move(s);
}

uint64_t UriRocksDBStats::Deserialize(const std::string &str) {
  char *end;
  return strtoull(str.c_str(), &end, 10);
}