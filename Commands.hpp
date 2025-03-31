#pragma once
#include <vector>
#include <string>
#include "Table.hpp"
#include "Hotel.hpp"
#include "Room.hpp"

void insertMaster(Table<Hotel>& hotelTable, std::vector<std::string>& args);
void insertSlave(Table<Hotel>& hotelTable, Table<Room>& roomTable, std::vector<std::string>& args);
void getMaster(Table<Hotel>& hotelTable, std::vector<std::string>& args);
void getSlave(Table<Room>& roomTable, std::vector<std::string>& args);
void deleteMaster(Table<Hotel>& hotelTable, Table<Room>& roomTable, std::vector<std::string>& args);
void deleteSlave(Table<Hotel>& hotelTable, Table<Room>& roomTable, std::vector<std::string>& args);
void updateMaster(Table<Hotel>& hotelTable, std::vector<std::string>& args);
void updateSlave(Table<Room>& roomTable, std::vector<std::string>& args);
void calculateMaster(Table<Hotel>& hotelTable, std::vector<std::string>& args);
void calculateSlave(Table<Room>& roomTable, std::vector<std::string>& args);
void utilityMaster(Table<Hotel>& hotelTable, std::vector<std::string>& args);
void utilitySlave(Table<Room>& roomTable, std::vector<std::string>& args);
