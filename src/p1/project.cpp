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
GLuint heightmapVertId, heightmapIdxId, heightmapNormalId;
GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 }; // directional light
Matrix4 meshWorldMat;
Matrix4 heightmapWorldMat;

GLfloat red[] = { 1.0, 0.0, 0.0, 1.0 };
GLfloat black[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat blue[] = { 0.0, 0.0, 1.0, 1.0 };
GLfloat mesh_shininess[] = { 40 };
GLfloat water_shininess[] = {50};
//---

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

	glClearColor(0.0f, 1.0f, 0.0f, 0.0f);

	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		//glewInit failed
		std::cerr << "glewInit failed, something is seriously wrong." << std::endl;
		return false;
	}

	meshWorldMat = Matrix4::Identity;
	make_transformation_matrix(&meshWorldMat, this->scene.mesh_position.position, this->scene.mesh_position.orientation, this->scene.mesh_position.scale);
	heightmapWorldMat = Matrix4::Identity;
	this->scene.heightmap_position.orientation = scene->heightmap_position.orientation * Quaternion(Vector3::UnitX, -PI / 2);
	this->scene.heightmap_position.scale = Vector3(1.5, 1.5, 1.5);
	make_transformation_matrix(&heightmapWorldMat, this->scene.heightmap_position.position, this->scene.heightmap_position.orientation, this->scene.heightmap_position.scale);

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(camera->get_fov_degrees(), camera->get_aspect_ratio(), camera->get_near_clip(), camera->get_far_clip());
	glMatrixMode(GL_MODELVIEW);

	compute_normals(this->scene.mesh.normals, this->scene.mesh.vertices, this->scene.mesh.num_vertices, this->scene.mesh.triangles, this->scene.mesh.num_triangles);
	compute_normals(this->scene.heightmap->normals, this->scene.heightmap->vertices, this->scene.heightmap->num_Vertices, this->scene.heightmap->triangles, this->scene.heightmap->num_triangles);

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
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, meshNormalId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(this->scene.mesh.vertices[0]) * this->scene.mesh.num_vertices, this->scene.mesh.normals, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//create buffers for heightmap
	glGenBuffers(1, &heightmapVertId);
	glGenBuffers(1, &heightmapIdxId);
	glGenBuffers(1, &heightmapNormalId);
	
	glBindBuffer(GL_ARRAY_BUFFER, heightmapVertId);
	glBufferData(GL_ARRAY_BUFFER, this->scene.heightmap->vertices_size, this->scene.heightmap->vertices, GL_STREAM_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, heightmapIdxId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(this->scene.heightmap->triangles[0]) * this->scene.heightmap->num_triangles, this->scene.heightmap->triangles, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, heightmapNormalId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(this->scene.heightmap->normals[0]) * this->scene.heightmap->num_Vertices, this->scene.heightmap->normals, GL_STREAM_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//setting lights
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
	glLightfv(GL_LIGHT0, GL_SPECULAR, white);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_RESCALE_NORMAL);

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
	compute_normals(scene.heightmap->normals, scene.heightmap->vertices, scene.heightmap->num_Vertices, scene.heightmap->triangles, scene.heightmap->num_triangles);
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
		glRotated(PI / 4, 1.0, 0.0, 0.0);
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glPopMatrix();
	//---

	//drawing mesh
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	
	//set material
	glMaterialfv(GL_FRONT, GL_AMBIENT, red);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, red);
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glMaterialfv(GL_FRONT, GL_SHININESS, mesh_shininess);

	glPushMatrix();
		glMultMatrixd(meshWorldMat.m);
	
		glBindBuffer(GL_ARRAY_BUFFER, meshVertId);
		glVertexPointer(3, GL_DOUBLE, offsetof(Vector3, x), 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ARRAY_BUFFER, meshNormalId);
		glNormalPointer(GL_DOUBLE, 0, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshIdxId);
	
		glDrawElements(GL_TRIANGLES, 3 * scene.mesh.num_triangles, GL_UNSIGNED_INT, 0);
	glPopMatrix();

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	//-------------

	//draw height map
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	//set material
	glMaterialfv(GL_FRONT, GL_AMBIENT, blue);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, blue);
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glMaterialfv(GL_FRONT, GL_SHININESS, water_shininess);

	glPushMatrix();
	glMultMatrixd(heightmapWorldMat.m);

	glBindBuffer(GL_ARRAY_BUFFER, heightmapVertId);
	glBufferSubData(GL_ARRAY_BUFFER, 0, scene.heightmap->vertices_size, scene.heightmap->vertices);
	glVertexPointer(3, GL_DOUBLE, offsetof(Vector3, x), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, heightmapNormalId);
	glBufferSubData(GL_ARRAY_BUFFER, 0, scene.heightmap->vertices_size, scene.heightmap->normals);
	glNormalPointer(GL_DOUBLE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, heightmapIdxId);

	glDrawElements(GL_TRIANGLES, 3 * scene.heightmap->num_triangles, GL_UNSIGNED_INT, 0);
	glPopMatrix();

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}

} /* _462 */
