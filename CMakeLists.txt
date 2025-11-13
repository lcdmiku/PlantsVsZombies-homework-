cmake_minimum_required(VERSION 3.16)

project(Game VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        main.cpp
        mainscene.cpp
        mainscene.h
        mainscene.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    find_package(Qt6 REQUIRED COMPONENTS Widgets)
    find_package(Qt6 REQUIRED COMPONENTS Core)
    find_package(Qt6 REQUIRED COMPONENTS Multimedia)
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)
find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)
find_package(Qt6 REQUIRED COMPONENTS Core)

    qt_add_executable(Game
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        res.qrc
        myobj.h myobj.cpp
        myobj.h myobj.cpp
        shop.h shop.cpp
        sunlight.h sunlight.cpp
        card.h card.cpp
        plant.h plant.cpp
        peashooter.h peashooter.cpp
        plantarea.h plantarea.cpp
        sunflower.h sunflower.cpp
        wallnut.h wallnut.cpp
        zombie.h zombie.cpp
        nomalzombie.h nomalzombie.cpp
        screenzombie.h screenzombie.cpp
        bullet.h bullet.cpp

        cherrybomb.h
        cherrybomb.cpp
        bucketzombie.h bucketzombie.cpp
        conezombie.h conezombie.cpp
        footballzombie.h footballzombie.cpp
        potatomine.h potatomine.cpp
        shovel.h shovel.cpp
        mower.h mower.cpp
        settingsmenu.h settingsmenu.cpp
        snowpeashooter.h snowpeashooter.cpp
        snowpea.h snowpea.cpp
        startwidget.h startwidget.cpp
        cardscontainer.h cardscontainer.cpp
        selectplant.h selectplant.cpp
        gamescene.h gamescene.cpp
        animate.h animate.cpp
        gameleveldata.h gameleveldata.cpp
        dominator.h dominator.cpp
        coordinate.h coordinate.cpp
        dialogbox.h dialogbox.cpp


    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET Game APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(Game SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(Game
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(Game PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(Game PRIVATE Qt6::Widgets)
target_link_libraries(Game PRIVATE Qt6::Core)
target_link_libraries(Game PRIVATE Qt6::Multimedia)
target_link_libraries(Game PRIVATE Qt6::Core Qt6::Widgets)
target_link_libraries(Game PRIVATE Qt6::Widgets)
target_link_libraries(Game PRIVATE Qt6::Core)
target_link_libraries(Game PRIVATE Qt6::Core)
target_link_libraries(Game PRIVATE Qt6::Core Qt6::Widgets)
target_link_libraries(Game PRIVATE Qt6::Core)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Game)
endif()
set_target_properties(Game PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS Game
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Game)
endif()
