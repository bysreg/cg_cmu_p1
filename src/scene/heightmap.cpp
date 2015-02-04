/**
 * @file heightmap.cpp
 * @brief Backend for the heightmap.
 *
 * @author Kristin Siu (kasiu)
 * @author Eric Butler (edbutler)
 * @author Frank Palermo (fpalermo)
 */


#include "scene/heightmap.hpp"

static const int DETAIL = 160;

namespace _462 {

	WaterSurface::WaterSurface() : current_time(0)
	{
		num_vertices = DETAIL * DETAIL;
		vertices = new Vector3[num_vertices];
		vertices_size = sizeof(vertices[0]) * num_vertices;
		int min = -1;
		int max = 1;
		real_t inc = (max - min) / ((real_t)DETAIL);

		for (int i = 0; i < DETAIL; i++)
		{
			for (int j = 0; j < DETAIL; j++)
			{
				real_t x, y;
				int index = i*DETAIL + j;
				if (i == DETAIL - 1)
				{
					y = max;
				}
				else
				{
					y = (i * inc) + min;
				}

				if (j == DETAIL - 1)
				{
					x = max;
				}
				else
				{
					x = (j * inc) + min;
				}
				vertices[index].x = x;
				vertices[index].y = y;
				vertices[index].z = compute_height(Vector2(x, y));
			}
		}

		//setup indices
		size_t num_Square = (DETAIL - 1) * (DETAIL - 1);
		int num_SquareCol = DETAIL - 1;
		int index = 0;

		num_triangles = num_Square * 2;
		triangles = new Triangle[num_triangles];

		//walk thru every squares
		for (int i = 0; i < DETAIL - 1; i++)
		{
			for (int j = 0; j < DETAIL - 1; j++)
			{
				typedef unsigned int uint;
				uint left_bottom = i * (num_SquareCol + 1) + j;
				uint left_top = (i + 1) * (num_SquareCol + 1) + j;
				uint right_bottom = left_bottom + 1;
				uint right_top = left_top + 1;

				triangles[index].vertices[0] = left_bottom;
				triangles[index].vertices[1] = right_bottom;
				triangles[index++].vertices[2] = left_top;

				triangles[index].vertices[0] = right_bottom;
				triangles[index].vertices[1] = right_top;
				triangles[index++].vertices[2] = left_top;
			}
		}

		//setup normals
		normals = new Vector3[num_vertices];
	}

	WaterSurface::~WaterSurface()
	{
		// dont delete vertices, normals, and triangles here. because those will be deleted inside the mesh renderer component
	}

	real_t WaterSurface::compute_height(const Vector2& pos) const
	{
		assert(pos.x >= -1.0 && pos.x <= 1.0);
		assert(pos.y >= -1.0 && pos.y <= 1.0);

		real_t h = 0;

		for (size_t i = 0; i < wave_points.size(); ++i) {
			const WavePoint& p = wave_points[i];
			real_t r = distance(pos, p.position);
			h += p.amplitude * exp(-p.falloff * r) * sin(p.period * r + p.timerate * current_time);
		}

		return h;
	}


	void WaterSurface::update(real_t dt)
	{
		current_time += dt;

		for (size_t i = 0; i < num_vertices; i++)
		{
			vertices[i].z = compute_height(Vector2(vertices[i].x, vertices[i].y));
		}
	}

} /* _462 */

