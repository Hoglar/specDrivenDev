#include "game_library/user.hpp"
#include <utility>

namespace game_library {
User::User(int id, std::string name, std::string email,
           std::vector<std::weak_ptr<Review>> reviews,
           std::vector<std::weak_ptr<Review_Comment>> comments)
    : user_id_(id), user_name_(std::move(name)), email_(std::move(email)),
      reviews_(std::move(reviews)), comments_(std::move(comments)) {}

bool User::no_active_suspension() const { return !suspension_active_; }

void User::handle(Event event) {
    for (const auto& transition : transitions) {
        if (transition.from != state_ || transition.event != event) continue;
        if (transition.guard && !(this->*transition.guard)()) continue;
        if (transition.action) (this->*transition.action)();
        // Preserve a suspension across closing/reopening the account.
        if (event == Event::SuspendAccount) suspension_active_ = true;
        if (event == Event::ReactivateAccount) suspension_active_ = false;
        state_ = transition.to;
        return;
    }
}
} // namespace game_library
