#include "ogl/user.hpp"
#include "ogl/game.hpp"
#include <algorithm>
#include <stdexcept>
#include <utility>

namespace ogl {
User::User(uuid id, std::string username)
    : id_(std::move(id)), username_(std::move(username))
{
}

void User::add_game_to_library(std::shared_ptr<Game> game)
{
    if (!game) {
        throw std::invalid_argument("A library entry requires a game");
    }
    std::erase_if(game_library_, [](const auto& entry) { return entry.expired(); });
    for (const auto& entry : game_library_) {
        if (entry.lock() == game) {
            return;
        }
    }
    game_library_.push_back(game);
}

std::shared_ptr<Game> User::create_new_game(std::string name, std::string repo_url)
{
    auto game = std::make_shared<Game>(shared_from_this(), std::move(name), std::move(repo_url));
    game->handle(Game::Event::submit);
    game->handle(Game::Event::validate);
    return game;
}

bool User::no_active_suspension() const
{
    return user_status_ != User_Status::Suspended;
}

void User::handle(Event event)
{
    for (const Transition& transition : transitions) {
        if (transition.from != state_ || transition.event != event) {
            continue;
        }
        if (transition.guard != nullptr && !(this->*transition.guard)()) {
            continue;
        }
        if (transition.action != nullptr) {
            (this->*transition.action)();
        }
        state_ = transition.to;
        switch (state_) {
        case State::AwaitingEmailVerification:
            user_status_ = User_Status::Pending;
            break;
        case State::Active:
            user_status_ = User_Status::Active;
            marked_for_delete_ = false;
            break;
        case State::Suspended:
            user_status_ = User_Status::Suspended;
            break;
        case State::Closed:
            marked_for_delete_ = true;
            break;
        case State::Deleted:
            marked_for_delete_ = true;
            game_library_.clear();
            id_.clear();
            username_.clear();
            break;
        }
        return;
    }
}
}  // namespace ogl
