#include <mutex>
#include <algorithm>
#include <list>
#include <thread>
#include <vector>
#include <stdexcept>

std::list<int> some_list;
std::mutex some_mutex;

class scoped_thread{
std::thread t;
public:
  explicit scoped_thread(std::thread t_): t(std::move(t_)){
    if (!t.joinable()) throw std::logic_error("No joinable thread");
  }
  ~scoped_thread(){
  if (t.joinable()) t.join();
  }
  scoped_thread(scoped_thread const&)=delete;
  scoped_thread& operator=(scoped_thread const&)=delete;
  scoped_thread(scoped_thread&& other) noexcept: t(std::move(other.t)){}
  scoped_thread& operator=(scoped_thread&& other) noexcept = delete;
};

void add_to_list(int new_value){
  // Locks the mutex on construction
  // Unlocks the mutex on destruction
  std::lock_guard<std::mutex> guard(some_mutex);
  some_list.push_back(new_value);
}

bool list_contains(int value_to_find){
  std::lock_guard<std::mutex> guard(some_mutex);
  return std::find(some_list.begin(), some_list.end(), value_to_find) != some_list.end(); 
}

int main(){
  std::vector<scoped_thread> threads;
  for (int i = 0; i < 5; i++){
    threads.emplace_back(std::thread(add_to_list, i));
    threads.emplace_back(std::thread(list_contains, i));
  }
  return 0;
}
