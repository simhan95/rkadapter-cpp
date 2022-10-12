cmake_minimum_required(VERSION 3.1)


set(REDIS_PLUS_PLUS_VERSION "1.3.5")
message(STATUS "redis-plus-plus version: ${REDIS_PLUS_PLUS_VERSION}")

project(redis++ LANGUAGES CXX VERSION ${REDIS_PLUS_PLUS_VERSION})

set(REDIS_PLUS_PLUS_DEFAULT_BUILD_TYPE "Release")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE ${REDIS_PLUS_PLUS_DEFAULT_BUILD_TYPE} CACHE STRING "Set build type" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "RelWithDebInfo" "MinSizeRel")
endif()
message(STATUS "redis-plus-plus build type: ${CMAKE_BUILD_TYPE}")

set(REDIS_PLUS_PLUS_DEFAULT_CXX_STANDARD 17)
if(NOT REDIS_PLUS_PLUS_CXX_STANDARD)
    set(REDIS_PLUS_PLUS_CXX_STANDARD ${REDIS_PLUS_PLUS_DEFAULT_CXX_STANDARD} CACHE STRING "Set CXX standard" FORCE)
    set_property(CACHE REDIS_PLUS_PLUS_CXX_STANDARD PROPERTY STRINGS "11" "14" "17" "20")
endif()
message(STATUS "redis-plus-plus build with CXX standard: c++${REDIS_PLUS_PLUS_CXX_STANDARD}")

if(NOT WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++${REDIS_PLUS_PLUS_CXX_STANDARD}")
else()
    if(MSVC)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++${REDIS_PLUS_PLUS_CXX_STANDARD}")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++${REDIS_PLUS_PLUS_CXX_STANDARD}")
    endif()
endif()

if(REDIS_PLUS_PLUS_BUILD_ASYNC)
    if(REDIS_PLUS_PLUS_BUILD_ASYNC STREQUAL "libuv")
        message(STATUS "redis-plus-plus build async interface with libuv")

        # libuv dependency
        find_path(REDIS_PLUS_PLUS_ASYNC_LIB_HEADER NAMES uv.h)
        find_library(REDIS_PLUS_PLUS_ASYNC_LIB uv)
    else()
        message(FATAL_ERROR "invalid REDIS_PLUS_PLUS_BUILD_ASYNC")
    endif()
endif()

if(REDIS_PLUS_PLUS_BUILD_CORO)
    if(NOT REDIS_PLUS_PLUS_BUILD_ASYNC)
        message(FATAL_ERROR "In order to build coroutine support, you must set REDIS_PLUS_PLUS_BUILD_ASYNC")
    endif()

    message(STATUS "redis-plus-plus build coroutine support: " ${REDIS_PLUS_PLUS_BUILD_CORO})
endif()

set(SOURCE_DIR src)

set(REDIS_PLUS_PLUS_SOURCES
        "${SOURCE_DIR}/command.cpp"
        "${SOURCE_DIR}/command_options.cpp"
        "${SOURCE_DIR}/connection.cpp"
        "${SOURCE_DIR}/connection_pool.cpp"
        "${SOURCE_DIR}/crc16.cpp"
        "${SOURCE_DIR}/errors.cpp"
        "${SOURCE_DIR}/pipeline.cpp"
        "${SOURCE_DIR}/redis.cpp"
        "${SOURCE_DIR}/redis_cluster.cpp"
        "${SOURCE_DIR}/reply.cpp"
        "${SOURCE_DIR}/sentinel.cpp"
        "${SOURCE_DIR}/shards.cpp"
        "${SOURCE_DIR}/shards_pool.cpp"
        "${SOURCE_DIR}/subscriber.cpp"
        "${SOURCE_DIR}/transaction.cpp"
)

set(HIREDIS_LIB "C:/Users/vjayarag/Documents/GitHub/rkadapter/lib/hiredis.lib")
set(RDKAFKA_LIB "C:/Users/vjayarag/Documents/GitHub/rkadapter/lib/rdkafka.lib")
set(JSONCPP_LIB "C:/Users/vjayarag/Documents/GitHub/rkadapter/lib/jsoncpp.lib")
set(JSONCPP_STATIC_LIB "C:/Users/vjayarag/Documents/GitHub/rkadapter/lib/jsoncpp_static.lib")
set(RDKAFKA_PLUS_LIB "C:/Users/vjayarag/Documents/GitHub/rkadapter/lib/rdkafka++.lib")
SET(HIREDIS_HEADER "C:/Users/vjayarag/Documents/GitHub/rkadapter/lib/hiredis/")

