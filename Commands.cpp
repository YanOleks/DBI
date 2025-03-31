#include "Commands.hpp"
#include "Hotel.hpp"
#include "Room.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>

void insertMaster(Table<Hotel>& hotelTable, std::vector<std::string>& args) {
    if (args.size() < 3) {
        std::cout << "Usage: insert-m <id> <name> <location>\n";
        return;
    }

    int hotelId = std::stoi(args[0]);

    if (hotelTable.exists(hotelId)) {
        throw std::invalid_argument("Hotel with this ID already exists.");
    }

    Hotel hotel{ hotelId, args[1], args[2], -1, false };
    hotelTable.insert(hotel);
    std::cout << "Hotel inserted successfully.\n";
}

void insertSlave(Table<Hotel>& hotelTable, Table<Room>& roomTable, std::vector<std::string>& args) {
    if (args.size() < 4) {
        std::cout << "Usage: insert-s <roomId> <hotelId> <number> <price>\n";
        return;
    }
    int roomId = std::stoi(args[0]);

    if (roomTable.exists(roomId)) {
        throw std::invalid_argument("Room with this ID already exists.");
    }

    int hotelId = std::stoi(args[1]);
    int price = std::stoi(args[3]);
    Hotel hotel = hotelTable.get(hotelId);
    Room room{ roomId, hotelId, args[2], price, hotel.firstRoomId, -1, false };
    if (hotel.firstRoomId != -1) {
        Room firstRoom = roomTable.get(hotel.firstRoomId);
        firstRoom.prevRoomId = roomId;
        roomTable.update(firstRoom.id, firstRoom);
    }
    hotel.firstRoomId = roomId;
    hotelTable.update(hotelId, hotel);
    roomTable.insert(room);
    std::cout << "Room inserted successfully.\n";
}

void getMaster(Table<Hotel>& hotelTable, std::vector<std::string>& args) {
    if (args.empty()) {
        auto hotels = hotelTable.getAll();
        if (!hotels.empty()) {
            Hotel::printTable(hotels);
        } 
        else {
            std::cout << "No records\n";
        }
    }
    else {
        auto const& hotelId = args[0];
        Hotel hotel = hotelTable.get(std::stoi(hotelId));
        hotel.print();
    }
}

void getSlave(Table<Room>& roomTable, std::vector<std::string>& args) {
    if (args.size() < 1) {
        auto rooms = roomTable.getAll();
        if (!rooms.empty()) {
            Room::printTable(rooms);
        }
        else {
            std::cout << "No records\n";
        }
    }
    else {
        auto const& roomId = args[0];
        Room room = roomTable.get(std::stoi(roomId));
        room.print();
        return;
    }
}

void deleteMaster(Table<Hotel>& hotelTable, Table<Room>& roomTable, std::vector<std::string>& args) {
    if (args.size() < 1) {
        std::cout << "Usage: del-m <id>\n";
        return;
    }
    int hotelId = std::stoi(args[0]);
    Hotel hotel = hotelTable.get(hotelId);
    int roomId = hotel.firstRoomId;
    while (roomId != -1) {
        Room room = roomTable.get(roomId);
        int nextRoomId = room.nextRoomId;
        roomTable.remove(roomId);
        roomId = nextRoomId;
    }
    hotelTable.remove(hotelId);
    std::cout << "Hotel " << hotelId << " and their rooms deleted successfully.\n";
}

void deleteSlave(Table<Hotel>& hotelTable, Table<Room>& roomTable, std::vector<std::string>& args) {
    if (args.size() < 1) {
        std::cout << "Usage: del-s <id>\n";
        return;
    }
    int roomId = std::stoi(args[0]);
    Room room = roomTable.get(roomId);
    if (room.prevRoomId != -1) {
        Room prevRoom = roomTable.get(room.prevRoomId);
        prevRoom.nextRoomId = room.nextRoomId;
        roomTable.update(prevRoom.id, prevRoom);
    }
    else {
        Hotel hotel = hotelTable.get(room.hotelId);
        hotel.firstRoomId = room.nextRoomId;
        hotelTable.update(hotel.id, hotel);
    }
    if (room.nextRoomId != -1) {
        Room nextRoom = roomTable.get(room.nextRoomId);
        nextRoom.prevRoomId = room.prevRoomId;
        roomTable.update(nextRoom.id, nextRoom);
    }
    roomTable.remove(roomId);
    std::cout << "Room " << roomId << " deleted successfully and links updated.\n";
}

void updateMaster(Table<Hotel>& hotelTable, std::vector<std::string>& args) {
    if (args.size() < 3) {
        std::cout << "Usage: update-m <id> <name> <location>\n";
        return;
    }
    int hotelId = std::stoi(args[0]);
    Hotel hotel = hotelTable.get(hotelId);
    hotel.setName(args[1]);
    hotel.setLocation(args[2]);
    hotelTable.update(hotelId, hotel);
    std::cout << "Hotel " << hotelId << " updated successfully.\n";
}

void updateSlave(Table<Room>& roomTable, std::vector<std::string>& args) {
    if (args.size() < 3) {
        std::cout << "Usage: update-s <id> <number> <price>\n";
        return;
    }
    int roomId = std::stoi(args[0]);
    Room room = roomTable.get(roomId);
    room.setNumber(args[1]);
    room.price = std::stoi(args[2]);
    roomTable.update(roomId, room);
    std::cout << "Room " << roomId << " updated.\n";
}

void calculateMaster(Table<Hotel>& hotelTable, std::vector<std::string>& args) {
    auto hotels = hotelTable.getAll();
    std::cout << "Total hotels: " << hotels.size() << "\n";
}

void calculateSlave(Table<Room>& roomTable, std::vector<std::string>& args) {
    auto rooms = roomTable.getAll();
    std::cout << "Total rooms: " << rooms.size() << "\n";
}

void utilityMaster(Table<Hotel>& hotelTable, std::vector<std::string>& args) {
    Hotel::printTable(hotelTable.getAllRecords(), true);
}

void utilitySlave(Table<Room>& roomTable, std::vector<std::string>& args) {
    Room::printTable(roomTable.getAllRecords(), true);
}
