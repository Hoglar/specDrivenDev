#include "game_library/developer.hpp"
#include <utility>
namespace game_library {
Developer::Developer(int id, std::string name, std::string email,
                     std::string webpage, std::string biography, Repo_Comment* comment)
    : User(id, std::move(name), std::move(email)), webpage_url(std::move(webpage)),
      bio(std::move(biography)), comment_(comment) {}
void Developer::create_repository() {
    // Placeholder: creation details and ownership are not specified by UML.
}
void Developer::join_repository(const Repository& repository) {
    // Placeholder: UML has no mechanism to obtain an owning developer handle.
    (void)repository;
}
} // namespace game_library
