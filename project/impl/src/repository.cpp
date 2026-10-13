#include "ogl/repository.hpp"
#include "ogl/request.hpp"
#include <cctype>
#include <cstddef>
#include <utility>

namespace ogl {
Repository::Repository(std::string repository_url)
    : repository_url_(std::move(repository_url))
{
    static std::size_t next_id = 0;
    id_ = "repository-" + std::to_string(++next_id);
}

std::shared_ptr<Request> Repository::add_feature_request(std::shared_ptr<User> user,
                                                       std::string title,
                                                       std::string description)
{
    auto request = std::make_shared<Request>(std::move(user), std::move(title),
                                             std::move(description));
    feature_requests_.push_back(request);
    return request;
}

const std::vector<std::shared_ptr<Request>>& Repository::show_requests() const
{
    return feature_requests_;
}

bool Repository::validate() const
{
    const auto separator = repository_url_.find("://");
    if (separator == std::string::npos) {
        return false;
    }
    const auto scheme = repository_url_.substr(0, separator);
    if (scheme != "https" && scheme != "http" && scheme != "ssh" && scheme != "git") {
        return false;
    }
    const auto host_start = separator + 3;
    const auto path_start = repository_url_.find('/', host_start);
    if (path_start == std::string::npos || path_start == host_start ||
        path_start + 1 == repository_url_.size()) {
        return false;
    }
    for (unsigned char character : repository_url_) {
        if (std::isspace(character) || std::iscntrl(character)) {
            return false;
        }
    }
    return true;
}
}  // namespace ogl
