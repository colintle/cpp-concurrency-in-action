#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>

template <typename T> class threadsafe_queue {
private:
  mutable std::mutex mut;
  std::queue<T> data_queue;
  std::condition_variable data_cond;

public:
  threadsafe_queue() {};
  threadsafe_queue(const threadsafe_queue &other) {
    std::lock_guard<std::mutex> lk(other.mut);
    data_queue = other.data_queue;
  }
  threadsafe_queue &operator=(const threadsafe_queue &) = delete;
  void push(T new_value) {
    std::lock_guard<std::mutex> lk(mut);
    data_queue.push(new_value);
    data_cond.notify_one();
  };
  bool try_pop(T &new_value) {
    std::lock_guard<std::mutex> lk(mut);
    if (data_queue.empty()) return false;
    new_value=data_queue.front();
    data_queue.pop();
    return true;
  }
  std::shared_ptr<T> try_pop(){
    std::lock_guard<std::mutex> lk(mut);
    if(data_queue.empty()) return std::shared_ptr<T>();
    std::shared_ptr<T> res{std::make_shared<T>(data_queue.front())};
    data_queue.pop();
    return res;
  }
  void wait_and_pop(T &value) {
    std::unique_lock<std::mutex> lk(mut);
    data_cond.wait(lk, [this] { return !data_queue.empty(); });
    value = data_queue.front();
    data_queue.pop();
  };
  std::shared_ptr<T> wait_and_pop() {
    std::unique_lock<std::mutex> lk(mut);
    data_cond.wait(lk, [this] { return !data_queue.empty(); });
    std::shared_ptr<T> res{std::make_shared<T>(data_queue.front())};
    data_queue.pop();
    return res;
  }
  bool empty() const{
    std::lock_guard<std::mutex> lk(mut);
    return data_queue.empty();
  }
};

struct data_chunk {};

threadsafe_queue<data_chunk> data_queue;

void data_preparation_thread() {
  data_chunk const data = data_chunk();
  data_queue.push(data);
}

void data_processing_thread() {
  data_chunk data;
  data_queue.wait_and_pop(data);
}

int main() {
  std::thread t1(data_preparation_thread);
  std::thread t2(data_processing_thread);
  t1.join();
  t2.join();
  return 0;
}
