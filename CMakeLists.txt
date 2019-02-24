cmake_minimum_required(VERSION 3.10)
project(top-k)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -faligned-new -DHAVE_ALIGNED_NEW -DROCKSDB_PLATFORM_POSIX -DROCKSDB_LIB_IO_POSIX  -DOS_LINUX -fno-builtin-memcmp -DROCKSDB_FALLOCATE_PRESENT -DSNAPPY -DGFLAGS=1 -DZLIB -DBZIP2 -DLZ4 -DZSTD -DROCKSDB_MALLOC_USABLE_SIZE -DROCKSDB_PTHREAD_ADAPTIVE_MUTEX -DROCKSDB_BACKTRACE -DROCKSDB_RANGESYNC_PRESENT -DROCKSDB_SCHED_GETCPU_PRESENT -march=native  -DHAVE_SSE42 -DHAVE_PCLMUL -DROCKSDB_SUPPORT_THREAD_LOCAL")

include_directories(./rocksdb/include ./)
link_directories(./rocksdb/lib)

add_executable(top-k
        main.cc
        murmur_hash.cc
        murmur_hash.h
        uri_stats_types.h
        uri_file_stats.cc
        uri_file_stats.h
        uri_file_utils.cc
        uri_file_utils.h
		uri_rocksdb_stats.cc
		uri_rocksdb_stats.h
)

target_link_libraries(top-k rocksdb rt snappy gflags z bz2 zstd lz4)