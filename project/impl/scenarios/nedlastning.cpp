// UI search and download are external to the domain classes; see design.md.
#include "ogl/game.hpp"
#include "ogl/user.hpp"
#include <memory>

namespace {
void scenario(ogl::User& user, const std::shared_ptr<ogl::Game>& selected_game)
{
    user.add_game_to_library(selected_game);
}
}  // namespace

int main()
{
    auto developer = std::make_shared<ogl::User>("developer-1", "developer");
    auto player = std::make_shared<ogl::User>("player-1", "player");
    developer->handle(ogl::User::Event::email_confirmation);
    player->handle(ogl::User::Event::email_confirmation);
    auto game = developer->create_new_game("Open Adventure", "https://example.org/games/adventure.git");
    scenario(*player, game);
    return 0;
}
