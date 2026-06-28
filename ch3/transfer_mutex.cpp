#include <mutex>

std::mutex some_mutex;

std::unique_lock<std::mutex> get_lock() {
  std::unique_lock<std::mutex> lk(some_mutex);
  return lk;
}

void process_data() {
  std::unique_lock<std::mutex> lk(get_lock());
}

int main() {
  process_data();
  return 0;
}
