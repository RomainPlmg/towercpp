#pragma once

#include "message_handler.hpp"
#include "protocol.hpp"
#include "transport.hpp"

namespace towercpp {

class Reply;
class Binder;

class Server {
   public:
    explicit Server(std::unique_ptr<Transport> transport);
    ~Server();

    bool run();

    void showMessage(const std::string& msg);

    // Getters
    Transport& getTransport() { return *p_transport_; }
    Binder& getBinder() { return *p_binder_; }

   private:
    std::unique_ptr<Transport> p_transport_;
    std::unique_ptr<ServerMessageHandler> p_msg_handler_;
    std::unique_ptr<Binder> p_binder_;

    void onInitialize(const InitializeParams& params, Reply reply);
    void onInitialized(const NoParams& params);
};

}  // namespace towercpp
