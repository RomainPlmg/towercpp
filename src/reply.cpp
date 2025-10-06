#include "reply.hpp"

#include "logger.hpp"

namespace towercpp {

Reply::Reply(const nlohmann::json& id, const std::string& method,
             Server* server)
    : start_(std::chrono::steady_clock::now()),
      id_(id),
      method_(method),
      server_(server) {
    assert(server_);
}

Reply::Reply(Reply&& other)
    : replied_(other.replied_.load()),
      start_(other.start_),
      id_(std::move(other.id_)),
      method_(std::move(other.method_)),
      server_(other.server_) {
    other.server_ = nullptr;
}

void Reply::operator()(const nlohmann::json& reply) {
    assert(server_);
    if (replied_.exchange(true)) {
        LOG_ERROR("Replied twice to message {}({})", method_, id_.dump());
        throw std::runtime_error("Must reply to each call only once");
    }

    const auto duration = std::chrono::steady_clock::now() - start_;

    if (!reply.empty()) {
        LOG_TRACE(
            "-> reply:{}({}) {}ms", method_, id_.dump(),
            std::chrono::duration_cast<std::chrono::milliseconds>(duration)
                .count());
        server_->getTransport().reply(id_, reply);
    }
}

}  // namespace towercpp