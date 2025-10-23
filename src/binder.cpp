#include "towercpp/binder.hpp"

namespace towercpp {

Binder::Binder(Server& server) : server_(server) {}

std::optional<LSPCallMethod<nlohmann::json>> Binder::getCall(
    const std::string& method) {
    if (!call_handlers.contains(method)) return std::nullopt;
    return call_handlers[method];
}

std::optional<LSPNotificationMethod<nlohmann::json>> Binder::getNotification(
    const std::string& method) {
    if (!notif_handlers.contains(method)) return std::nullopt;
    return notif_handlers[method];
}

}  // namespace towercpp
