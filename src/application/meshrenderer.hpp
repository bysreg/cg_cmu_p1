#pragma once

#include "math\vector.hpp"
#include "component.hpp"
#include <GL\glew.h>

namespace _462
{

	class MeshRenderer : public Component
	{
	public:
		Vector3* vertices;
		size_t num_vertices;
		size_t vertices_size; // in bytes
		
		Triangle* triangles;
		size_t num_triangles;
		size_t triangles_size; // in bytes

		Vector3* normals;
		size_t num_normals;
		size_t normals_size;

		//materials
		GLfloat ambient_color[4];
		GLfloat diffuse_color[4];
		GLfloat specular_color[4];
		GLfloat shininess[1];

		void initialize();
		void render();

		explicit MeshRenderer();
		virtual ~MeshRenderer();
		void set_dirty(); // call this if either vertices or normals has been changed 
	private:
		GLuint vertices_id, triangles_id, normals_id;
		bool is_initialized;
		bool is_dirty;
	};

}