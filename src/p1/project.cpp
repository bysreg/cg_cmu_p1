/**
 * @file project.cpp
 * @brief OpenGL project
 *
 * @author H. Q. Bovik (hqbovik)
 * @bug Unimplemented
 */

#include "p1/project.hpp"

// use this header to include the OpenGL headers
// DO NOT include gl.h or glu.h directly; it will not compile correctly.
#include "application/opengl.hpp"

// A namespace declaration. All proejct files use this namespace.
// Add this declration (and its closing) to all source/headers you create.
// Note that all #includes should be BEFORE the namespace declaration.
namespace _462 {

// definitions of functions for the OpenglProject class

// constructor, invoked when object is created
OpenglProject::OpenglProject()
{
    // TODO any basic construction or initialization of members
    // Warning: Although members' constructors are automatically called,
    // ints, floats, pointers, and classes with empty contructors all
    // will have uninitialized data!
}

// destructor, invoked when object is destroyed
OpenglProject::~OpenglProject()
{
    // TODO any final cleanup of members
    // Warning: Do not throw exceptions or call virtual functions from deconstructors!
    // They will cause undefined behavior (probably a crash, but perhaps worse).
}

//data for rendering
GLuint vertId, idxId;
//data test
GLfloat vertices[] = { 0.0, -1.0, 0.0, 1.0, 1.0, 1.0 };
GLuint indices[] = { 0, 1, 2 };
GLfloat colors[] = { 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f };
//
//---

/**
 * Initialize the project, doing any necessary opengl initialization.
 * @param camera An already-initialized camera.
 * @param scene The scene to render.
 * @return true on success, false on error.
 */
bool OpenglProject::initialize( Camera* camera, Scene* scene )
{
    // copy scene
    this->scene = *scene;

	glClearColor(0.0f, 1.0f, 0.0f, 0.0f);

	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		//glewInit failed
		std::cerr << "glewInit failed, something is seriously wrong." << std::endl;
		return false;
	}

	// Create buffers
	glGenBuffers(1, &vertId);
	glGenBuffers(1, &idxId);
	// Bind buffers to modify/render them
	glBindBuffer(GL_ARRAY_BUFFER, vertId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idxId);
	// Load data into buffers
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(colors), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(colors), colors);

    return true;
}

/**
 * Clean up the project. Free any memory, etc.
 */
void OpenglProject::destroy()
{
    // TODO any cleanup code, e.g., freeing memory
}

/**
 * Perform an update step. This happens on a regular interval.
 * @param dt The time difference from the previous frame to the current.
 */
void OpenglProject::update( real_t dt )
{
    // update our heightmap
    scene.heightmap->update( dt );

    // TODO any update code, e.g. commputing heightmap mesh positions and normals
}

/**
 * Clear the screen, then render the mesh using the given camera.
 * @param camera The logical camera to use.
 * @see math/camera.hpp
 */
void OpenglProject::render( const Camera* camera )
{
	glClear(GL_COLOR_BUFFER_BIT);

	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	
	glBindBuffer(GL_ARRAY_BUFFER, vertId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idxId);

	glVertexPointer(2, GL_FLOAT, 0, 0);
	glColorPointer(3, GL_FLOAT, 0, ((GLubyte*)NULL + sizeof(vertices)));
	
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}

} /* _462 */
