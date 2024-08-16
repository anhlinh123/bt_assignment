#include <algorithm>
#include <backend/backend.hpp>
#include <gmock/gmock.h>

using namespace testing;

class BackendTest : public testing::Test {
public:
  void SetUp() {
    std::ifstream istrm{"data.json", std::ios::binary};
    ASSERT_TRUE(istrm.is_open());
    Json::CharReaderBuilder rbuilder;
    rbuilder["collectComments"] = false;
    std::string errs;
    Json::parseFromStream(rbuilder, istrm, &root, &errs);
  }

protected:
  Json::Value root;
  static constexpr auto sortByName = [](auto &&lhs, auto &&rhs) {
    return lhs.name < rhs.name;
  };
};

TEST_F(BackendTest, contructorDoesntThrow) { EXPECT_NO_THROW(Backend{root}); }

TEST_F(BackendTest, getMoviesCorrectly) {
  Backend backend{root};
  auto movies = backend.getMovies();
  std::vector<std::string> expected = {"movie_1", "movie_2"};
  std::ranges::sort(movies, sortByName);
  EXPECT_EQ(movies.size(), expected.size());
  for (int i = 0; i < movies.size(); i++) {
    EXPECT_EQ(movies[i].name, expected[i]);
  }
}

TEST_F(BackendTest, getTheatersCorrectly) {
  Backend backend{root};
  auto theaters = backend.getTheaters("movie_1");
  std::vector<std::string> expected = {"theater_1", "theater_2"};
  std::ranges::sort(theaters, sortByName);
  EXPECT_EQ(theaters.size(), expected.size());
  for (int i = 0; i < theaters.size(); i++) {
    EXPECT_EQ(theaters[i].name, expected[i]);
  }
}

TEST_F(BackendTest, getTheatersReturnsEmpty) {
  Backend backend{root};
  auto theaters = backend.getTheaters("movie_3");
  EXPECT_EQ(theaters.size(), 0);
}

TEST_F(BackendTest, getAvailableSeatsCorrectly) {
  Backend backend{root};
  auto seats = backend.getAvailableSeats("theater_1", "movie_1");
  std::ranges::sort(seats, sortByName);
  std::vector<std::string> expected{"1", "2", "3", "4"};
  EXPECT_EQ(seats.size(), expected.size());
  for (int i = 0; i < seats.size(); i++) {
    EXPECT_EQ(seats[i].name, expected[i]);
  }
}

TEST_F(BackendTest, bookSuccessfully) {
  Backend backend{root};
  auto success =
      backend.book("theater_1", "movie_1", std::vector<std::string>{"1", "2"});
  EXPECT_EQ(success, true);
}

TEST_F(BackendTest, bookUnsuccessfully) {
  Backend backend{root};
  auto success =
      backend.book("theater_1", "movie_1", std::vector<std::string>{"1", "2"});
  EXPECT_EQ(success, true);
  success =
      backend.book("theater_1", "movie_1", std::vector<std::string>{"4", "2"});
  EXPECT_EQ(success, false);
}
