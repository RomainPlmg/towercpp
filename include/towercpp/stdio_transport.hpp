#pragma once

#include "transport.hpp"

namespace towercpp {

class StdioTransport final : public Transport {
   public:
    void notify(const std::string& method,
                const nlohmann::json& params) override;
    void call(const std::string& method, const nlohmann::json& params) override;
    void reply(const nlohmann::json& id, const nlohmann::json& result) override;

    void loop(MessageHandler& handler) override;

   private:
    std::string readMessage();
    bool handleMessage(const nlohmann::json& msg, MessageHandler& handler);
};

}  // namespace towercpp