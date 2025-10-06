#pragma once

#include <nlohmann/json.hpp>

#include "server.hpp"

namespace towercpp {

class Reply {
   public:
    Reply(const nlohmann::json& id, const std::string& method, Server* server);
    Reply(Reply&& other);
    Reply& operator=(Reply&&) = delete;
    Reply(const Reply&) = delete;
    Reply& operator=(const Reply&) = delete;

    void operator()(const nlohmann::json& reply);

    // Getters
    [[nodiscard]] nlohmann::json getId() const { return id_; }
    [[nodiscard]] std::string getMethod() const { return method_; }

   private:
    std::atomic<bool> replied_ = false;
    std::chrono::steady_clock::time_point start_;
    nlohmann::json id_;
    std::string method_;
    Server* server_ = nullptr;
};

}  // namespace towercpp
