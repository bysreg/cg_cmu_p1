#pragma once

#include "math/vector.hpp"
#include "math/quaternion.hpp"
#include "math/matrix.hpp"
#include "component.hpp"
#include <vector>

namespace _462
{
	//forward declarations
	class MeshRenderer;

	class Transform : public Component
	{
	public:		
		void transform(const Vector3& position, const Quaternion& orientation, const Vector3& scale);
		void update();

		explicit Transform();
		virtual ~Transform();
		Matrix4 get_matrix();

	private:
		Vector3 position;
		Quaternion orientation;
		Vector3 scale;

		Matrix4 trans_mat;
		bool need_update_trans_mat;

	};

	class GameObject
	{
	public:
		Transform transform;
		MeshRenderer* renderer;

		template<class T> Component* add_component()
		{
			T* component = new T;
			component->game_object = this;
			components.push_back(component);

			if (component->type == "MeshRenderer")
			{
				renderer = component;
			}

			return component;
		}

		void update(real_t dt);
		void render();

		explicit GameObject();
		virtual ~GameObject();

	private:
		std::vector<Component*> components;
	};

}