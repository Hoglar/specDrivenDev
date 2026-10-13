#pragma once
#include "ogl/comment.hpp"

namespace ogl {
class Review : public Comment
{
public:
    Review(std::shared_ptr<User> user, std::string title, std::string message);
    void update_score(int score);

private:
    int score_ = 0;
};
}  // namespace ogl
