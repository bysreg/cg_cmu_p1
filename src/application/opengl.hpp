/**
 * @file opengl.hpp
 * @brief Includes the correct opengl header files depending on the
 *  platform. Use this file to include any gl header files.
 */

#ifndef _462_OPENGL_HPP_
#define _462_OPENGL_HPP_

#include "glew/GL/glew.h"
#include "math/matrix.hpp"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#endif

#define NO_SDL_GLEXT
#include <SDL_opengl.h>

#endif /* _462_OPENGL_HPP_ */