if(REDIS_PLUS_PLUS_BUILD_ASYNC)
    list(APPEND REDIS_PLUS_PLUS_SOURCES
        "${SOURCE_DIR}/async_connection.cpp"
        "${SOURCE_DIR}/async_connection_pool.cpp"
        "${SOURCE_DIR}/async_redis.cpp"
        "${SOURCE_DIR}/event_loop.cpp"
        "${SOURCE_DIR}/async_sentinel.cpp"
        "${SOURCE_DIR}/async_redis_cluster.cpp"
        "${SOURCE_DIR}/async_shards_pool.cpp"
        "${SOURCE_DIR}/async_subscriber.cpp"
        "${SOURCE_DIR}/async_subscriber_impl.cpp"
    )

    if(NOT REDIS_PLUS_PLUS_ASYNC_FUTURE)
        set(REDIS_PLUS_PLUS_ASYNC_FUTURE "std")
    endif()

    if(REDIS_PLUS_PLUS_ASYNC_FUTURE STREQUAL "std")
        set(REDIS_PLUS_PLUS_ASYNC_FUTURE_HEADER "${SOURCE_DIR}/future/std")
    elseif(REDIS_PLUS_PLUS_ASYNC_FUTURE STREQUAL "boost")
        set(REDIS_PLUS_PLUS_ASYNC_FUTURE_HEADER "${SOURCE_DIR}/future/boost")
        find_package(Boost REQUIRED COMPONENTS system thread)
    else()
        message(FATAL_ERROR "invalid REDIS_PLUS_PLUS_ASYNC_FUTURE")
    endif()
endif()

# cxx utils
if(REDIS_PLUS_PLUS_CXX_STANDARD LESS 17)
    set(CXX_UTILS_DIR "${SOURCE_DIR}/cxx11")
else()
    set(CXX_UTILS_DIR "${SOURCE_DIR}/cxx17")
endif()

# TLS support
option(REDIS_PLUS_PLUS_USE_TLS "Build with TLS support" OFF)
message(STATUS "redis-plus-plus TLS support: ${REDIS_PLUS_PLUS_USE_TLS}")

if(REDIS_PLUS_PLUS_USE_TLS)
    set(TLS_SUB_DIR "${SOURCE_DIR}/tls")

    list(APPEND REDIS_PLUS_PLUS_SOURCES "${TLS_SUB_DIR}/tls.cpp")

    set(REDIS_PLUS_PLUS_DEPENDS "hiredis,hiredis_ssl")
else()
    set(TLS_SUB_DIR "${SOURCE_DIR}/no_tls")

    set(REDIS_PLUS_PLUS_DEPENDS "hiredis")
endif()


# hiredis dependency
find_package(hiredis QUIET)
list(APPEND REDIS_PLUS_PLUS_HIREDIS_LIBS ${RDKAFKA_LIB})
list(APPEND REDIS_PLUS_PLUS_HIREDIS_LIBS ${RDKAFKA_PLUS_LIB})
list(APPEND REDIS_PLUS_PLUS_HIREDIS_LIBS ${JSONCPP_STATIC_LIB})
list(APPEND REDIS_PLUS_PLUS_HIREDIS_LIBS ${JSONCPP_LIB})
if(hiredis_FOUND)
    list(APPEND REDIS_PLUS_PLUS_HIREDIS_LIBS hiredis::hiredis)

    if(REDIS_PLUS_PLUS_USE_TLS)
        find_package(hiredis_ssl REQUIRED)
        list(APPEND REDIS_PLUS_PLUS_HIREDIS_LIBS hiredis::hiredis_ssl)
    endif()
else()
    find_path(HIREDIS_HEADER hiredis)
    find_library(HIREDIS_LIB hiredis)
    list(APPEND REDIS_PLUS_PLUS_HIREDIS_LIBS ${HIREDIS_LIB})

    if(REDIS_PLUS_PLUS_USE_TLS)
        find_library(HIREDIS_TLS_LIB hiredis_ssl)
        list(APPEND REDIS_PLUS_PLUS_HIREDIS_LIBS ${HIREDIS_TLS_LIB})
    endif()
endif()

# Build static library
option(REDIS_PLUS_PLUS_BUILD_STATIC "Build static library" ON)
message(STATUS "redis-plus-plus build static library: ${REDIS_PLUS_PLUS_BUILD_STATIC}")

