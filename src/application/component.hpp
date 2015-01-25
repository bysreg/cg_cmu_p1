#pragma once

#include <string>

namespace _462
{
	//forward declaration
	class GameObject;

	class Component
	{
	public:
		GameObject* game_object;
		std::string type;

		explicit Component() {}
		virtual ~Component() {}
	};

}