//
// Created by Michael on 2019/2/23.
//

#ifndef TOP_K_URI_STATS_H
#define TOP_K_URI_STATS_H

#include <string>
#include "uri_file_utils.h"
#include "uri_stats_types.h"

// --------------------------------------------------------------------------------------------------------------------
// UriFileStats
// --------------------------------------------------------------------------------------------------------------------
class UriFileStats : public BaseUriStats {
 public:
  explicit UriFileStats(const std::string &filename);
  ~UriFileStats();

  void Process() override;

  UriFileStats(const UriFileStats &) = delete;
  UriFileStats &operator=(const UriFileStats &) = delete;

 private:
  void GroupUriByHashcode();
  int GetTopUriFromGroupedFile();
  void AddCounts(const std::string &uri, UriCountsMap &uri_counts);
  void GetTopk(const UriCountsMap &uri_counts_map, const int k, UriCountsQueue &uri_counts_queue);
  uint64_t Hashcode(const std::string &uri);

  static const int kFileNumber = 100;

  std::string filename_;
  UriFileReader uri_file_reader_;
  UriFileWriter uri_file_writer_[kFileNumber];
};

#endif //TOP_K_URI_STATS_H
