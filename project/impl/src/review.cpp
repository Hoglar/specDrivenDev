#include "game_library/review.hpp"
#include "game_library/review_comment.hpp"
#include <utility>
namespace game_library {
Review::Review(const User& author, std::string text, Date created,
               std::vector<std::weak_ptr<Review_Comment>> comments)
    : date(created), content(std::move(text)), comments_(std::move(comments)) {
    // UML places the author-to-review relation on User, not Review.
    (void)author;
}
Review_Comment Review::add_comment(const User& author, const std::string& text) {
    return Review_Comment(author, text);
}
void Review::remove_comment(const Review_Comment& comment) {
    // Placeholder: comment identity/removal policy is not specified.
    (void)comment;
}
} // namespace game_library
