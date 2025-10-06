#include "message_handler.hpp"

#include "binder.hpp"

namespace towercpp {

ServerMessageHandler::ServerMessageHandler(Server& server) : server_(server) {}

bool ServerMessageHandler::onNotify(const std::string& method,
                                    const nlohmann::json& params) {
    if (const auto handler = server_.getBinder().getNotification(method)) {
        handler(params);
        return true;
    }

    return false;
}

bool ServerMessageHandler::onCall(const std::string& method,
                                  const nlohmann::json& params,
                                  const nlohmann::json& id) {
    Reply reply(id, method, &server_);
    if (const auto handler = server_.getBinder().getCall(method)) {
        handler(params, std::move(reply));
        return true;
    }

    return false;
}

bool ServerMessageHandler::onReply(const nlohmann::json& id,
                                   const nlohmann::json& result) {
    return true;
}

}  // namespace Chipy