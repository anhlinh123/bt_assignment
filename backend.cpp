#include "database.hpp"

#include <boost/algorithm/string.hpp>
#include <drogon/drogon.h>
using namespace drogon;

namespace {
Database database{"."};
}

int main() {
  app().registerHandler(
      "/movies",
      [](const HttpRequestPtr &req,
         std::function<void(const HttpResponsePtr &)> &&callback) {
        Json::Value json;
        auto movies = database.getMovies();
        for (auto &&movie : movies) {
          Json::Value val;
          val["name"] = movie.name;
          json.append(val);
        }
        auto resp = HttpResponse::newHttpJsonResponse(json);
        callback(resp);
      },
      {Get, "LoginFilter"});
  app().registerHandler(
      "/theaters?movie={movie}",
      [](const HttpRequestPtr &req,
         std::function<void(const HttpResponsePtr &)> &&callback,
         const std::string &movie) {
        Json::Value json;
        auto theaters = database.getTheaters(movie);
        for (auto &&theater : theaters) {
          Json::Value val;
          val["name"] = theater.name;
          json.append(val);
        }
        auto resp = HttpResponse::newHttpJsonResponse(json);
        callback(resp);
      },
      {Get, "LoginFilter"});
  app().registerHandler(
      "/seats?theater={theater}&movie={movie}",
      [](const HttpRequestPtr &req,
         std::function<void(const HttpResponsePtr &)> &&callback,
         const std::string &theater, const std::string &movie) {
        Json::Value json;
        auto seats = database.getAvailableSeats(theater, movie);
        for (auto &&seat : seats) {
          Json::Value val;
          val["name"] = seat.name;
          json.append(val);
        }
        auto resp = HttpResponse::newHttpJsonResponse(json);
        callback(resp);
      },
      {Get, "LoginFilter"});
  app().registerHandler(
      "/book?theater={theater}&movie={movie}&seats={seats}",
      [](const HttpRequestPtr &req,
         std::function<void(const HttpResponsePtr &)> &&callback,
         const std::string &theater, const std::string &movie,
         const std::string &seats) {
        Json::Value json;
        std::vector<std::string> seat_names;
        boost::split(seat_names, seats, boost::is_any_of(","));
        auto result = database.book(theater, movie, seat_names);
        json["success"] = result;
        auto resp = HttpResponse::newHttpJsonResponse(json);
        callback(resp);
      },
      {Get, "LoginFilter"});
  app()
      .setLogPath("./")
      .setLogLevel(trantor::Logger::kWarn)
      .addListener("0.0.0.0", 8080)
      .setThreadNum(16)
      .run();
}
