#pragma once

#include <nlohmann/json.hpp>

namespace towercpp {

struct NoParams {};
inline void from_json([[maybe_unused]] const nlohmann::json& j,
                      [[maybe_unused]] NoParams& p) {
    p = {};
}
inline void to_json(nlohmann::json& j, [[maybe_unused]] NoParams& p) { j = {}; }

/**
 * The LSP any type
 *
 * @since 3.17.0
 */
using LSPAny = nlohmann::json;

/**
 * LSP object definition.
 *
 * @since 3.17.0
 */
using LSPObject = nlohmann::json;

/**
 * LSP arrays.
 *
 * @since 3.17.0
 */
using LSPArray = nlohmann::json;

enum class TraceValue {
    OFF,
    MESSAGES,
    VERBOSE,
};
NLOHMANN_JSON_SERIALIZE_ENUM(TraceValue, {{TraceValue::OFF, "off"},
                                          {TraceValue::MESSAGES, "messages"},
                                          {TraceValue::VERBOSE, "verbose"}})

/**
 * Defines how the host (editor) should sync document changes to the
 * language server.
 */
enum class TextDocumentSyncKind {
    /**
     * Documents should not be synced at all.
     */
    None,

    /**
     * Documents are synced by always sending the full content
     * of the document.
     */
    Full,

    /**
     * Documents are synced by sending the full content on open.
     * After that only incremental updates to the document are
     * sent.
     */
    Incremental,
};
NLOHMANN_JSON_SERIALIZE_ENUM(TextDocumentSyncKind,
                             {{TextDocumentSyncKind::None, 0},
                              {TextDocumentSyncKind::Full, 1},
                              {TextDocumentSyncKind::Incremental, 2}})

/**
 * A set of predefined position encoding kinds.
 *
 * @since 3.17.0
 */
enum class PositionEncodingKind {
    /**
     * Character offsets count UTF-8 code units (e.g bytes).
     */
    UTF8,

    /**
     * Character offsets count UTF-16 code units.
     *
     * This is the default and must always be supported
     * by servers
     */
    UTF16,

    /**
     * Character offsets count UTF-32 code units.
     *
     * Implementation note: these are the same as Unicode code points,
     * so this `PositionEncodingKind` may also be used for an
     * encoding-agnostic representation of character offsets.
     */
    UTF32,
};
NLOHMANN_JSON_SERIALIZE_ENUM(PositionEncodingKind,
                             {{PositionEncodingKind::UTF8, "utf-8"},
                              {PositionEncodingKind::UTF16, "utf-16"},
                              {PositionEncodingKind::UTF32, "utf-32"}})

struct WorkspaceFolder {
    /**
     * The associated URI for this workspace folder.
     */
    std::string uri;

    /**
     * The name of the workspace folder. Used to refer to this
     * workspace folder in the user interface.
     */
    std::string name;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(WorkspaceFolder, uri, name)

struct ClientInfo {
    /**
     * The name of the client as defined by the client.
     */
    std::string name;

    /**
     * The client's version as defined by the client.
     */
    std::string version;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(ClientInfo, name, version)

struct ServerInfo {
    /**
     * The name of the server as defined by the server.
     */
    std::string name;

    /**
     * The server's version as defined by the server.
     */
    std::string version;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(ServerInfo, name, version)

struct TextDocumentSyncOptions {
    /**
     * Open and close notifications are sent to the server. If omitted open
     * close notifications should not be sent.
     */
    bool openClose = false;

    /**
     * Change notifications are sent to the server. See
     * TextDocumentSyncKind.None, TextDocumentSyncKind.Full and
     * TextDocumentSyncKind.Incremental. If omitted it defaults to
     * TextDocumentSyncKind.None.
     */
    TextDocumentSyncKind change = TextDocumentSyncKind::None;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(TextDocumentSyncOptions,
                                                openClose, change)

struct ClientCapabilities {  // TODO
    bool applyEdit = false;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(ClientCapabilities, applyEdit)

struct ServerCapabilities {  // TODO
    /**
     * The position encoding the server picked from the encodings offered
     * by the client via the client capability `general.positionEncodings`.
     *
     * If the client didn't provide any position encodings the only valid
     * value that a server can return is 'utf-16'.
     *
     * If omitted it defaults to 'utf-16'.
     *
     * @since 3.17.0
     */
    PositionEncodingKind positionEncoding = PositionEncodingKind::UTF16;

    /**
     * Defines how text documents are synced. Is either a detailed structure
     * defining each notification or for backwards compatibility the
     * TextDocumentSyncKind number. If omitted it defaults to
     * `TextDocumentSyncKind.None`.
     */
    TextDocumentSyncOptions textDocumentSync;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(ServerCapabilities,
                                                positionEncoding,
                                                textDocumentSync)

struct InitializeParams {
    /**
     * The process Id of the parent process that started the server. Is null
     * if the process has not been started by another process. If the parent
     * process is not alive then the server should exit (see exit
     * notification) its process.
     */
    std::optional<int> processId;

    /**
     * Information about the client
     *
     * @since 3.15.0
     */
    ClientInfo clientInfo;

    /**
     * The locale the client is currently showing the user interface
     * in. This must not necessarily be the locale of the operating
     * system.
     *
     * Uses IETF language tags as the value's syntax
     * (See https://en.wikipedia.org/wiki/IETF_language_tag)
     *
     * @since 3.16.0
     */
    std::string locale;

    /**
     * User provided initialization options.
     */
    LSPAny initializationOptions;

    /**
     * The capabilities provided by the client (editor or tool)
     */
    ClientCapabilities capabilities;

    /**
     * The initial trace setting. If omitted trace is disabled ('off').
     */
    TraceValue trace = TraceValue::OFF;

    /**
     * The workspace folders configured in the client when the server
     * starts. This property is only available if the client supports
     * workspace folders. It can be `null` if the client supports workspace
     * folders but none are configured.
     *
     * @since 3.6.0
     */
    std::vector<WorkspaceFolder> workspaceFolders;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(InitializeParams, processId,
                                                clientInfo, locale,
                                                initializationOptions,
                                                capabilities, trace,
                                                workspaceFolders)

struct InitializeResult {
    /**
     * The capabilities the language server provides.
     */
    ServerCapabilities capabilities;

    /**
     * Information about the server.
     *
     * @since 3.15.0
     */
    ServerInfo serverInfo;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(InitializeResult, capabilities,
                                                serverInfo)

}  // namespace towercpp
