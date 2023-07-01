cmake_minimum_required(VERSION 3.10)

project(si)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/../cmake")

enable_language(CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "-O2 -Wall -g")

set(SOURCES
gflags.cpp

si.cpp

# global
global/global.cpp

# client
client/multi_client.cpp
client/rendezvous.cpp
client/requests.cpp
client/request.cpp

# tx
tx/tx.cpp

# db
db/database.cpp
db/records.cpp
db/record.cpp
db/version.cpp
db/lock.cpp
db/access_counter.cpp

# gc
gc/epo.cpp
gc/epo_r.cpp
gc/oat_bg.cpp
gc/oat.cpp
gc/local_actives.cpp
gc/time_stamp_counter.cpp
gc/active.cpp

# operation
operations.cpp
operation.cpp

# timestamp
timestamp.cpp

# measure
measure/measure.cpp

# io
io/file.cpp

# perf
perf/perf.cpp
perf/perf_read.cpp

numa.cpp
)


add_executable(si.exe ${SOURCES})

include_directories($ENV{HOME}/gflags-master)

target_link_libraries(
si.exe
pthread
gflags
$ENV{HOME}/mimalloc-master/out/release/libmimalloc.a
)


# ------------ Experiment Parameters ------------
# # 5*10^{-3}秒ごとにスキャン
# # マイクロ秒(US)(10^{-6}秒)ごとにスキャン

if (DEFINED SLEEP_US_LONG)
    add_definitions(-DSLEEP_US_LONG=${SLEEP_US_LONG})
else ()
    add_definitions(-DSLEEP_US_LONG=0)
endif ()


if (DEFINED EXEC_SEC)
    add_definitions(-DEXEC_SEC=${EXEC_SEC})
else ()
    add_definitions(-DEXEC_SEC=5)
endif ()

if (DEFINED BACKOFF_US)
    add_definitions(-DBACKOFF_US=${BACKOFF_US})
else ()
    add_definitions(-DBACKOFF_US=0)
endif ()

# NO-GC: 0, EPO: 1, EPO-R: 2
if (DEFINED GC_MODE)
    add_definitions(-DGC_MODE=${GC_MODE})
else ()
    add_definitions(-DGC_MODE=0)
endif ()

# -------------------------------------
if (DEFINED OPT)
    add_definitions(-DOPT=${OPT})
else ()
    add_definitions(-DOPT=0)
endif ()

if (DEFINED OPT_INTERVAL_MS)
    add_definitions(-DOPT_INTERVAL_MS=${OPT_INTERVAL_MS})
else ()
    add_definitions(-DOPT_INTERVAL_MS=100)
endif ()

if (DEFINED OPT_DELETION_THRESHOLD)
    add_definitions(-DOPT_DELETION_THRESHOLD=${OPT_DELETION_THRESHOLD})
else ()
    add_definitions(-DOPT_DELETION_THRESHOLD=20)
endif ()

if (DEFINED OPT_MAX_COUNT)
    add_definitions(-DOPT_MAX_COUNT=${OPT_MAX_COUNT})
else ()
    add_definitions(-DOPT_MAX_COUNT=3)
endif ()

# -------------------------------------
if (DEFINED LOGICAL_CORE_SIZE)
    add_definitions(-DLOGICAL_CORE_SIZE=${LOGICAL_CORE_SIZE})
else ()
    add_definitions(-DLOGICAL_CORE_SIZE=224)
endif ()
# -------------------------------------
