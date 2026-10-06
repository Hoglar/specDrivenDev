#include "game_library/repo_comment.hpp"
#include <utility>
namespace game_library {
Repo_Comment::Repo_Comment(const Developer& author, std::string content, Repository* repository)
    : content_(std::move(content)), repository_(repository) { (void)author; }
void Repo_Comment::edit_content(const std::string& new_content) { content_ = new_content; }
} // namespace game_library
