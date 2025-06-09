cmake_minimum_required(VERSION 3.10)
project(PersonalFinanceManager)

set(CMAKE_CXX_STANDARD 17)

# Указываем путь к vcpkg (можно заменить на абсолютный путь, если нужно)
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/../vcpkg/scripts/buildsystems/vcpkg.cmake"
        CACHE STRING "Vcpkg toolchain file")
endif()

# Добавляем исходники
add_executable(MyApp
    main.cpp
    Account.cpp
    Wallet.cpp
    BankCard.cpp
    DebitCard.cpp
    CreditCard.cpp
    Category.cpp
    Transaction.cpp
    Income.cpp
    Expence.cpp
)

# Подключаем Boost (если ты его используешь)
find_package(Boost REQUIRED CONFIG)
target_link_libraries(MyApp PRIVATE Boost::boost)
