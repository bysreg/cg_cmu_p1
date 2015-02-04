#include "gameobject.hpp"
#include "meshrenderer.hpp"

namespace _462
{
	void Transform::transform(const Vector3& pos, const Quaternion& ori, const Vector3& sc)
	{
		need_update_trans_mat = true;

		position = pos;
		orientation = ori;
		scale = sc;
	}

	void Transform::update()
	{
		if (need_update_trans_mat)
		{
			make_transformation_matrix(&trans_mat, position, orientation, scale);
			need_update_trans_mat = false;
		}
	}

	Matrix4 Transform::get_matrix()
	{
		return trans_mat;
	}

	Transform::Transform()
	{
		trans_mat = Matrix4::Identity;
	}

	Transform::~Transform()
	{
	}

	void GameObject::update(real_t dt)
	{
		transform.update();
	}

	void GameObject::render()
	{
		if (renderer != NULL)
		{
			renderer->render();
		}
	}

	GameObject::GameObject() : renderer(NULL)
	{
		transform.game_object = this;
	}

	GameObject::~GameObject()
	{
		for (size_t i = 0; i < components.size(); i++)
		{
			delete components[i];
		}
	}
}

