#include <deque>
#include <future>
#include <iostream>
#include <mutex>
#include <thread>

std::mutex mutex;
std::deque<std::packaged_task<void()>> gui_task_queue;

bool gui_shutdown_requested() {
  static int loops = 0;
  return loops++ > 50;
}
void process_ui_events() {
  std::this_thread::sleep_for(std::chrono::milliseconds(50));
}

void gui_thread_loop() {
  while (!gui_shutdown_requested()) {
    process_ui_events();
    std::packaged_task<void()> current_task;
    {
      std::lock_guard<std::mutex> lk(mutex);
      if (!gui_task_queue.empty()) {
        current_task = std::move(gui_task_queue.front());
        gui_task_queue.pop_front();
      }
    }

    if (current_task.valid()) {
      current_task();
    }
  }
}

template <typename Func> std::future<void> post_task_to_gui_thread(Func f) {
  std::packaged_task<void()> task(f);
  std::future<void> res = task.get_future();
  {
    std::lock_guard<std::mutex> lk(mutex);
    gui_task_queue.push_back(std::move(task));
  }

  return res;
}

void background_worker() {
  std::this_thread::sleep_for(std::chrono::seconds(1));

  std::cout << "[Worker] Heavy logic complete. Requesting UI update... \n";

  std::future<void> ui_updated = post_task_to_gui_thread([]() {
    std::cout << "[GUI Thread] Updating text labels and button colors here "
                 "safely.\n";
  });

  // Block until the GUI thread actually pops and runs our lambda
  ui_updated.get();
  std::cout << "[Worker] GUI has finished updating. Resuming background "
               "operations.\n";
}

int main(){
  std::thread background(background_worker);
  gui_thread_loop();
  background.join();
  return 0;
}
