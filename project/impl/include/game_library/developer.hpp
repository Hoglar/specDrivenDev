#pragma once
#include "game_library/user.hpp"

namespace game_library {
class Repository;
class Repo_Comment;
class Developer : public User {
public:
    std::string webpage_url;
    std::string bio;
    Developer(int id, std::string name, std::string email,
              std::string webpage_url, std::string bio, Repo_Comment* comment = nullptr);
    void create_repository();
    void join_repository(const Repository& repository);
private:
    Repo_Comment* comment_;
};
} // namespace game_library
