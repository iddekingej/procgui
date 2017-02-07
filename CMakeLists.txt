cmake_minimum_required(VERSION 2.8.12)
project(procgui)

set(CMAKE_AUTOMOC ON)

find_package(ECM REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})


find_package(Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS 
    Core    
)

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
  src/base/config.cpp
  src/base/fieldlist.cpp
  src/base/os.cpp
  src/base/utils.cpp
  src/data/processinfo.cpp
  src/data/processinfolist.cpp
  src/gui/fieldconfig.cpp
  src/gui/hyrfiller.cpp
  src/gui/procgui.cpp  
  src/gui/procinfodialog.cpp
  src/gui/sortproxy.cpp
  
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
add_subdirectory(src/po)

add_executable(procgui   src/main.cpp ${procgui_SRC})

target_link_libraries(procgui ${procgui_LIBS})

install(TARGETS procgui DESTINATION bin)

set(RUN_TEST ${run_test})
if(RUN_TEST EQUAL "1")
   include("test.cmake")
endif()