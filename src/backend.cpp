#include <backend/backend.hpp>
#include <mutex>
#include <shared_mutex>

class Backend::BackendImpl {
public:
  BackendImpl(const Json::Value &root) {
    for (auto &&theater_val : root) {
      auto &theater = theaters[theater_val["name"].asString()];
      theater.name = theater_val["name"].asString();
      for (auto &&movie_val : theater_val["movies"]) {
        auto &&cinema = theater.movies[movie_val["name"].asString()];
        for (auto &&seat : movie_val["seats"]) {
          cinema.seats[seat.asString()].name = seat.asString();
        }

        auto &&movie = movies[movie_val["name"].asString()];
        movie.name = movie_val["name"].asString();
        movie.theaters.push_back(theater.name);
      }
    }
  }

  std::vector<Movie> getMovies() const {
    std::shared_lock lk{mutex};
    std::vector<Movie> ret;
    for (auto &&[name, movie] : movies) {
      ret.push_back(movie);
    }
    return ret;
  }

  std::vector<Theater> getTheaters(const std::string &movie_name) const {
    std::shared_lock lk{mutex};
    std::vector<Theater> ret;
    if (auto movie_it = movies.find(movie_name); movie_it != movies.end()) {
      for (auto &&theater_names : movie_it->second.theaters) {
        if (auto theater_it = theaters.find(theater_names);
            theater_it != theaters.end()) {
          ret.push_back(theater_it->second);
        }
      }
    }
    return ret;
  }

  std::vector<Seat> getAvailableSeats(const std::string &theater_name,
                                      const std::string &movie_name) const {
    std::shared_lock lk{mutex};
    std::vector<Seat> ret;
    if (!theaters.contains(theater_name))
      return ret;
    auto &&theater = theaters.find(theater_name)->second;
    if (!theater.movies.contains(movie_name))
      return ret;

    for (auto &&[seat_name, seat] :
         theater.movies.find(movie_name)->second.seats) {
      if (!seat.is_booked)
        ret.push_back(seat);
    }

    return ret;
  }

  bool book(const std::string &theater_name, const std::string &movie_name,
            const std::vector<std::string> &seat_names) {
    std::scoped_lock lk{mutex};

    if (!theaters.contains(theater_name))
      return false;
    auto &&theater = theaters[theater_name];
    if (!theater.movies.contains(movie_name))
      return false;

    auto &&seats = theater.movies[movie_name].seats;
    for (auto &&seat_name : seat_names) {
      if (!seats.contains(seat_name) || seats[seat_name].is_booked)
        return false;
    }

    for (auto &&seat_name : seat_names) {
      seats[seat_name].is_booked = true;
    }
    return true;
  }

private:
  std::unordered_map<std::string, Movie> movies;
  std::unordered_map<std::string, Theater> theaters;
  mutable std::shared_mutex mutex;
};

Backend::Backend(const Json::Value &root) : impl(new BackendImpl(root)) {}
Backend::~Backend() = default;
Backend::Backend(Backend &&) noexcept = default;
Backend &Backend::operator=(Backend &&) noexcept = default;

std::vector<Movie> Backend::getMovies() const { return impl->getMovies(); }

std::vector<Theater> Backend::getTheaters(const std::string &movie_name) const {
  return impl->getTheaters(movie_name);
}

std::vector<Seat>
Backend::getAvailableSeats(const std::string &theater_name,
                           const std::string &movie_name) const {
  return impl->getAvailableSeats(theater_name, movie_name);
}

bool Backend::book(const std::string &theater_name,
                   const std::string &movie_name,
                   const std::vector<std::string> &seat_names) {
  return impl->book(theater_name, movie_name, seat_names);
}
