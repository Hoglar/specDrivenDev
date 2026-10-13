#include "ogl/review.hpp"
#include <utility>

namespace ogl {
Review::Review(std::shared_ptr<User> user, std::string title, std::string message)
    : Comment(std::move(user), std::move(title), std::move(message))
{
}

void Review::update_score(int score)
{
    score_ = score;
}
}  // namespace ogl
