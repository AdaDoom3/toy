//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is licensed  under the terms of the GNU General Public License v3.0.
//  See the attached LICENSE.txt file or https://www.gnu.org/licenses/gpl-3.0.en.html.
//  This notice and the license may not be removed or altered from any source distribution.

#define TOY_PRIVATE
#include <core/Bullet/BulletMotionState.h>

#include <core/Entity/Entity.h>
#include <core/Bullet.h>

using namespace mud; namespace toy
{
    BulletMotionState::BulletMotionState(MotionState& motion_state)
        : btMotionState()
		, m_motion_state(motion_state)
    {}

	void BulletMotionState::getWorldTransform(btTransform& transform) const
	{
		MotionState::Transform t = m_motion_state.transform();
		transform.setOrigin(to_btvec3(t.m_position));
		transform.setRotation(to_btquat(t.m_rotation));
	}

    void BulletMotionState::setWorldTransform(const btTransform& transform)
    {
		m_motion_state.sync_transform(to_vec3(transform.getOrigin()), to_quat(transform.getRotation()));
    }
}
