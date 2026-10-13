#include "ogl/game.hpp"
#include "ogl/repository.hpp"
#include "ogl/request.hpp"
#include "ogl/review.hpp"
#include "ogl/user.hpp"
#include <iostream>
#include <memory>

int main()
{
    auto developer = std::make_shared<ogl::User>("developer-1", "developer");
    auto player = std::make_shared<ogl::User>("player-1", "player");
    developer->handle(ogl::User::Event::email_confirmation);
    player->handle(ogl::User::Event::email_confirmation);

    auto game = developer->create_new_game("Open Adventure", "https://example.org/games/adventure.git");
    game->add_category(ogl::Category::Adventure);
    player->add_game_to_library(game);
    auto review = game->add_review(player, "Fun exploration", "I enjoyed the puzzles.");
    review->update_score(5);
    review->update("Fun exploration", "The puzzles and exploration work well together.");

    ogl::Repository repository("https://example.org/games/adventure.git");
    auto request = repository.add_feature_request(player, "More levels", "Add a second chapter.");
    request->upvote();
    request->downvote();
    request->update_status(ogl::Request_Status::Rejected);
    request->update("Second chapter", "Add a chapter using the existing puzzle mechanics.");
    request->update_status(ogl::Request_Status::Accepted);
    request->update_status(ogl::Request_Status::Completed);

    player->handle(ogl::User::Event::close_account);
    player->handle(ogl::User::Event::reopen_account);
    game->handle(ogl::Game::Event::archiveGame);

    std::cout << "Open Game Library demo: " << repository.show_requests().size()
              << " feature request stored.\n";
    return 0;
}
