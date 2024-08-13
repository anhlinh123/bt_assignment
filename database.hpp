#include "types.hpp"

#include <memory>
#include <span>
#include <string>
#include <vector>

class Database {
public:
  Database(const std::string &root_dir);
  ~Database();

  Database(const Database &) = delete;
  Database(Database &&) noexcept;
  Database &operator=(const Database &) = delete;
  Database &operator=(Database &&) noexcept;

  std::vector<Movie> getMovies() const;
  std::vector<Theater> getTheaters(const std::string &movie_name) const;
  std::vector<Seat> getAvailableSeats(const std::string &theater_name,
                                      const std::string &movie_name) const;
  bool book(const std::string &theater_name, const std::string &movie_name,
            const std::vector<std::string> &seat_names);

private:
  class DatabaseImpl;
  std::unique_ptr<DatabaseImpl> impl;
};
