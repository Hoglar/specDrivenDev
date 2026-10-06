#pragma once
#include "game_library/user.hpp"

namespace game_library {
class Review_Comment;
class Review {
public:
    Date date;
    std::string content;
    Review(const User& author, std::string content, Date date = Date{},
           std::vector<std::weak_ptr<Review_Comment>> comments = {});
    Review_Comment add_comment(const User& author, const std::string& content);
    void remove_comment(const Review_Comment& comment);
private:
    std::vector<std::weak_ptr<Review_Comment>> comments_;
};
} // namespace game_library
