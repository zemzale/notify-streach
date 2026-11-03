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

constexpr char TITLE[] = "Time to stretch!";
constexpr char MESSAGE[] = "Take a short break and stretch your body.";
constexpr char ICON[] = "appointment-soon";
constexpr char APP_NAME[] = "StretchReminder";

bool sendNotification() {
  NotifyNotification *notification =
      notify_notification_new(TITLE, MESSAGE, ICON);

  // Set notification properties
  notify_notification_set_timeout(notification, 10000); // 10 seconds
  notify_notification_set_urgency(notification, NOTIFY_URGENCY_CRITICAL);

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

  // Main loop
  while (running) {
    // Send notification
    sendNotification();

    // Calculate next reminder time (30 minutes from now)
    auto next_reminder =
        std::chrono::steady_clock::now() + std::chrono::minutes(30);

    // Sleep until next reminder or until interrupted
    while (running && std::chrono::steady_clock::now() < next_reminder) {
      std::this_thread::sleep_for(std::chrono::seconds(5));
    }
  }

  // Clean up
  notify_uninit();
  std::cout << "Stretch reminder stopped." << std::endl;

  return 0;
}
