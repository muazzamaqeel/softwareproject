cmake_minimum_required(VERSION 3.5)
project(Staging_Environment_Recovered VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Sql Test)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql Test)

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    registration_window.h registration_window.cpp registration_window.ui
    settings.h settings.cpp settings.ui
    backlog.h backlog.cpp backlog.ui
    taskboard.h taskboard.cpp taskboard.ui
    calendar.h calendar.cpp calendar.ui
    sprints.h sprints.cpp sprints.ui
    confluence.h confluence.cpp confluence.ui
    parentboard.ui parentboard.cpp parentboard.h
    projectcreation.h projectcreation.cpp projectcreation.ui
    backlog.cpp backlog.h backlog.ui issuecreation.h issuecreation.cpp issuecreation.ui
    databasemanager.h databasemanager.cpp
    qtunittest.h qtunittest.cpp
    adminstrator.h adminstrator.cpp adminstrator.ui
    launchscreen.h launchscreen.cpp launchscreen.ui
    projectsadmin.h projectsadmin.cpp projectsadmin.ui
    pb_confluence_implemenation.h pb_confluence_implemenation.cpp
    pb_productbacklog_implementation.h pb_productbacklog_implementation.cpp
    pb_sprint_implemenation.h pb_sprint_implemenation.cpp
    pb_taskboard_implemenation.h pb_taskboard_implemenation.cpp


)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Staging_Environment_Recovered
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        pb_team_implementation.cpp
        pb_team_implemenation.h

        teammember_projectswindow.h teammember_projectswindow.cpp teammember_projectswindow.ui
        assets/resources.qrc
        TableAnimator.h TableAnimator.cpp
        databasefetcher.h databasefetcher.cpp

    )
elseif(ANDROID)
    add_library(Staging_Environment_Recovered SHARED
        ${PROJECT_SOURCES}
    )
else()
    add_executable(Staging_Environment_Recovered
        ${PROJECT_SOURCES}
    )
endif()

target_link_libraries(Staging_Environment_Recovered PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Sql
    Qt${QT_VERSION_MAJOR}::Test
)

if(WIN32)
    target_link_libraries(Staging_Environment_Recovered PRIVATE
        ws2_32
        crypt32
        bcrypt
    )
endif()

if(APPLE)
    set_target_properties(Staging_Environment_Recovered PROPERTIES
        MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Staging_Environment_Recovered
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
    )
endif()

include(GNUInstallDirs)
install(TARGETS Staging_Environment_Recovered
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Staging_Environment_Recovered)
endif()
