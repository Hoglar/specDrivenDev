#include "ogl/comment.hpp"
#include <cstddef>
#include <stdexcept>
#include <utility>

namespace ogl {
Comment::Comment(std::shared_ptr<User> user, std::string title, std::string description)
    : user_(std::move(user)), title_(std::move(title)), description_(std::move(description))
{
    if (!user_) {
        throw std::invalid_argument("A comment requires a user");
    }
    static std::size_t next_id = 0;
    id_ = "comment-" + std::to_string(++next_id);
}

Comment::~Comment() = default;

void Comment::update(std::string title, std::string description)
{
    title_ = std::move(title);
    description_ = std::move(description);
}
}  // namespace ogl
