#pragma once

#include <nlohmann/json.hpp>

#include "transport.hpp"

namespace towercpp {

class Server;

class ServerMessageHandler final : public Transport::MessageHandler {
   public:
    explicit ServerMessageHandler(Server& server);

    bool onNotify(const std::string& method,
                  const nlohmann::json& params) override;
    bool onCall(const std::string& method, const nlohmann::json& params,
                const nlohmann::json& id) override;
    bool onReply(const nlohmann::json& id,
                 const nlohmann::json& result) override;

   private:
    Server& server_;
};

}  // namespace Chipy
