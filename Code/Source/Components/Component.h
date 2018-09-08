#pragma once

namespace LateralEngine {
	class Component
	{
	public:
		Component();
		virtual ~Component();
	public:
		virtual void Start();
		virtual void Update();
		class GameObject* gameObject;
	};
}
