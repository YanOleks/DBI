#include <iostream>
#include <unordered_map>
#include <functional>
#include <sstream>
#include <vector>
#include <string>
#include <filesystem>

#include "Commands.hpp"
#include "Table.hpp"
#include "Hotel.hpp"
#include "Room.hpp"

const std::string helpMessage = R"(Available commands:
    insert-m - Insert a master record (hotel). Usage: insert-m <HotelID> <Name> <Location>
    insert-s - Insert a slave record (room). Usage: insert-s <RoomID> <HotelID> <Number> <Price>
    get-m    - Retrieve a master record by HotelID. Usage: get-m <HotelID>
    get-s    - Retrieve a slave record by RoomID. Usage: get-s <RoomID>
    del-m    - Delete a master record (hotel) and its associated slave records (rooms). Usage: del-m <HotelID>
    del-s    - Delete a slave record (room). Usage: del-s <RoomID>
    update-m - Update a master record's (hotel) field. Usage: update-m <HotelID> <Name> <Location>
    update-s - Update a slave record's (room) field. Usage: update-s <RoomID> <Number> <Price>
    calc-m   - Calculate total number of master records (hotels).
    calc-s   - Calculate total number of slave records (rooms).
    ut-m     - Display all master records (hotels) including metadata.
    ut-s     - Display all slave records (rooms) including metadata.)";

int main(int argc, char* argv[]) {
    Table<Hotel> hotelTable("data/hotels");
    Table<Room> roomTable("data/rooms");

    std::unordered_map<
        std::string_view,
        std::function<void(std::vector<std::string>&)>,
        std::hash<std::string_view>,
        std::equal_to<>
    > commandMap = {
        {"insert-m", [&hotelTable](std::vector<std::string>& args) { insertMaster(hotelTable, args); }},
        {"insert-s", [&hotelTable, &roomTable](std::vector<std::string>& args) { insertSlave(hotelTable, roomTable, args); }},
        {"get-m", [&hotelTable](std::vector<std::string>& args) { getMaster(hotelTable, args); }},
        {"get-s", [&roomTable](std::vector<std::string>& args) { getSlave(roomTable, args); }},
        {"del-m", [&hotelTable, &roomTable](std::vector<std::string>& args) { deleteMaster(hotelTable,roomTable, args); }},
        {"del-s", [&hotelTable, &roomTable](std::vector<std::string>& args) { deleteSlave(hotelTable,roomTable, args); }},
        {"update-m", [&hotelTable](std::vector<std::string>& args) { updateMaster(hotelTable, args); }},
        {"update-s", [&roomTable](std::vector<std::string>& args) { updateSlave(roomTable, args); }},
        {"calc-m", [&hotelTable](std::vector<std::string>& args) { calculateMaster(hotelTable, args); }},
        {"calc-s", [&roomTable](std::vector<std::string>& args) { calculateSlave(roomTable, args); }},
        {"ut-m", [&hotelTable](std::vector<std::string>& args) { utilityMaster(hotelTable, args); }},
        {"ut-s", [&roomTable](std::vector<std::string>& args) { utilitySlave(roomTable, args); }}
    };

    std::string input;
    while (true) {
        std::cout << ">";
        std::getline(std::cin, input);

        if (input == "exit") break;

        if (input.empty()) continue;

        if (input == "help") {
            std::cout << helpMessage << std::endl;
            continue;
        }

        std::istringstream iss(input);
        std::string cmd;
        iss >> cmd;

        std::vector<std::string> args;
        std::string arg;
        while (iss >> arg) {
            args.push_back(arg);
        }

        auto it = commandMap.find(std::string_view(cmd));
        if (it != commandMap.end()) {
            try { it->second(args); }
            catch (const std::exception& e) {
                std::cerr << "Error: " << e.what() << "\n";
            }
        }
        else {
            std::cout << "Unknown command: " << cmd << " Type 'help' for available commands.\n";
        }
    }

    return 0;
}
