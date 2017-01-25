enable_testing()

message(STATUS "Testing enabled")
set(test_SRCS   
		test/base/test01.cpp
		test/base/run_base.cpp
		test/data/cgroup01.cpp
		test/data/procinfo01.cpp
		test/data/run_data.cpp
		test/lib/testclass.cpp
		test/lib/runner.cpp
		test/lib/testconfig.cpp
)

 
add_executable(procgui_test test/test.cpp ${test_SRCS} ${procgui_SRC})

target_link_libraries(procgui_test
    ${procgui_LIBS}
    ${LIB_BLKID}    
    ${LIB_UDEV}
)


install(TARGETS procgui_test  ${INSTALL_TARGETS_DEFAULT_ARGS})
add_subdirectory(test)