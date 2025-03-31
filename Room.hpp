#pragma once
#include <cstring>
#include <string>
#include <iostream> 

struct Room {
    int id = 0;
    int hotelId = 0;
    char number[15] = { 0 };
    int price = 0;
    int nextRoomId = -1;
    int prevRoomId = -1;
    bool isDeleted = false;

    Room() = default;

    Room(int roomId, int hotel, const std::string& number, int roomPrice, int next = -1, int prev = -1, bool deleted = false)
        : id(roomId), hotelId(hotel), price(roomPrice), nextRoomId(next), prevRoomId(prev), isDeleted(deleted)
    {
        setNumber(number);
    }

    void setNumber(const std::string& num) {
        strncpy_s(number, num.c_str(), sizeof(number) - 1);
        number[sizeof(number) - 1] = '\0';
    }

    void print() const {
        std::cout << "Room ID: " << id;
        std::cout << ", Hotel ID: " << hotelId;
        std::cout << ", Number: " << number;
        std::cout << ", Price: " << price;
        std::cout << "\n";
    }

    static void printTable(const std::vector<Room>& rooms, bool ut = false) {
        if (ut) {
            std::cout << std::left << std::setw(6) << "ID"
                << std::setw(10) << "HotelID"
                << std::setw(16) << "Number"
                << std::setw(8) << "Price"
                << std::setw(12) << "NextRoomID"
                << std::setw(12) << "PrevRoomID"
                << "Deleted" << "\n";
            std::cout << std::string(74, '-') << "\n";
        }
        else {
            std::cout << std::left << std::setw(6) << "ID"
                << std::setw(10) << "HotelID"
                << std::setw(16) << "Number"
                << std::setw(8) << "Price" << "\n";
            std::cout << std::string(40, '-') << "\n";
        }

        for (const auto& room : rooms) {
            if (ut) {
                std::cout << std::setw(6) << room.id
                    << std::setw(10) << room.hotelId
                    << std::setw(16) << room.number
                    << std::setw(8) << room.price
                    << std::setw(12) << room.nextRoomId
                    << std::setw(12) << room.prevRoomId
                    << (room.isDeleted ? "Yes" : "No") << "\n";
            }
            else {
                std::cout << std::setw(6) << room.id
                    << std::setw(10) << room.hotelId
                    << std::setw(16) << room.number
                    << std::setw(8) << room.price << "\n";
            }
        }
    }
};
