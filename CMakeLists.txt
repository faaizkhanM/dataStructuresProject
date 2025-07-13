cmake_minimum_required(VERSION 3.16)

project(CabBookingSystem VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find Qt packages
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Multimedia MultimediaWidgets Sql)

# Define header files
set(HEADER_FILES
    mainwindow.h
    cabbooking.h
    profilemanager.h
    user.h
    userdatastructures.h
    cab.h
    priorityqueue.h
    signup.h
    customvector.h
    customlist.h
    customsort.h
    citymapwindow.h
    mapview.h
    citymap.h
)

# Define source files
set(SOURCE_FILES
    main.cpp
    mainwindow.cpp
    cabbooking.cpp
    profilemanager.cpp
    user.cpp
    userdatastructures.cpp
    cab.cpp
    priorityqueue.cpp
    signup.cpp
    citymapwindow.cpp
    mapview.cpp
    citymap.cpp
)

# Define UI files
set(UI_FILES
    mainwindow.ui
    cabbooking.ui
    profilemanager.ui
    signup.ui
    cab.ui
)

# Define resource files
set(RESOURCE_FILES
    resources.qrc
    images.qrc
)

# Combine all files
set(PROJECT_SOURCES
    ${HEADER_FILES}
    ${SOURCE_FILES}
    ${UI_FILES}
    ${RESOURCE_FILES}
)

qt_add_executable(CabBookingSystem
    ${PROJECT_SOURCES}
)

target_link_libraries(CabBookingSystem PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Multimedia
    Qt6::MultimediaWidgets
    Qt6::Sql
)

# macOS/iOS bundle settings
set_target_properties(CabBookingSystem PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# Install rules
include(GNUInstallDirs)
install(TARGETS CabBookingSystem
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(CabBookingSystem)
endif()
