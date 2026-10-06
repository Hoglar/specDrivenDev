#pragma once
#include "game_library/user.hpp"

namespace game_library {
class Review;
class Game {
public:
    Game(int id, std::string title, std::string version, Date release_date,
         std::vector<std::weak_ptr<Review>> reviews = {});
    Review add_review(const User& author, const std::string& content);
    Game download();
private:
    int game_id_;
    std::string title_;
    std::string version_;
    Date release_date_;
    std::vector<std::weak_ptr<Review>> reviews_;
};
} // namespace game_library
