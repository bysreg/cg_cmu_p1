#include "meshrenderer.hpp"
#include "gameobject.hpp"

namespace _462
{
	void MeshRenderer::initialize()
	{
		vertices_size = sizeof(vertices[0]) * num_vertices;
		triangles_size = sizeof(triangles[0]) * num_triangles;
		normals_size = sizeof(normals[0]) * num_normals;

		// Create buffers for mesh
		glGenBuffers(1, &vertices_id);
		glGenBuffers(1, &triangles_id);
		glGenBuffers(1, &normals_id);
		// Bind & Load buffers to modify/render them
		glBindBuffer(GL_ARRAY_BUFFER, vertices_id);
		glBufferData(GL_ARRAY_BUFFER, vertices_size, vertices, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangles_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, triangles_size, triangles, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ARRAY_BUFFER, normals_id);
		glBufferData(GL_ARRAY_BUFFER, normals_size, normals, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		is_initialized = true;
	}

	void MeshRenderer::render()
	{
		if (!is_initialized)
			initialize();

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);

		//set material
		glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_color);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_color);
		glMaterialfv(GL_FRONT, GL_SPECULAR, specular_color);
		glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

		glPushMatrix();
		glMultMatrixd(game_object->transform.get_matrix().m);

		if (is_dirty)
		{
			glBindBuffer(GL_ARRAY_BUFFER, vertices_id);
			glBufferSubData(GL_ARRAY_BUFFER, 0, vertices_size, vertices);
			glVertexPointer(3, GL_DOUBLE, offsetof(Vector3, x), 0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glBindBuffer(GL_ARRAY_BUFFER, normals_id);
			glBufferSubData(GL_ARRAY_BUFFER, 0, normals_size, normals);
			glNormalPointer(GL_DOUBLE, 0, 0);

			is_dirty = false;
		}
		else
		{
			glBindBuffer(GL_ARRAY_BUFFER, vertices_id);
			glVertexPointer(3, GL_DOUBLE, offsetof(Vector3, x), 0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glBindBuffer(GL_ARRAY_BUFFER, normals_id);
			glNormalPointer(GL_DOUBLE, 0, 0);
		}

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangles_id);

		glDrawElements(GL_TRIANGLES, 3 * num_triangles, GL_UNSIGNED_INT, 0);
		glPopMatrix();

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
	}

	MeshRenderer::MeshRenderer() : is_initialized(false), is_dirty(false)
	{
		type = "MeshRenderer";
	}

	MeshRenderer::~MeshRenderer()
	{
		delete[] vertices;
		delete[] triangles;
		delete[] normals;
	}

	void MeshRenderer::set_dirty()
	{
		is_dirty = true;
	}
}