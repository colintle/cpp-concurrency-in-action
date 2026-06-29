#include <functional>
#include <future>
#include <string>

struct X {
  void foo(int, std::string const &) {}

  std::string bar(std::string const &) { return ""; }
};

struct Y {
  double operator()(double x) { return x; }
};

X baz(X &x) { return x; }

class move_only {
public:
  move_only() {}

  move_only(move_only &&) {}

  move_only(move_only const &) = delete;

  move_only &operator=(move_only &&) { return *this; }

  move_only &operator=(move_only const &) = delete;

  void operator()() {}
};

int main() {
  X x;

  auto f1 = std::async(&X::foo, &x, 42, "hello");
  auto f2 = std::async(&X::bar, x, "goodbye");

  Y y;
  auto f3 = std::async(Y(), 3.141);
  auto f4 = std::async(std::ref(y), 2.718);

  auto f5 = std::async(baz, std::ref(x));
  auto f6 = std::async(move_only());
  
  // Run in new thread
  auto f7 = std::async(std::launch::async, Y(), 1.2);
  // Run in wait() or get()
  auto f8 = std::async(std::launch::deferred, baz, std::ref(x));
  // Implementation chooses
  auto f9 = std::async(std::launch::deferred | std::launch::async, baz, std::ref(x));
  auto f10 = std::async(baz, std::ref(x));

  // Invoke deferred function
  f8.wait();
  return 0;
}
