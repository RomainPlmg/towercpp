#include "towercpp/stdio_transport.hpp"

#include <iostream>

#include "towercpp/logger.hpp"


namespace towercpp {

void StdioTransport::notify(const std::string& method,
                            const nlohmann::json& params) {
    nlohmann::json msg = {
        {"jsonrpc", "2.0"},
        {"method", method},
        {"params", params},
    };

    const std::string content = msg.dump();
    const std::string notif =
        "Content-Length: " + std::to_string(content.size()) + "\r\n\r\n" +
        content;

    std::cout << notif << std::flush;
}

void StdioTransport::call(const std::string& method,
                          const nlohmann::json& params) {
    nlohmann::json msg = {
        {"jsonrpc", "2.0"},
        {"id", "1"},  // TODO: for test
        {"method", method},
        {"params", params},
    };
}

void StdioTransport::reply(const nlohmann::json& id,
                           const nlohmann::json& result) {
    const nlohmann::json params = {
        {"jsonrpc", "2.0"},
        {"id", id},
        {"result", result},
    };

    const std::string content = params.dump();
    const std::string reply =
        "Content-Length: " + std::to_string(content.size()) + "\r\n\r\n" +
        content;

    std::cout << reply << std::flush;
}

void StdioTransport::loop(MessageHandler& handler) {
    while (true) {
        std::string msg = readMessage();  // Message buffer
        if (msg.empty()) {
            if (std::cin.eof()) {
                LOG_INFO("Client disconnected");
                break;
            }
            continue;
        }

        nlohmann::json request;
        try {
            request = nlohmann::json::parse(msg);
        } catch (const std::exception& e) {
            LOG_ERROR("JSON parse error: {}", e.what());
        }

        handleMessage(request, handler);
    }
}

std::string StdioTransport::readMessage() {
    std::string line;
    uint64_t content_length = 0;

    while (std::getline(std::cin, line)) {
        // Remove the \r if present
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        if (line.rfind("Content-Length:", 0) == 0) {
            content_length = std::stoi(line.substr(15));
        }

        // Empty line = header end
        if (line.empty()) {
            break;
        }
    }

    std::string content(content_length, '\0');
    std::cin.read(&content[0], content_length);
    return content;
}

bool StdioTransport::handleMessage(const nlohmann::json& msg,
                                   MessageHandler& handler) {
    if (msg.empty() || msg.value("jsonrpc", "") != "2.0") {
        LOG_ERROR("Not a JSON-RPC 2.0 message: {}", msg.dump());
        return false;
    }

    // ID may be any JSON value. If absent, this is a notification
    std::optional<nlohmann::json> id;
    if (msg.contains("id")) {
        id = msg["id"];
    }

    // Method
    const auto method = msg.value("method", "");
    if (method.empty()) {
        if (!id.has_value()) {
            LOG_ERROR("No method and no response ID: {}", msg.dump());
        }
    }

    // Params
    const nlohmann::json params = msg.value("params", nlohmann::json::object());

    if (id.has_value()) {
        return handler.onCall(method, params, id.value());
    }
    return handler.onNotify(method, params);
}

}  // namespace towercpp
