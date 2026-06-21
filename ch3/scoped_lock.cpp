#include <iostream>
#include <mutex>
#include <utility>

class X{
private:
  int some_data;
  std::mutex m;
public:
  X(int const& _some_data): some_data(_some_data){}
  friend void swap(X& lhs, X& rhs){
    if (&lhs == &rhs) return;
    std::lock(lhs.m, rhs.m);
    std::lock_guard<std::mutex> lock_a(lhs.m, std::adopt_lock);
    std::lock_guard<std::mutex> lock_b(rhs.m, std::adopt_lock);
    std::swap(lhs.some_data, rhs.some_data);
  }
  friend void swap_v2(X& lhs, X& rhs){
    std::scoped_lock guard(lhs.m, rhs.m);
    std::swap(lhs.some_data, rhs.some_data);
  }
};

int main(void){
  X x1(1);
  X x2(2);
  swap(x1, x2);
  swap_v2(x1, x2);
  return 0;
}
