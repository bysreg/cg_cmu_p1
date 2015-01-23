/**
 * @file project.cpp
 * @brief OpenGL project
 *
 * @author H. Q. Bovik (hqbovik)
 * @bug Unimplemented
 */

#include "p1/project.hpp"
#include <stddef.h> 

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
GLuint meshVertId, meshIdxId, meshNormalId;
GLuint heightmapVertId, heightmapIdxId;
GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 }; // directional light
Matrix4 meshWorldMat;
Matrix4 heightmapWorldMat;
//---

/**
 * Initialize the project, doing any necessary opengl initialization.
 * @param camera An already-initialized camera.
 * @param scene The scene to render.
 * @return true on success, false on error.
 */
bool OpenglProject::initialize(Camera* camera, Scene* scene, int width, int height)
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

	float k = 0.4f;
	GLfloat red[] = {1.0, 0.0, 0.0, 1.0};
	GLfloat black[] = {0.0, 0.0, 0.0, 1.0};
	GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat blue[] = { 0.0, 1.0, 0.0, 1.0 };
	GLfloat whitek[] = {k, k, k, 1.0};
	GLfloat mat_shininess[] = { 5 };
	meshWorldMat = Matrix4::Identity;
	make_transformation_matrix(&meshWorldMat, scene->mesh_position.position, scene->mesh_position.orientation, scene->mesh_position.scale);

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(camera->get_fov_degrees(), camera->get_aspect_ratio(), camera->get_near_clip(), camera->get_far_clip());
	glMatrixMode(GL_MODELVIEW);

	// Create buffers for mesh
	glGenBuffers(1, &meshVertId);
	glGenBuffers(1, &meshIdxId);
	glGenBuffers(1, &meshNormalId);
	// Bind & Load buffers to modify/render them
	glBindBuffer(GL_ARRAY_BUFFER, meshVertId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(this->scene.mesh.vertices[0]) * this->scene.mesh.num_vertices, this->scene.mesh.vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshIdxId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(this->scene.mesh.triangles[0]) * this->scene.mesh.num_triangles, this->scene.mesh.triangles, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ARRAY_BUFFER, meshNormalId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(this->scene.mesh.vertices[0]) * this->scene.mesh.num_vertices, this->scene.mesh.normals, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//create buffers for heightmap
	glGenBuffers(1, &heightmapVertId);
	glGenBuffers(1, &heightmapIdxId);

	//setting lights
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glMaterialfv(GL_FRONT, GL_AMBIENT, red);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, red);
	glMaterialfv(GL_FRONT, GL_SPECULAR, whitek);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, whitek);
	glLightfv(GL_LIGHT0, GL_SPECULAR, whitek);

	glEnable(GL_DEPTH_TEST);

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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	using namespace std;

	//matrix thingy
	Vector3 cameraTarget = camera->get_position() + camera->get_direction();

	//cout << camera->get_position() << " || " /*<< camera->get_direction() << " || "*/ << camera->get_up() << endl;
	//cout << cameraTarget << endl;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(camera->get_position().x, camera->get_position().y, camera->get_position().z, cameraTarget.x, cameraTarget.y, cameraTarget.z, camera->get_up().x, camera->get_up().y, camera->get_up().z);
	
	//lighting
	glPushMatrix();
		glRotatef(PI / 4, 1.0, 0.0, 0.0);
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glPopMatrix();
	//---

	glMultMatrixd(meshWorldMat.m);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	
	glBindBuffer(GL_ARRAY_BUFFER, meshVertId);
	glVertexPointer(3, GL_DOUBLE, offsetof(Vector3, x), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, meshNormalId);
	glNormalPointer(GL_DOUBLE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshIdxId);
	
	glDrawElements(GL_TRIANGLES, 3 * scene.mesh.num_triangles, GL_UNSIGNED_INT, 0);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}

} /* _462 */
