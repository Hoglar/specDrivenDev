#include "game_library/game_page.hpp"
#include "game_library/game.hpp"
#include "game_library/review.hpp"
namespace game_library {
Review game_page::send_inn_review(Game& game, const User& author, const std::string& content) {
    return game.add_review(author, content);
}
} // namespace game_library
