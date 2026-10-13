#pragma once
#include "ogl/category.hpp"
#include "ogl/game_status.hpp"
#include "ogl/repository.hpp"
#include "ogl/uuid.hpp"
#include <memory>
#include <string>
#include <vector>

namespace ogl {
class User;
class Review;

class Game
{
public:
    enum class State { Draft, Validation, Published, Archived };
    enum class Event { submit, validate, archiveGame };

    Game(std::shared_ptr<User> creator, std::string name, std::string repo_url);
    std::shared_ptr<Review> add_review(std::shared_ptr<User> user,
                                     std::string title, std::string message);
    void add_category(Category category);
    void handle(Event event);

private:
    uuid id_;
    std::string name_;
    std::shared_ptr<User> creator_;
    Game_status game_status_ = Game_status::Draft;
    Repository game_repository_;
    std::vector<Category> categories_;
    std::vector<std::shared_ptr<Review>> reviews_;
    State state_ = State::Draft;

    bool valid_repo() const;
    bool invalid_repo() const;

    struct Transition
    {
        State from;
        Event event;
        State to;
        bool (Game::*guard)() const;
        void (Game::*action)();
    };
    static constexpr Transition transitions[] = {
        {State::Draft, Event::submit, State::Validation, nullptr, nullptr},
        {State::Validation, Event::validate, State::Published, &Game::valid_repo, nullptr},
        {State::Validation, Event::validate, State::Draft, &Game::invalid_repo, nullptr},
        {State::Published, Event::archiveGame, State::Archived, nullptr, nullptr},
    };
};
}  // namespace ogl
