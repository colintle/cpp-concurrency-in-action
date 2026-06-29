#include <iostream>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <queue>

struct data_chunk {};
std::mutex mut;
std::queue<data_chunk> data_queue;
std::condition_variable data_cond;

data_chunk prepare_data(){
  return data_chunk();
}

void process_data(){
}

void data_preparation_thread(){
  data_chunk const data = prepare_data();
  std::lock_guard<std::mutex> lk(mut);
  data_queue.push(data);
  std::cout << "Pushed data in queue" << std::endl;
  data_cond.notify_one();
}

void data_processing_thread(){
    std::unique_lock<std::mutex> lk(mut);
    data_cond.wait(lk, []{
      return !data_queue.empty();
    });
    std::cout << "Woke up from condition variable" << std::endl;
    data_chunk data = data_queue.front();
    data_queue.pop();
    process_data();
    lk.unlock();
}

int main(){
  std::thread t1(data_preparation_thread);
  std::thread t2(data_processing_thread);

  t1.join();
  t2.join();
  return 0;
}
