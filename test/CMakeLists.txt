set(test_name bzip2stream-test)
set(test_app bzip2stream-test)

add_library(bzip2stream::bzip2stream ALIAS bzip2stream)

hunter_add_package(GTest)
find_package(GTest CONFIG REQUIRED)

hunter_add_package(gauze)
find_package(gauze CONFIG REQUIRED)

hunter_add_package(Boost COMPONENTS system)
find_package(Boost CONFIG REQUIRED system)

add_executable(${test_app} bzip2_stream_test.cpp)
target_link_libraries(${test_app} PUBLIC bzip2stream::bzip2stream GTest::main Boost::system)

gauze_add_test( # links gauze::gauze
  NAME ${test_name}
  COMMAND ${test_app}
  )
