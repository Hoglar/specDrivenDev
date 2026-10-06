#pragma once
#include <chrono>
#include <memory>
#include <string>
#include <vector>

namespace game_library {
using Date = std::chrono::sys_days;
class Review;
class Review_Comment;

class User {
public:
    enum class State { AwaitingEmailVerification, Active, Suspended, Closed, Deleted };
    enum class Event { EmailConfirmation, CloseAccount, SuspendAccount,
                       ReactivateAccount, ReopenAccount, After30Days };

    User(int id, std::string name, std::string email,
         std::vector<std::weak_ptr<Review>> reviews = {},
         std::vector<std::weak_ptr<Review_Comment>> comments = {});
    void handle(Event event);

private:
    int user_id_;
    std::string user_name_;
    std::string email_;
    std::vector<std::weak_ptr<Review>> reviews_;
    std::vector<std::weak_ptr<Review_Comment>> comments_;
    // Required by the state diagram, absent from its class box: ADR 0001.
    State state_ = State::AwaitingEmailVerification;
    bool suspension_active_ = false;
    bool no_active_suspension() const;

    struct Transition {
        State from;
        Event event;
        State to;
        bool (User::*guard)() const;
        void (User::*action)();
    };
    static constexpr Transition transitions[] = {
        {State::AwaitingEmailVerification, Event::EmailConfirmation, State::Active, nullptr, nullptr},
        {State::Active, Event::CloseAccount, State::Closed, nullptr, nullptr},
        {State::Active, Event::SuspendAccount, State::Suspended, nullptr, nullptr},
        {State::Suspended, Event::CloseAccount, State::Closed, nullptr, nullptr},
        {State::Suspended, Event::ReactivateAccount, State::Active, nullptr, nullptr},
        {State::Closed, Event::ReopenAccount, State::Active, &User::no_active_suspension, nullptr},
        {State::Closed, Event::After30Days, State::Deleted, nullptr, nullptr},
    };
};
} // namespace game_library
