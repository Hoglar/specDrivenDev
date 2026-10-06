#include "game_library/developer.hpp"
#include "game_library/game.hpp"
#include "game_library/game_page.hpp"
#include "game_library/repo_comment.hpp"
#include "game_library/repository.hpp"
#include "game_library/review.hpp"
#include "game_library/review_comment.hpp"
#include <cassert>
#include <iostream>
#include <memory>

int main() {
    using namespace game_library;
    User user(1, "player", "player@example.test");
    user.handle(User::Event::EmailConfirmation);
    auto developer = std::make_shared<Developer>(2, "developer", "dev@example.test", "", "Demo");
    Game game(1, "Open Game", "0.1", Date{std::chrono::year{2026}/10/6});
    Repository repository(1, "https://example.test/game", game, {developer});
    game_page page;
    auto review = page.submit_review(game, user, "A deterministic review");
    assert(review.content == "A deterministic review");
    auto comment = review.add_comment(user, "A comment");
    comment.edit_content("Updated comment");
    review.remove_comment(comment);
    auto repo_comment = repository.add_comment(*developer, "A repository comment");
    repo_comment.edit_content("Updated repository comment");
    repository.remove_comment(repo_comment);
    developer->create_repository();
    developer->join_repository(repository);
    const auto downloaded_repository = repository.download();
    assert(downloaded_repository.repository_id == repository.repository_id);
    assert(downloaded_repository.url == repository.url);
    const auto downloaded_game = game.download();
    (void)downloaded_game;
    std::cout << "C++ UML mock-up: " << review.content << '\n';
}
