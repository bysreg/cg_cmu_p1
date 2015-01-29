/**
 * @file project.cpp
 * @brief OpenGL project
 *
 * @author H. Q. Bovik (hqbovik)
 * @bug Unimplemented
 */

#include "p1/project.hpp"
#include <stddef.h>
#include <algorithm>
#include <cstring>
#include "application/gameobject.hpp"
#include "application/meshrenderer.hpp"

// use this header to include the OpenGL headers
// DO NOT include gl.h or glu.h directly; it will not compile correctly.
#include "application/opengl.hpp"

// A namespace declaration. All proejct files use this namespace.
// Add this declration (and its closing) to all source/headers you create.
// Note that all #includes should be BEFORE the namespace declaration.
namespace _462 {

	//data for rendering
	static GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 }; // directional light
	static GameObject* border;
	static GameObject* water;
	//---

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

void compute_normals(Vector3* normals, const Vector3* vertices, size_t num_vertices, const Triangle* triangles, size_t num_triangles)
{
	std::fill_n(normals, num_vertices, Vector3::Zero);
	for (size_t i = 0; i < num_triangles; i++)
	{
		unsigned int index0 = triangles[i].vertices[0];
		unsigned int index1 = triangles[i].vertices[1];
		unsigned int index2 = triangles[i].vertices[2];

		Vector3 surface_normal = normalize(cross(vertices[index1] - vertices[index0], vertices[index2] - vertices[index0]));
		normals[index0] += surface_normal;
		normals[index1] += surface_normal;
		normals[index2] += surface_normal;
	}
	//average all the surface normals
	for (size_t i = 0; i < num_vertices; i++)
	{
		normals[i] = normalize(normals[i]);
	}
}

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

	glClearColor(0.5f, 0.5f, 0.5f, 0.0f);

	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		//glewInit failed
		std::cerr << "glewInit failed, something is seriously wrong." << std::endl;
		return false;
	}

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(camera->get_fov_degrees(), camera->get_aspect_ratio(), camera->get_near_clip(), camera->get_far_clip());
	glMatrixMode(GL_MODELVIEW);

	GLfloat red[] = { 1.0, 0.0, 0.0, 1.0 };
	GLfloat black[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat blue[] = { 0.32, 0.64, 0.78, 1.0 };
	GLfloat mesh_shininess[] = { 40 };
	GLfloat water_shininess[] = { 50 };

	// Create buffers for border object
	border = new GameObject;
	border->add_component<MeshRenderer>();
	border->transform.transform(this->scene.mesh_position.position, this->scene.mesh_position.orientation, this->scene.mesh_position.scale);
	border->renderer->num_vertices = this->scene.mesh.num_vertices;
	border->renderer->num_normals = this->scene.mesh.num_vertices;
	border->renderer->num_triangles = this->scene.mesh.num_triangles;
	border->renderer->vertices = new Vector3[border->renderer->num_vertices];
	border->renderer->normals = new Vector3[border->renderer->num_normals];
	border->renderer->triangles = new Triangle[border->renderer->num_triangles];
	for (int i = 0; i < border->renderer->num_vertices; i++)
	{
		border->renderer->vertices[i] = this->scene.mesh.vertices[i];
	}
	for (int i= 0; i < border->renderer->num_triangles; i++)
	{
		border->renderer->triangles[i] = this->scene.mesh.triangles[i];
	}
	compute_normals(border->renderer->normals, border->renderer->vertices, border->renderer->num_vertices, border->renderer->triangles, border->renderer->num_triangles);
	memcpy(border->renderer->ambient_color, red, sizeof(red));
	memcpy(border->renderer->diffuse_color, red, sizeof(red));
	memcpy(border->renderer->specular_color, white, sizeof(white));
	memcpy(border->renderer->shininess, mesh_shininess, sizeof(mesh_shininess));

	water = new GameObject;
	water->add_component<MeshRenderer>();
	water->transform.transform(this->scene.heightmap_position.position, (this->scene.heightmap_position.orientation * Quaternion(Vector3::UnitX, -PI / 2)), Vector3(1.5, 1.5, 0.5));
	water->renderer->num_vertices = this->scene.heightmap->num_vertices;
	water->renderer->num_triangles = this->scene.heightmap->num_triangles;
	water->renderer->num_normals = this->scene.heightmap->num_vertices;
	water->renderer->vertices = this->scene.heightmap->vertices;
	water->renderer->triangles = this->scene.heightmap->triangles;
	water->renderer->normals = this->scene.heightmap->normals;
	compute_normals(water->renderer->normals, water->renderer->vertices, water->renderer->num_vertices, water->renderer->triangles, water->renderer->num_triangles);
	memcpy(water->renderer->ambient_color, blue, sizeof(red));
	memcpy(water->renderer->diffuse_color, blue, sizeof(red));
	memcpy(water->renderer->specular_color, white, sizeof(white));
	memcpy(water->renderer->shininess, water_shininess, sizeof(water_shininess));

	//setting lights
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
	glLightfv(GL_LIGHT0, GL_SPECULAR, white);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_NORMALIZE);

    return true;
}

/**
 * Clean up the project. Free any memory, etc.
 */
void OpenglProject::destroy()
{
	delete border;
	delete water;
}

/**
 * Perform an update step. This happens on a regular interval.
 * @param dt The time difference from the previous frame to the current.
 */
void OpenglProject::update( real_t dt )
{
	border->update(dt);
	water->update(dt);

    // update our heightmap
    scene.heightmap->update( dt );

	compute_normals(water->renderer->normals, water->renderer->vertices, water->renderer->num_vertices, water->renderer->triangles, water->renderer->num_triangles);
	water->renderer->set_dirty();
}

/**
 * Clear the screen, then render the mesh using the given camera.
 * @param camera The logical camera to use.
 * @see math/camera.hpp
 */
void OpenglProject::render( const Camera* camera )
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//matrix thingy
	Vector3 cameraTarget = camera->get_position() + camera->get_direction();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(camera->get_position().x, camera->get_position().y, camera->get_position().z, cameraTarget.x, cameraTarget.y, cameraTarget.z, camera->get_up().x, camera->get_up().y, camera->get_up().z);
	
	//lighting
	glPushMatrix();
		glRotated(PI / 4, 1.0, 0.0, 0.0);
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glPopMatrix();
	//---

	border->render();
	water->render();
}

} /* _462 */
