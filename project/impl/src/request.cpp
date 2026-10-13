#include "ogl/request.hpp"
#include <limits>
#include <stdexcept>
#include <utility>

namespace ogl {
Request::Request(std::shared_ptr<User> user, std::string title, std::string description)
    : Comment(std::move(user), std::move(title), std::move(description))
{
}

void Request::upvote()
{
    if (votes_ == std::numeric_limits<int>::max()) {
        throw std::overflow_error("Vote count overflow");
    }
    ++votes_;
}

void Request::downvote()
{
    if (votes_ == std::numeric_limits<int>::min()) {
        throw std::overflow_error("Vote count underflow");
    }
    --votes_;
}

void Request::update_status(Request_Status status)
{
    switch (status) {
    case Request_Status::Pending: handle(Event::update); break;
    case Request_Status::Rejected: handle(Event::reject); break;
    case Request_Status::Accepted: handle(Event::accept); break;
    case Request_Status::Completed: handle(Event::complete); break;
    }
}

void Request::update(std::string title, std::string description)
{
    Comment::update(std::move(title), std::move(description));
    if (state_ == State::Rejected) {
        update_status(Request_Status::Pending);
    }
}

void Request::handle(Event event)
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
        case State::Pending: status_ = Request_Status::Pending; break;
        case State::Rejected: status_ = Request_Status::Rejected; break;
        case State::Accepted: status_ = Request_Status::Accepted; break;
        case State::Completed: status_ = Request_Status::Completed; break;
        }
        return;
    }
}
}  // namespace ogl