if(REDIS_PLUS_PLUS_BUILD_STATIC)
    set(STATIC_LIB redis++_static)
    #add_library(${STATIC_LIB} STATIC ${REDIS_PLUS_PLUS_SOURCES})
    #add_library(redis++::${STATIC_LIB} ALIAS ${STATIC_LIB})
    #target_link_libraries(${STATIC_LIB} PUBLIC ${REDIS_PLUS_PLUS_HIREDIS_LIBS})
    message(STATUS " library names ${REDIS_PLUS_PLUS_HIREDIS_LIBS}")
    list(APPEND REDIS_PLUS_PLUS_TARGETS ${STATIC_LIB})

    #target_include_directories(${STATIC_LIB} PUBLIC
      #      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE_DIR}>
      #      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${TLS_SUB_DIR}>
      #      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${CXX_UTILS_DIR}>
      #      $<INSTALL_INTERFACE:include>)

    if(hiredis_FOUND)
        target_include_directories(${STATIC_LIB} PUBLIC $<BUILD_INTERFACE:${hiredis_INCLUDE_DIRS}>)
        target_link_libraries(${STATIC_LIB} PUBLIC ${REDIS_PLUS_PLUS_HIREDIS_LIBS})
    else()
        #target_include_directories(${STATIC_LIB} PUBLIC $<BUILD_INTERFACE:${HIREDIS_HEADER}>)
    endif()

    if(REDIS_PLUS_PLUS_BUILD_ASYNC)
        target_include_directories(${STATIC_LIB} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${REDIS_PLUS_PLUS_ASYNC_FUTURE_HEADER}>)
        target_include_directories(${STATIC_LIB} PUBLIC $<BUILD_INTERFACE:${REDIS_PLUS_PLUS_ASYNC_LIB_HEADER}>)
        if(REDIS_PLUS_PLUS_ASYNC_FUTURE STREQUAL "boost")
            target_include_directories(${STATIC_LIB} SYSTEM PUBLIC $<BUILD_INTERFACE:${Boost_INCLUDE_DIR}>)
        endif()
    endif()

    

    

    option(REDIS_PLUS_PLUS_BUILD_STATIC_WITH_PIC "Build static library with position independent code" ON)
    message(STATUS "redis-plus-plus build static library with position independent code: ${REDIS_PLUS_PLUS_BUILD_STATIC_WITH_PIC}")

    
endif()



# Install headers.
set(HEADER_PATH "include")
set(HEADER_PATH1 "include/nlohmann")
file(GLOB HEADERS "${SOURCE_DIR}/*.h*" "${TLS_SUB_DIR}/*.h" "${CXX_UTILS_DIR}/*.h" "${REDIS_PLUS_PLUS_ASYNC_FUTURE_HEADER}/*.h")
if(NOT REDIS_PLUS_PLUS_BUILD_ASYNC)
    file(GLOB ASYNC_HEADERS "${HEADER_PATH}/async_*.h" "${HEADER_PATH}/event_*.h")
    list(REMOVE_ITEM HEADERS ${ASYNC_HEADERS})
endif()

if(NOT REDIS_PLUS_PLUS_BUILD_CORO)
    file(GLOB CORO_HEADERS "${HEADER_PATH}/co_*.h")
    list(REMOVE_ITEM HEADERS ${CORO_HEADERS})
endif()

install(FILES ${HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${HEADER_PATH})
install(FILES ${HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${HEADER_PATH1})

include(CMakePackageConfigHelpers)

write_basic_package_version_file("${CMAKE_CURRENT_BINARY_DIR}/cmake/redis++-config-version.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY AnyNewerVersion)

#configure_package_config_file("${CMAKE_CURRENT_SOURCE_DIR}/cmake/redis++-config.cmake.in"
        #"${CMAKE_CURRENT_BINARY_DIR}/cmake/redis++-config.cmake"
        #INSTALL_DESTINATION ${REDIS_PLUS_PLUS_CMAKE_DESTINATION})

#install(FILES "${CMAKE_CURRENT_BINARY_DIR}/cmake/redis++-config.cmake"
       # "${CMAKE_CURRENT_BINARY_DIR}/cmake/redis++-config-version.cmake"
      #  DESTINATION ${REDIS_PLUS_PLUS_CMAKE_DESTINATION})

#export(EXPORT redis++-targets
     #   FILE "${CMAKE_CURRENT_BINARY_DIR}/cmake/redis++-targets.cmake"
     #   NAMESPACE redis++::)

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/cmake/redis++.pc.in"
        "${CMAKE_CURRENT_BINARY_DIR}/cmake/redis++.pc" @ONLY)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/cmake/redis++.pc"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig")

# All the Debian-specific cpack defines.
if(${CMAKE_VERSION} VERSION_GREATER 3.6)
  SET(CPACK_DEBIAN_PACKAGE_GENERATE_SHLIBS "ON")
endif()
if(NOT DEFINED CPACK_DEBIAN_PACKAGE_DEPENDS)
  SET(CPACK_DEBIAN_PACKAGE_DEPENDS "libstdc++6, libhiredis-dev")
endif()
SET(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)
SET(CPACK_DEBIAN_PACKAGE_VERSION "${REDIS_PLUS_PLUS_VERSION}")
SET(CPACK_DEBIAN_PACKAGE_SOURCE "https://github.com/sewenew/redis-plus-plus")
message(STATUS "Debian package name: ${CPACK_PACKAGE_FILE_NAME}.deb")

# All the common cpack defines.
if(NOT DEFINED CPACK_PACKAGE_NAME)
    SET(CPACK_PACKAGE_NAME "libredis++-dev")
endif()
SET(CPACK_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")
SET(CPACK_PACKAGE_DESCRIPTION "A pure C++ client for Redis, based on hiredis.")
SET(CPACK_PACKAGE_CONTACT "anonymous")
SET(CPACK_GENERATOR "DEB")
INCLUDE(CPack)
