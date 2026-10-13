#include "ogl/game.hpp"
#include "ogl/review.hpp"
#include <algorithm>
#include <cstddef>
#include <stdexcept>
#include <utility>

namespace ogl {
Game::Game(std::shared_ptr<User> creator, std::string name, std::string repo_url)
    : name_(std::move(name)), creator_(std::move(creator)),
      game_repository_(std::move(repo_url))
{
    if (!creator_) {
        throw std::invalid_argument("A game requires a creator");
    }
    static std::size_t next_id = 0;
    id_ = "game-" + std::to_string(++next_id);
}

std::shared_ptr<Review> Game::add_review(std::shared_ptr<User> user,
                                       std::string title, std::string message)
{
    auto review = std::make_shared<Review>(std::move(user), std::move(title),
                                          std::move(message));
    reviews_.push_back(review);
    return review;
}

void Game::add_category(Category category)
{
    if (std::find(categories_.begin(), categories_.end(), category) == categories_.end()) {
        categories_.push_back(category);
    }
}

bool Game::valid_repo() const
{
    return game_repository_.validate();
}

bool Game::invalid_repo() const
{
    return !game_repository_.validate();
}

void Game::handle(Event event)
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
        case State::Draft:
        case State::Validation: game_status_ = Game_status::Draft; break;
        case State::Published: game_status_ = Game_status::Published; break;
        case State::Archived: game_status_ = Game_status::Archived; break;
        }
        return;
    }
}
}  // namespace ogl
