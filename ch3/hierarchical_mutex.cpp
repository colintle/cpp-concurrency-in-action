#include <iostream>
#include <climits>
#include <stdexcept>
#include <mutex>

class hierarchical_mutex {
  void check_for_hierarchy_violation() {
    if (this_thread_hierarchy_value <= hierarchy_value) {
      throw std::logic_error("mutex hierarchy violated");
    }
  }
  void update_hierarchy_value(){
    previous_hierarchy_value = this_thread_hierarchy_value;
    this_thread_hierarchy_value = hierarchy_value;
  }

public:
  std::mutex internal_mutex;
  unsigned long const hierarchy_value;
  unsigned long previous_hierarchy_value;
  // each thread stores its own hierarchy value
  static thread_local unsigned long this_thread_hierarchy_value;

  explicit hierarchical_mutex(unsigned long value): previous_hierarchy_value(0), hierarchy_value(value){}
  void lock(){
    check_for_hierarchy_violation();
    internal_mutex.lock();
    update_hierarchy_value();
  }
  void unlock(){
    if (this_thread_hierarchy_value != hierarchy_value){
      throw std::logic_error("mutex hierarchy violated");
    }
    this_thread_hierarchy_value = previous_hierarchy_value;
    internal_mutex.unlock();
  }
  bool try_lock()
{
    check_for_hierarchy_violation();
    if (!internal_mutex.try_lock()) return false;
    update_hierarchy_value();
    return true;
  }
};
thread_local unsigned long hierarchical_mutex::this_thread_hierarchy_value(ULONG_MAX);

int main(){
  hierarchical_mutex m1(10);
  hierarchical_mutex m2(2);

  m1.lock();
  m2.lock();

  m2.unlock();
  std::cout << m2.previous_hierarchy_value << std::endl;
  m1.unlock();
  std::cout << m1.previous_hierarchy_value << std::endl;
  return 0;
}
