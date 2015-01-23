/**
 * @file heightmap.cpp
 * @brief Backend for the heightmap.
 *
 * @author Kristin Siu (kasiu)
 * @author Eric Butler (edbutler)
 * @author Frank Palermo (fpalermo)
 */


#include "scene/heightmap.hpp"

static const int DETAIL = 100;

namespace _462 {

WaterSurface::WaterSurface() : current_time( 0 ) 
{
	num_Vertices = DETAIL * DETAIL;
	vertices = new Vector3[num_Vertices];
}

WaterSurface::~WaterSurface() 
{ 
	delete[] vertices;
}

real_t WaterSurface::compute_height( const Vector2& pos ) const
{
    assert( pos.x >= -1.0 && pos.x <= 1.0 );
    assert( pos.y >= -1.0 && pos.y <= 1.0 );

    real_t h = 0;

    for ( size_t i = 0; i < wave_points.size(); ++i ) {
        const WavePoint& p = wave_points[i];
        real_t r = distance( pos, p.position );
        h += p.amplitude * exp( -p.falloff * r ) * sin( p.period * r + p.timerate * current_time );
    }

    return h;
}


void WaterSurface::update( real_t dt )
{
    current_time += dt;

	real_t inc = 2.0 / DETAIL;

	for (int i = 0; i <= 2; i+=inc)
	{
		for (int j = 0; j <= 2; j+=inc)
		{
			//vertices[i*2 + j]
		}
	}

	compute_height(Vector2::Zero);
}


} /* _462 */

