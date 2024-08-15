#include "types.hpp"

#include <memory>
#include <span>
#include <string>
#include <vector>

class Backend {
public:
  Backend(const std::string &root_dir);
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
