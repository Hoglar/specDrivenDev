#pragma once
#include "ogl/uuid.hpp"
#include <memory>
#include <string>
#include <vector>

namespace ogl {
class User;
class Request;

class Repository
{
public:
    explicit Repository(std::string repository_url);
    std::shared_ptr<Request> add_feature_request(std::shared_ptr<User> user,
                                               std::string title,
                                               std::string description);
    const std::vector<std::shared_ptr<Request>>& show_requests() const;
    bool validate() const;

private:
    uuid id_;
    std::string repository_url_;
    std::vector<std::shared_ptr<Request>> feature_requests_;
};
}  // namespace ogl
