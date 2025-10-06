#include "binder.hpp"

#include "logger.hpp"
#include "reply.hpp"

namespace towercpp {

Binder::Binder(Server& server) : server_(server) {}

LSPCallMethod<nlohmann::json> Binder::getCall(const std::string& method) {
    const auto it = call_handlers.find(method);
    if (it == call_handlers.end()) return {};

    return [f = it->second](const nlohmann::json& params, Reply reply) {
        f(nlohmann::json(params), std::move(reply));
    };
}

LSPNotificationMethod<nlohmann::json> Binder::getNotification(
    const std::string& method) {
    const auto it = notif_handlers.find(method);
    if (it == notif_handlers.end()) return {};

    return [f = it->second](const nlohmann::json& params) {
        f(nlohmann::json(params));
    };
}

}  // namespace towercpp
