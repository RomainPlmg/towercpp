#include "server.hpp"

#include "binder.hpp"
#include "logger.hpp"

#ifdef _WIN32
#include <fcntl.h>
#include <io.h>
#endif

namespace towercpp {

Server::Server(std::unique_ptr<Transport> transport)
    : p_transport_(std::move(transport)) {
    // On Windows, force binary stdin, stdout & stderr
#ifdef _WIN32
    _setmode(_fileno(stdin), _O_BINARY);
    _setmode(_fileno(stdout), _O_BINARY);
    _setmode(_fileno(stderr), _O_BINARY);
#endif
    // Disable buffering for queek I/O
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    assert(p_transport_);
    p_msg_handler_ = std::make_unique<ServerMessageHandler>(*this);
    p_binder_ = std::make_unique<Binder>(*this);
}

Server::~Server() { LOG_INFO("Server shutdown"); }

bool Server::run() {
    try {
        p_transport_->loop(*p_msg_handler_);
    } catch (const std::exception& e) {
        LOG_ERROR("Server error: {}", e.what());
        return false;
    }

    return true;
}

void Server::showMessage(const std::string& msg) {
    nlohmann::json params;
    params["type"] = 3;
    params["message"] = msg;

    p_transport_->notify("window/showMessage", params);
}

}  // namespace towercpp