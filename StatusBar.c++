#include <thread>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cstdio>
#include <condition_variable>
#include <sys/ioctl.h>
#include <unistd.h>
#include <chrono>
#include "StatusBar.h"

StatusBar::StatusBar(std::chrono::milliseconds interval) : interval_(interval) {}


void StatusBar::close()
{

    run_.notify_all();
    thread_.join();

}

void StatusBar::add(std::string name, ItemType type)
{

    StatusBar::Item item(type);
    items_.insert(std::pair<std::string, StatusBar::Item>(name, item));

}

void StatusBar::add(std::string name, ItemType type, std::string unit)
{

    StatusBar::Item item(type, unit);
    items_.insert(std::pair<std::string, StatusBar::Item>(name, item));

}

std::string& StatusBar::operator[](std::string itemName)
{

    return items_.at(itemName).value;

}

void StatusBar::launchThread()
{

    auto result = std::cv_status::timeout;

    while(result == std::cv_status::timeout)
    {
        refresh();

        std::unique_lock<std::mutex> lock(runMutex_);
        result = run_.wait_for(lock, interval_);
    }
    return;

}

void StatusBar::show()
{

    std::unique_lock<std::mutex> lock(runMutex_);
    thread_ = std::thread(&StatusBar::launchThread, this);

}

void StatusBar::refresh()
{

    std::cout.flush();
    std::cout << "\r";
    for(auto& item: items_)
    {
        std::cout << item.first << item.second.value << item.second.unit;
    }

}
