#pragma once
#include "ogl/comment.hpp"
#include "ogl/request_status.hpp"

namespace ogl {
class Request : public Comment
{
public:
    enum class State { Pending, Rejected, Accepted, Completed };
    enum class Event { accept, reject, complete, update };

    Request(std::shared_ptr<User> user, std::string title, std::string description);
    void upvote();
    void downvote();
    void update_status(Request_Status status);
    void update(std::string title, std::string description) override;
    void handle(Event event);

private:
    int votes_ = 0;
    Request_Status status_ = Request_Status::Pending;
    State state_ = State::Pending;

    struct Transition
    {
        State from;
        Event event;
        State to;
        bool (Request::*guard)() const;
        void (Request::*action)();
    };
    static constexpr Transition transitions[] = {
        {State::Pending, Event::accept, State::Accepted, nullptr, nullptr},
        {State::Pending, Event::reject, State::Rejected, nullptr, nullptr},
        {State::Accepted, Event::complete, State::Completed, nullptr, nullptr},
        {State::Rejected, Event::update, State::Pending, nullptr, nullptr},
    };
};
}  // namespace ogl
