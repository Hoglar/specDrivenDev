#pragma once
#include <string>

namespace game_library {
class User;
class Review_Comment {
public:
    Review_Comment(const User& author, std::string content);
    void edit_content(const std::string& new_content);
private:
    std::string content_;
};
} // namespace game_library
