#include "Entity.hpp"

namespace Atlas {

Entity::Entity(entt::entity handle, Scene* scene) 
	: m_EntityHandle(handle), m_Scene(scene) { }

} // namespace Atlas
