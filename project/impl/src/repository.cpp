#include "game_library/repository.hpp"
#include "game_library/repo_comment.hpp"
#include <stdexcept>
#include <utility>
namespace game_library {
Repository::Repository(int id, std::string link, Game& game,
                       std::vector<std::weak_ptr<Developer>> developers)
    : repository_id(id), url(std::move(link)), developers_(std::move(developers)), game_(&game) {
    bool has_developer = false;
    for (const auto& developer : developers_) has_developer |= !developer.expired();
    if (!has_developer) throw std::invalid_argument("Repository needs at least one developer");
}
Repository Repository::download() { return *this; }
Repo_Comment Repository::add_comment(const Developer& author, const std::string& content) {
    return Repo_Comment(author, content, this);
}
void Repository::remove_comment(const Repo_Comment& comment) {
    // Placeholder: this diagram defines no repository-owned comment collection.
    (void)comment;
}
} // namespace game_library
