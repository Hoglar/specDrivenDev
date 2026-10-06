#include "game_library/game.hpp"
#include "game_library/game_page.hpp"
#include "game_library/review.hpp"
#include "game_library/user.hpp"
#include <cassert>

void scenario(game_library::game_page& page, game_library::Game& game,
              const game_library::User& author) {
    const auto review = page.submit_review(game, author, "Scenario review");
    assert(review.content == "Scenario review");
}
int main() {
    using namespace game_library;
    User author(1, "player", "player@example.test");
    Game game(1, "Open Game", "0.1", Date{std::chrono::year{2026}/10/6});
    game_page page;
    scenario(page, game, author);
}
