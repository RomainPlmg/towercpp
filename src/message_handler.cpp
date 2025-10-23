#include "towercpp/message_handler.hpp"

#include "towercpp/binder.hpp"

namespace towercpp {

ServerMessageHandler::ServerMessageHandler(Server& server) : server_(server) {}

bool ServerMessageHandler::onNotify(const std::string& method,
                                    const nlohmann::json& params) {
    auto handler = server_.getBinder().getNotification(method);
    if (handler.has_value()) {
        (*handler)(params);
        return true;
    }
    return false;
}

bool ServerMessageHandler::onCall(const std::string& method,
                                  const nlohmann::json& params,
                                  const nlohmann::json& id) {
    Reply reply(id, method, &server_);
    auto handler = server_.getBinder().getCall(method);
    if (handler.has_value()) {
        (*handler)(params, std::move(reply));
        return true;
    }
    return false;
}

bool ServerMessageHandler::onReply(const nlohmann::json& id,
                                   const nlohmann::json& result) {
    return true;
}

}  // namespace towercpp