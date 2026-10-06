#pragma once
#include <string>

namespace game_library {
class Game;
class Review;
class User;
// Sequence-only controller. Its absence from the class diagram is reported.
class game_page {
public:
    Review submit_review(Game& game, const User& author, const std::string& content);
};
} // namespace game_library
