#ifndef STATUS_BAR_HH
#define STATUS_BAR_HH

#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <map>
#include <chrono>

// Implements a simple status bar for a program running in the terminal
// TODO: add mutex for map access
// TODO: add progress bar and toggle support
class StatusBar
{

    public:

        enum ItemType { VALUE, PROGRESS_BAR, TOGGLE };

        // StatusBar with refresh interval "interval" in milliseconds
        StatusBar(  std::chrono::milliseconds interval = 
                    (std::chrono::duration<unsigned int, std::milli>(100)));

        // Shows or "starts" the StatusBar after is has been
        // constructed and configured
        void show();

        // Close the StatusBar
        void close();

        // Add an empty item of "type" to the StatusBar
        void add(std::string name, ItemType type);

        // Add an item with a unit specifier (for example "kB/s")
        void add(std::string name, ItemType type, std::string unit);

        // Method for accessing the value of itemName in the StatusBar 
        // with their name
        std::string& operator[](std::string itemName);

    private:

        void launchThread();

        void refresh();

        // Class for storing items
        class Item
        {

            public:

                Item(   ItemType type, std::string unit = "", 
                        std::string value = "") :   type(type), unit(unit),
                                                    value(value) {}

                ItemType type;
                std::string unit;
                std::string value;

        };

        // Map where all the values are stored
        std::map<std::string, Item> items_;

        // Refresh interval in milliseconds
        std::chrono::milliseconds interval_;

        // Condition variable that tells the thread if it should die
        std::condition_variable run_;
        std::mutex runMutex_;

        // Thread that updates the bar
        std::thread thread_;

};
#endif
