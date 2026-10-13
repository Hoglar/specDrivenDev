// Developer Portal is represented by the external actor; see design.md.
#include "ogl/game.hpp"
#include "ogl/user.hpp"
#include <memory>

namespace {
std::shared_ptr<ogl::Game> scenario(ogl::User& developer)
{
    return developer.create_new_game("Open Adventure", "https://example.org/games/adventure.git");
}
}  // namespace

int main()
{
    auto developer = std::make_shared<ogl::User>("developer-1", "developer");
    developer->handle(ogl::User::Event::email_confirmation);
    const auto published_game = scenario(*developer);
    return 0;
}
