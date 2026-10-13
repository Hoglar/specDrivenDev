// game_page is external; Game_Review denotes Review; see design.md.
#include "ogl/game.hpp"
#include "ogl/user.hpp"
#include <memory>

namespace {
void scenario(ogl::Game& game, const std::shared_ptr<ogl::User>& author)
{
    game.add_review(author, "Fun exploration", "I enjoyed the puzzles.");
}
}  // namespace

int main()
{
    auto developer = std::make_shared<ogl::User>("developer-1", "developer");
    auto author = std::make_shared<ogl::User>("author-1", "author");
    developer->handle(ogl::User::Event::email_confirmation);
    author->handle(ogl::User::Event::email_confirmation);
    auto game = developer->create_new_game("Open Adventure", "https://example.org/games/adventure.git");
    scenario(*game, author);
    return 0;
}
