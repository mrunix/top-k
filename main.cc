//
// Created by Michael on 2019/2/23.
//

#include <iostream>
#include <cstring>
#include <sys/time.h>

#include "uri_file_stats.h"
#include "uri_rocksdb_stats.h"

#define TIME_DIFF(a, b) (1.0*((b).tv_sec-(a).tv_sec)+0.000001*((b).tv_usec-(a).tv_usec))

int main(int argc, char **argv) {
  struct timeval start, file_stats_done, rocksdb_stats_done;

  gettimeofday(&start, NULL);
  UriFileStats file_stats("./data/uri.txt");
  file_stats.Process();
  gettimeofday(&file_stats_done, NULL);
  std::cout << "FILE STATS TIME: " << TIME_DIFF(start, file_stats_done) << std::endl;

  UriRocksDBStats db_stats("./data/uri.txt");
  db_stats.Process();
  gettimeofday(&rocksdb_stats_done, NULL);
  std::cout << "RocksDB STATS TIME: " << TIME_DIFF(file_stats_done, rocksdb_stats_done) << std::endl;
  return 0;
}