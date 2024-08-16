#pragma once

#include <string>
#include <unordered_map>
#include <vector>

struct Movie {
  std::string name;
  std::vector<std::string> theaters;
};

struct Seat {
  std::string name;
  bool is_booked{};
};

struct Cinema {
  std::unordered_map<std::string, Seat> seats;
};

struct Theater {
  std::string name;
  std::unordered_map<std::string, Cinema> movies;
};
