#include "game_library/game.hpp"
#include "game_library/review.hpp"
#include <utility>
namespace game_library {
Game::Game(int id, std::string title, std::string version, Date release_date,
           std::vector<std::weak_ptr<Review>> reviews)
    : game_id_(id), title_(std::move(title)), version_(std::move(version)),
      release_date_(release_date), reviews_(std::move(reviews)) {}
Review Game::add_review(const User& author, const std::string& content) {
    // Return by value as drawn. Never store a pointer to this temporary.
    return Review(author, content);
}
Game Game::download() { return *this; }
} // namespace game_library
