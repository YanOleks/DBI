#pragma once
#include <cstring>
#include <string>
#include <iostream>
#include <vector>

struct Hotel {
    int id = 0;
    char name[15] = { 0 };
    char location[25] = { 0 };
    int firstRoomId = -1;
    bool isDeleted = false;

    Hotel() = default;

    Hotel(int id, const std::string& name, const std::string& location, int firstRoom = -1, bool deleted = false)
        : id(id), firstRoomId(firstRoom), isDeleted(deleted)
    {
        setName(name);
        setLocation(location);
    }

    void setName(const std::string& hotelName) {
        strncpy_s(name, hotelName.c_str(), sizeof(name) - 1);
        name[sizeof(name) - 1] = '\0';
    }

    void setLocation(const std::string& loc) {
        strncpy_s(location, loc.c_str(), sizeof(location) - 1);
        location[sizeof(location) - 1] = '\0';
    }

    void print() const {
        std::cout << "Hotel ID: " << id;
        std::cout << ", Name: " << name;
        std::cout << ", Location: " << location;
        std::cout << "\n";
    }

    static void printTable(const std::vector<Hotel>& hotels, bool ut = false) {
        if (ut) {
            std::cout << std::left << std::setw(6) << "ID"
                << std::setw(16) << "Name"
                << std::setw(26) << "Location"
                << std::setw(14) << "FirstRoomID"
                << "Deleted" << "\n";
            std::cout << std::string(80, '-') << "\n";
        }
        else {
            std::cout << std::left << std::setw(6) << "ID"
                << std::setw(16) << "Name"
                << std::setw(26) << "Location" << "\n";
            std::cout << std::string(48, '-') << "\n";
        }

        for (const auto& hotel : hotels) {
            if (ut) {
                std::cout << std::setw(6) << hotel.id
                    << std::setw(16) << hotel.name
                    << std::setw(26) << hotel.location
                    << std::setw(14) << hotel.firstRoomId
                    << (hotel.isDeleted ? "Yes" : "No") << "\n";
            }
            else {
                std::cout << std::setw(6) << hotel.id
                    << std::setw(16) << hotel.name
                    << std::setw(26) << hotel.location << "\n";
            }
        }
    }
};