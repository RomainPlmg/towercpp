#pragma once

#include <nlohmann/json.hpp>

#include "reply.hpp"

namespace towercpp {

class Server;

template <typename Params>
using LSPCallMethod = std::function<void(const Params&, Reply)>;
template <typename Params>
using LSPNotificationMethod = std::function<void(const Params&)>;

class Binder {
   public:
    explicit Binder(Server& server);

    LSPCallMethod<nlohmann::json> getCall(const std::string& method);
    LSPNotificationMethod<nlohmann::json> getNotification(
        const std::string& method);

    template <typename Obj, typename Params>
    void bind(const std::string& name, Obj* obj,
              void (Obj::*method)(const Params&, Reply));
    template <typename Obj, typename Params>
    void bind(const std::string& name, Obj* obj,
              void (Obj::*method)(const Params&));

   private:
    Server& server_;

    // Mapping tables
    std::unordered_map<std::string, LSPCallMethod<nlohmann::json>>
        call_handlers;
    std::unordered_map<std::string, LSPNotificationMethod<nlohmann::json>>
        notif_handlers;
};

template <typename Obj, typename Params>
void Binder::bind(const std::string& name, Obj* obj,
                  void (Obj::*method)(const Params&, Reply)) {
    call_handlers[name] = [obj, method](const Params& params, Reply reply) {
        (obj->*method)(params, std::move(reply));
    };
}

template <typename Obj, typename Params>
void Binder::bind(const std::string& name, Obj* obj,
                  void (Obj::*method)(const Params&)) {
    notif_handlers[name] = [obj, method](const Params& params) {
        (obj->*method)(params);
    };
}

}  // namespace towercpp
