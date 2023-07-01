#include "multi_client.hh"

#include <unistd.h>  // getpid

#include <iostream>

#include "../client/requests.hh"
#include "../db/lock.hh"
#include "../gflags.hh"
#include "../global/actives.hh"
#include "../numa.hh"
#include "../tx/tx.hh"

void MultiClient::request(const int cid, Rendezvous &rendezvous,
                          AccessCounter &ac) {
  // tid_ = gettid();
  // Numa numa(tid_, cid % LOGICAL_CORE_SIZE);
  unsigned int cpu, node;
  if (getcpu(&cpu, &node)) exit(EXIT_FAILURE);
  Requests requests(cid);
  Measure *measure = new Measure(cid, node);
  Tx tx(cid, *measure);
  bool isLong = IS_LONG(cid);  // TODO
  [[maybe_unused]] volatile uint64_t value;
  rendezvous.notifyReady();
  rendezvous.waitStart();
  measure->start();
  while (rendezvous.isContinuable()) {
    requests.generate();
  RETRY:
    if (!rendezvous.isContinuable()) break;
    tx.begin();
    for (int i = 0; i < requests.size(); i++) {
      measure->check();
      switch (requests[i].getOperation()) {
        case Request::Operation::READ:
          value = tx.read(requests[i].getIndex());
#if SLEEP_US_LONG
          if (isLong) usleep(SLEEP_US_LONG);
#endif
          break;
        case Request::Operation::WRITE:
          if (!tx.write(requests[i].getIndex(), cid)) {
            tx.abort();
#if BACKOFF_US
            usleep(BACKOFF_US);
#endif
            goto RETRY;
          };
#if SLEEP_US_LONG
          if (isLong) usleep(SLEEP_US_LONG);
#endif
          break;
        default:
          std::cout << "operation not supported" << std::endl;
          exit(EXIT_FAILURE);
          break;
      }
    };
    tx.commit();
  };

  {
    Lock lock(ac);
    delete measure;
  }
}

MultiClient::MultiClient() : threads_(new std::thread *[FLAGS_thread_size]) {
  for (unsigned int cid = 0; cid < FLAGS_thread_size; cid++) {
    // Numa numa(gettid(), cid % LOGICAL_CORE_SIZE);
    threads_[cid] =
        new std::thread(request, cid, std::ref(rendezvous_), std::ref(ac_));
  }
  rendezvous_.rendezvous();
  rendezvous_.announceTheStart();
  sleep();
  rendezvous_.announceTheEnd();
}

MultiClient::~MultiClient() {
  join();
  for (int i = 0; i < FLAGS_thread_size; i++) {
    delete threads_[i];
  }
  delete[] threads_;
};

void MultiClient::join() {
  for (int i = 0; i < FLAGS_thread_size; i++) {
    threads_[i]->join();
  }
};

void MultiClient::sleep() {
  std::this_thread::sleep_for(std::chrono::seconds(EXEC_SEC));
};