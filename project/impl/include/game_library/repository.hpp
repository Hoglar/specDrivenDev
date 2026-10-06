#pragma once
#include <memory>
#include <string>
#include <vector>

namespace game_library {
class Developer;
class Game;
class Repo_Comment;
class Repository {
public:
    int repository_id;
    std::string url;
    Repository(int id, std::string url, Game& game,
               std::vector<std::weak_ptr<Developer>> developers);
    Repository download();
    Repo_Comment add_comment(const Developer& author, const std::string& content);
    void remove_comment(const Repo_Comment& comment);
private:
    std::vector<std::weak_ptr<Developer>> developers_;
    Game* game_;
};
} // namespace game_library
