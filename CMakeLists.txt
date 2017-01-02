cmake_minimum_required(VERSION 2.8.11)
project(procgui)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(ECM REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})
find_package(Qt5Widgets REQUIRED)
find_package(KF5 ${KF5_MIN_VERSION} REQUIRED COMPONENTS
    CoreAddons      
    I18n            
    WidgetsAddons   
    Config
)


include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings)
include(FeatureSummary)

set(procgui_SRC
  src/main.cpp
  src/base/config.cpp
  src/base/fieldlist.cpp
  src/base/os.cpp
  src/base/utils.cpp
  src/data/processinfo.cpp
  src/data/processinfolist.cpp
  src/gui/fieldconfig.cpp
  src/gui/procgui.cpp  
  src/gui/procinfodialog.cpp
  
)
set(procgui_LIBS
    Qt5::Widgets
    KF5::CoreAddons
    KF5::ConfigCore
    KF5::I18n
 )

qt5_wrap_ui(procgui_SRC 
	
	src/ui/fields.ui
	src/ui/procgui.ui
	src/ui/procinfodialog.ui
)

add_executable(procgui ${procgui_SRC})

target_link_libraries(procgui ${procgui_LIBS})


install(TARGETS procgui DESTINATION bin)
