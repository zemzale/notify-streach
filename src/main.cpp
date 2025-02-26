#include <atomic>
#include <chrono>
#include <csignal>
#include <iostream>
#include <libnotify/notify.h>
#include <thread>

std::atomic<bool> running{true};

void signalHandler(int signal) {
  running = false;
  std::cout << "Shutting down stretch reminder..." << std::endl;
}

bool sendNotification(const std::string &title, const std::string &message) {
  NotifyNotification *notification =
      notify_notification_new(title.c_str(), message.c_str(),
                              "appointment-soon" // Calendar icon
      );

  // Set notification properties
  notify_notification_set_timeout(notification, 10000); // 10 seconds
  notify_notification_set_urgency(notification, NOTIFY_URGENCY_NORMAL);

  // Show notification
  bool success = notify_notification_show(notification, nullptr);

  // Clean up
  g_object_unref(G_OBJECT(notification));

  return success;
}

int main() {
  // Initialize libnotify
  if (!notify_init("StretchReminder")) {
    std::cerr << "Failed to initialize libnotify" << std::endl;
    return 1;
  }

  // Set up signal handling for clean shutdown
  std::signal(SIGINT, signalHandler);
  std::signal(SIGTERM, signalHandler);

  std::cout << "Stretch reminder started. Press Ctrl+C to exit." << std::endl;

  // Main loop
  while (running) {
    // Send notification
    sendNotification("Time to Stretch!",
                     "Take a short break and stretch your body.");

    // Wait for 30 minutes (1800 seconds)
    for (int i = 0; i < 1800 && running; ++i) {
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }
  }

  // Clean up
  notify_uninit();
  std::cout << "Stretch reminder stopped." << std::endl;

  return 0;
}
