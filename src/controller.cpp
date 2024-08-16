#include <backend/controller.hpp>
#include <boost/algorithm/string.hpp>
#include <exception>
#include <iostream>

using namespace drogon;

Controller::Controller(std::shared_ptr<Backend> ptr)
    : backendPtr(std::move(ptr)) {
  if (!backendPtr)
    throw std::runtime_error{"Null backend"};
}

void Controller::getMovies(
    const HttpRequestPtr &req,
    std::function<void(const HttpResponsePtr &)> &&callback) {
  Json::Value json;
  auto movies = backendPtr->getMovies();
  for (auto &&movie : movies) {
    Json::Value val;
    val["name"] = movie.name;
    json.append(val);
  }
  auto resp = HttpResponse::newHttpJsonResponse(json);
  callback(resp);
}

void Controller::getTheaters(
    const HttpRequestPtr &req,
    std::function<void(const HttpResponsePtr &)> &&callback,
    const std::string &movie_name) {
  Json::Value json;
  auto theaters = backendPtr->getTheaters(movie_name);
  for (auto &&theater : theaters) {
    Json::Value val;
    val["name"] = theater.name;
    json.append(val);
  }
  auto resp = HttpResponse::newHttpJsonResponse(json);
  callback(resp);
}

void Controller::getAvailableSeats(
    const HttpRequestPtr &req,
    std::function<void(const HttpResponsePtr &)> &&callback,
    const std::string &theater_name, const std::string &movie_name) {
  Json::Value json;
  auto seats = backendPtr->getAvailableSeats(theater_name, movie_name);
  for (auto &&seat : seats) {
    Json::Value val;
    val["name"] = seat.name;
    json.append(val);
  }
  auto resp = HttpResponse::newHttpJsonResponse(json);
  callback(resp);
}

void Controller::book(const HttpRequestPtr &req,
                      std::function<void(const HttpResponsePtr &)> &&callback,
                      const std::string &theater_name,
                      const std::string &movie_name, const std::string &seats) {
  Json::Value json;
  std::vector<std::string> seat_names;
  boost::split(seat_names, seats, boost::is_any_of(","));
  auto result = backendPtr->book(theater_name, movie_name, seat_names);
  json["success"] = result;
  auto resp = HttpResponse::newHttpJsonResponse(json);
  callback(resp);
}
