// One deterministic path through the alt fragment: Accepted -> Completed.
// The requester, players and developer lifelines represent external actors.
#include "ogl/repository.hpp"
#include "ogl/request.hpp"
#include "ogl/user.hpp"
#include <memory>

namespace {
void scenario(ogl::Repository& repository, const std::shared_ptr<ogl::User>& requester)
{
    auto request = repository.add_feature_request(requester, "More levels", "Add a second chapter.");
    request->upvote();     // requester
    request->upvote();     // players
    request->downvote();   // players
    request->update_status(ogl::Request_Status::Accepted);   // developer
    request->update_status(ogl::Request_Status::Completed);  // developer
}
}  // namespace

int main()
{
    auto requester = std::make_shared<ogl::User>("requester-1", "requester");
    requester->handle(ogl::User::Event::email_confirmation);
    ogl::Repository repository("https://example.org/games/adventure.git");
    scenario(repository, requester);
    return 0;
}
