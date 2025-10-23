#pragma once

#include <nlohmann/json.hpp>

namespace towercpp {

class Transport {
   public:
    virtual ~Transport() = default;

    /// Transport type
    enum class Type {
        STDIO,
    };

    /// Send a notification, don't expect client's response
    virtual void notify(const std::string& method,
                        const nlohmann::json& params) = 0;

    /// Send a request, expect client's response
    virtual void call(const std::string& method,
                      const nlohmann::json& params) = 0;

    /// Send a reply from a client's request
    virtual void reply(const nlohmann::json& id,
                       const nlohmann::json& result) = 0;

    class MessageHandler {
       public:
        virtual ~MessageHandler() = default;
        // Handler returns true to keep processing messages, or false to
        // shut down
        virtual bool onNotify(const std::string& method,
                              const nlohmann::json& params) = 0;
        virtual bool onCall(const std::string& method,
                            const nlohmann::json& params,
                            const nlohmann::json& id) = 0;
        virtual bool onReply(const nlohmann::json& id,
                             const nlohmann::json& result) = 0;
    };

    virtual void loop(MessageHandler& handler) = 0;

    static std::unique_ptr<Transport> create(Type type);
};

}  // namespace towercpp
