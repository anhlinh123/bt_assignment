#pragma once

#include "types.hpp"

#include <fstream>
#include <json/json.h>
#include <memory>
#include <span>
#include <string>
#include <vector>

/**
 * The core logic of the backend.
 * It stores all data internally and provide APIs to access it.
 * Using pimpl idiom wasn't a significant design consideration.
 * I just needed to develop backend without rebuilding other parts, hence the
 * idiom. Other than that, it makes no difference.
 *
 * The backend is thread-safe. All functions are synchronized so concurrent
 * bookings will be executed one by one.
 */
class Backend {
public:
  /**
   * Construct the backend from a json value.
   * The JSon value must have this format:
   * [
   *    {
   *        "name" : {theater name},
   *        "movies" : [
   *            {
   *                "name" : {movie name},
   *                "seats" : [ {list of seats} ]
   *            }
   *        ]
   *    }
   * ]
   */
  Backend(const Json::Value &root);
  ~Backend();

  Backend(const Backend &) = delete;
  Backend(Backend &&) noexcept;
  Backend &operator=(const Backend &) = delete;
  Backend &operator=(Backend &&) noexcept;

  std::vector<Movie> getMovies() const;
  std::vector<Theater> getTheaters(const std::string &movie_name) const;
  std::vector<Seat> getAvailableSeats(const std::string &theater_name,
                                      const std::string &movie_name) const;
  bool book(const std::string &theater_name, const std::string &movie_name,
            const std::vector<std::string> &seat_names);

private:
  class BackendImpl;
  std::unique_ptr<BackendImpl> impl;
};
