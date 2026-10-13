#pragma once
#include "ogl/user_status.hpp"
#include "ogl/uuid.hpp"
#include <memory>
#include <string>
#include <vector>

namespace ogl {
class Game;

class User : public std::enable_shared_from_this<User>
{
public:
    enum class State { AwaitingEmailVerification, Active, Suspended, Closed, Deleted };
    enum class Event {
        email_confirmation, suspend_account, reactivate_account,
        close_account, reopen_account, after_30_days
    };

    User(uuid id, std::string username);
    User(const User&) = delete;
    User& operator=(const User&) = delete;
    void add_game_to_library(std::shared_ptr<Game> game);
    std::shared_ptr<Game> create_new_game(std::string name, std::string repo_url);
    void handle(Event event);

private:
    uuid id_;
    std::string username_;
    std::vector<std::weak_ptr<Game>> game_library_;
    bool marked_for_delete_ = false;
    User_Status user_status_ = User_Status::Pending;
    State state_ = State::AwaitingEmailVerification;

    bool no_active_suspension() const;

    struct Transition
    {
        State from;
        Event event;
        State to;
        bool (User::*guard)() const;
        void (User::*action)();
    };
    static constexpr Transition transitions[] = {
        {State::AwaitingEmailVerification, Event::email_confirmation, State::Active, nullptr, nullptr},
        {State::Active, Event::suspend_account, State::Suspended, nullptr, nullptr},
        {State::Suspended, Event::reactivate_account, State::Active, nullptr, nullptr},
        {State::Active, Event::close_account, State::Closed, nullptr, nullptr},
        {State::Suspended, Event::close_account, State::Closed, nullptr, nullptr},
        {State::Closed, Event::reopen_account, State::Active, &User::no_active_suspension, nullptr},
        {State::Closed, Event::after_30_days, State::Deleted, nullptr, nullptr},
    };
};
}  // namespace ogl
