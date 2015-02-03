include_directories(
	${PROJECT_SOURCE_DIR}
	${SDL_INCLUDE_DIR}
	${GLEW_INCLUDE_DIRS}
    ${PNG_INCLUDE_DIRS}
)

add_library(application application.cpp camera_roam.cpp imageio.cpp gameobject.cpp component.cpp meshrenderer.cpp log.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /FI\"${PROJECT_SOURCE_DIR}/memory/memoryoperator.h\"")