//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#include <core/Path/DetourPath.h>

#include <core/Entity/Entity.h>
#include <core/Path/Pathfinder.h>

#include <DetourNavMeshQuery.h>

using namespace mud; namespace toy
{
	OWaypoint::OWaypoint(Id id, Entity& parent, const vec3& position)
		: Complex(id, type<OWaypoint>())
		, m_entity(id, *this, parent, position, ZeroQuat)
	{}

	DetourPath::DetourPath(Entity& entity, Pathfinder& pathfinder, const vec3& origin, const vec3& destination)
		: m_entity(entity)
		, m_pathfinder(pathfinder)
		, m_origin(origin)
		, m_destination(destination)
	{}
	
	void DetourPath::clear()
	{
		m_currentPoly = 0;
		m_waypoints.clear();
		m_poly_path.clear();
	}

	bool DetourPath::compute()
	{
		dtNavMeshQuery& query = *m_pathfinder.m_query;
		dtQueryFilter& filter = *m_pathfinder.m_filter;

		this->clear();

		vec3 extents = { 0.0f, 2.0f, 0.0f };
		dtPolyRef start_poly;
		dtPolyRef end_poly;
		vec3 start_pos;
		vec3 end_pos;

		if(!query.findNearestPoly(&m_origin[0], &extents[0], &filter, &start_poly, &start_pos[0]))
			return false;

		if(!query.findNearestPoly(&m_destination[0], &extents[0], &filter, &end_poly, &end_pos[0]))
			return false;

		std::vector<dtPolyRef> poly_path(m_pathfinder.m_max_polys);
		int polyCount = 0;

		if(!query.findPath(start_poly, end_poly, &start_pos[0], &end_pos[0], &filter, poly_path.data(), &polyCount, m_pathfinder.m_max_polys))
			return false;

		std::vector<vec3> point_path(m_pathfinder.m_max_waypoints);
		std::vector<dtPolyRef> poly_refs(m_pathfinder.m_max_waypoints);

		int count;

		if(!query.findStraightPath(&start_pos[0], &end_pos[0], poly_path.data(), polyCount, value_ptr(point_path[0]), 0, poly_refs.data(), &count, m_pathfinder.m_max_waypoints))
			return false;

		for(int i = count - 1; i >= 0; i--)
			m_path.push_back(point_path[i]);

		for(int i = count - 1; i > 0; i--)
			m_waypoints.push_back(point_path[i]);

		for(int i = count - 1; i > 0; i--)
			m_poly_path.push_back(poly_refs[i]);

		return true;
	}
}
