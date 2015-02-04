include_directories(
	${PROJECT_SOURCE_DIR}
	${SDL_INCLUDE_DIR}
)
add_executable(p1 main.cpp project.cpp)

target_link_libraries(p1 application math scene memory ${SDL_LIBRARY} ${PNG_LIBRARIES}
	${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${GLEW_LIBRARIES})

if(APPLE)
	target_link_libraries(p1 SDLMain)
endif()

if(WIN32)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /FI\"${PROJECT_SOURCE_DIR}/memory/memoryoperator.h\"")
endif()

if(CMAKE_COMPILER_IS_GNUCXX)
	#add_definitions(-include \"${PROJECT_SOURCE_DIR}/memory/memoryoperator.h\");
	#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -include \"${PROJECT_SOURCE_DIR}/memory/memoryoperator.h\"")    
endif()

install(TARGETS p1 DESTINATION ${PROJECT_SOURCE_DIR}/..)
