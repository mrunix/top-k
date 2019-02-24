//
// Created by Michael on 2019/2/23.
//

#ifndef TOP_K_URI_STATS_TYPES_H
#define TOP_K_URI_STATS_TYPES_H

#include <cstdint>
#include <iosfwd>
#include <unordered_map>
#include <queue>

// --------------------------------------------------------------------------------------------------------------------
// UriCountsStats
// --------------------------------------------------------------------------------------------------------------------
class UriCountsStats {
 public:
  UriCountsStats(const std::string &uri, uint64_t counts) : uri_(uri), counts_(counts) {}

  friend bool operator<(const UriCountsStats &uri_counts1, const UriCountsStats &uri_counts2) {
    return uri_counts1.counts_ > uri_counts2.counts_;
  }

  std::string uri_;
  uint64_t counts_;
};

typedef std::priority_queue<UriCountsStats> UriCountsQueue;
typedef std::vector<UriCountsStats> UriCountsVec;

typedef std::unordered_map<std::string, uint64_t> UriCountsMap;

// --------------------------------------------------------------------------------------------------------------------
// Counters
// --------------------------------------------------------------------------------------------------------------------
class Counters {
 public:
  Counters() = default;
  virtual ~Counters() = default;

  // (re)set the value of a named counter
  virtual void Set(const std::string &key, uint64_t value) = 0;

  // remove the named counter
  virtual void Remove(const std::string &key) = 0;

  // retrieve the current value of the named counter, return false if not found
  virtual bool Get(const std::string &key, uint64_t *value) = 0;

  // increase the named counter by value.
  // if the counter does not exist,  treat it as if the counter was initialized to zero
  virtual void Add(const std::string &key, uint64_t value) = 0;
};

// --------------------------------------------------------------------------------------------------------------------
// BaseUriStats
// --------------------------------------------------------------------------------------------------------------------
class BaseUriStats {
 public:
  BaseUriStats() = default;
  virtual ~BaseUriStats() = default;

  virtual void Process() = 0;
  void PrintTopUri() {
  for (size_t i = 0; i < top_uri_.size(); ++i) {
    std::cout << top_uri_[i].counts_ << "\t"
              << top_uri_[i].uri_ << std::endl;
  }
}

 protected:
  static const int kTopKNumber = 100;
  UriCountsVec top_uri_;
};

#endif //TOP_K_URI_STATS_TYPES_H
