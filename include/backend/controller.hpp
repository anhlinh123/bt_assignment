#pragma once

#include <backend/backend.hpp>
#include <drogon/drogon.h>
#include <functional>
#include <memory>
#include <string>

/**
 * The controller to handle http requests. This is the glue that connects the
 * backend with the drogon framework.
 *
 */
class Controller : public drogon::HttpController<Controller, false> {
public:
  Controller(std::shared_ptr<Backend> ptr);

  METHOD_LIST_BEGIN
  ADD_METHOD_TO(Controller::getMovies, "/movies", drogon::Get);
  ADD_METHOD_TO(Controller::getTheaters, "/theaters?movie={1}", drogon::Get);
  ADD_METHOD_TO(Controller::getAvailableSeats, "/seats?theater={1}&movie={2}",
                drogon::Get);
  ADD_METHOD_TO(Controller::book, "/book?theater={1}&movie={2}&seats={3}",
                drogon::Post);
  METHOD_LIST_END

  void
  getMovies(const drogon::HttpRequestPtr &req,
            std::function<void(const drogon::HttpResponsePtr &)> &&callback);
  void
  getTheaters(const drogon::HttpRequestPtr &req,
              std::function<void(const drogon::HttpResponsePtr &)> &&callback,
              const std::string &movie_name);
  void getAvailableSeats(
      const drogon::HttpRequestPtr &req,
      std::function<void(const drogon::HttpResponsePtr &)> &&callback,
      const std::string &theater_name, const std::string &movie_name);
  void book(const drogon::HttpRequestPtr &req,
            std::function<void(const drogon::HttpResponsePtr &)> &&callback,
            const std::string &theater_name, const std::string &movie_name,
            const std::string &seats);

private:
  std::shared_ptr<Backend> backendPtr;
};
