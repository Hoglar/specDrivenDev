#pragma once
#include "ogl/uuid.hpp"
#include <memory>
#include <string>

namespace ogl {
class User;

class Comment
{
public:
    Comment(std::shared_ptr<User> user, std::string title, std::string description);
    virtual ~Comment() = 0;
    virtual void update(std::string title, std::string description);

private:
    uuid id_;
    std::shared_ptr<User> user_;
    std::string title_;
    std::string description_;
};
}  // namespace ogl
