# CMakeList.txt: проект CMake для Maz_Protei_locator; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
cmake_minimum_required (VERSION 3.8)

# Включение горячей перезагрузки для компиляторов MSVC, если поддерживается.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("Maz_Protei_locator")

# Добавьте источник в исполняемый файл этого проекта.
include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)

include(FetchContent)
FetchContent_Declare(spdlog URL https://github.com/gabime/spdlog/archive/v1.8.1.tar.gz)
FetchContent_MakeAvailable(spdlog)

include(FetchContent)
FetchContent_Declare(
googletest
URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable (Maz_Protei_locator "Maz_Protei_locator.cpp" "Maz_Protei_locator.h" "model/Subscriber.h" "model/Subscriber.cpp" "model/Zone.h" "model/Zone.cpp" "model/ZoneTrigger.h" "model/ZoneTrigger.cpp" "model/SubscribersProximityTrigger.h" "model/SubscribersProximityTrigger.cpp" "locator_service/Locator.h" "locator_service/Locator.cpp")
target_link_libraries(Maz_Protei_locator PRIVATE nlohmann_json::nlohmann_json spdlog::spdlog gtest_main)
set_target_properties(Maz_Protei_locator PROPERTIES OUTPUT_NAME "locator")

add_executable (Maz_Protei_locator_test "Maz_Protei_locator_test.cpp" "Maz_Protei_locator.h" "model/Subscriber.h" "model/Subscriber.cpp" "model/Zone.h" "model/Zone.cpp" "model/ZoneTrigger.h" "model/ZoneTrigger.cpp" "model/SubscribersProximityTrigger.h" "model/SubscribersProximityTrigger.cpp" "locator_service/Locator.h" "locator_service/Locator.cpp")
target_link_libraries(Maz_Protei_locator_test PRIVATE nlohmann_json::nlohmann_json spdlog::spdlog gtest_main)
set_target_properties(Maz_Protei_locator_test PROPERTIES OUTPUT_NAME "locatortests")

  add_custom_target(copy_config
      file(COPY ${CMAKE_SOURCE_DIR}/"config.json" DESTINATION ${CMAKE_BINARY_DIR})
  )

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Maz_Protei_locator PROPERTY CXX_STANDARD 20)
endif()

