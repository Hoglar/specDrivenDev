#include "game_library/review_comment.hpp"
#include <utility>
namespace game_library {
Review_Comment::Review_Comment(const User& author, std::string content)
    : content_(std::move(content)) { (void)author; }
void Review_Comment::edit_content(const std::string& new_content) { content_ = new_content; }
} // namespace game_library
