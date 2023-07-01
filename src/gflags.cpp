#include <gflags/gflags.h>

DEFINE_uint64(thread_size, 0, "0 <=");
DEFINE_uint64(filename, 0, "");
DEFINE_uint64(record_size, 1000, "0 <=");
DEFINE_double(skew, 0.0, "0 ~ 0.999...");
DEFINE_uint64(request_size, 10, "0 <=");
DEFINE_uint64(request_size_long, 0, "0 <=");
DEFINE_uint64(read_ratio, 50, "0 <=");
DEFINE_uint64(read_ratio_long, 0, "0 <=");
DEFINE_uint64(actives_update_interval_ms, 0, "0 <=");
DEFINE_uint64(tpr_limit, 50, "0 <=");
