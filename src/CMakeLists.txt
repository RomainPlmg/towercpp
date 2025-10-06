FetchContent_Declare(
        json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG develop
)
FetchContent_MakeAvailable(json)

FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG v1.15.3
)
FetchContent_MakeAvailable(spdlog)

set(SRC
        pch.cpp
        logger.cpp
        binder.cpp
        message_handler.cpp
        reply.cpp
        server.cpp
        stdio_transport.cpp
        transport.cpp
)

add_library(towercpp STATIC ${SRC})
target_precompile_headers(towercpp PRIVATE pch.hpp)
target_link_libraries(towercpp PRIVATE nlohmann_json::nlohmann_json spdlog::spdlog)