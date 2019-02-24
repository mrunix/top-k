//
// Created by Michael on 2019/2/24.
//

#ifndef TOP_K_URI_ROCKSDB_STATS_H
#define TOP_K_URI_ROCKSDB_STATS_H

#include <string>
#include "rocksdb/db.h"
#include "uri_file_utils.h"
#include "uri_stats_types.h"

// --------------------------------------------------------------------------------------------------------------------
// UriRocksDBStats
// --------------------------------------------------------------------------------------------------------------------
class UriRocksDBStats : public BaseUriStats, public Counters {
 public:
  explicit UriRocksDBStats(const std::string &filename);
  ~UriRocksDBStats();

  void Process() override;

  UriRocksDBStats(const UriRocksDBStats &) = delete;
  UriRocksDBStats &operator=(const UriRocksDBStats &) = delete;

 private:
  void Init();
  void Uninit();

  // mapped to a RocksDB Put
  void Set(const std::string &key, uint64_t value) override;

  // mapped to a RocksDB Delete
  void Remove(const std::string &key) override;

  // mapped to a RocksDB Get
  bool Get(const std::string &key, uint64_t *value) override;

  // implemented as get -> modify -> set
  void Add(const std::string &key, uint64_t value) override;

  std::string Serialize(uint64_t value);
  uint64_t Deserialize(const std::string &str);

  void GetTopk(const std::string &uri, const std::string &value, const int k, UriCountsQueue &uri_counts_queue);

  static const uint64_t kDefaultCount = 0;

  std::string filename_;
  UriFileReader uri_file_reader_;
  rocksdb::DB *db_;
};

#endif //TOP_K_URI_ROCKSDB_STATS_H
