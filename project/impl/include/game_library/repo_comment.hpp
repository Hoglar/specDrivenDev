#pragma once
#include <string>

namespace game_library {
class Developer;
class Repository;
class Repo_Comment {
public:
    Repo_Comment(const Developer& author, std::string content, Repository* repository = nullptr);
    void edit_content(const std::string& new_content);
private:
    std::string content_;
    Repository* repository_;
};
} // namespace game_library
