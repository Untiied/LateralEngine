#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>

struct TransformReturn {
	glm::vec3 scale = glm::vec3();
	glm::quat rotation = glm::quat();
	glm::vec3 translation = glm::vec3();
	glm::vec3 skew = glm::vec3();
	glm::vec4 perspective = glm::vec4();
};

class Transform
{
public:
	Transform();
	~Transform();
public:
	void Update();

	void addLocation(glm::vec3 Location);
	void addLocation(float x, float y, float z);
	void setLocation(glm::vec3 Location);

	void addRotation(float angle, glm::vec3 Rotation);
	void setRotation(float angle, glm::vec3 Rotation);

	void setScale(glm::vec3 Scale);

	void setLocationAndRotation(glm::vec3 Location, float angle, glm::vec3 Rotation);

	void setColor(glm::vec3 Color);
	void addColor(glm::vec3 Color);
	inline glm::vec3 getColor() {
		return Color;
	}

	inline glm::vec3 getLocation() { return getTransformInfo(Position).translation; }
	inline glm::vec3 getScale() { return getTransformInfo(Scale).scale; }
	inline glm::quat getRotationQuat() { return getTransformInfo(Rotation).rotation; }
	inline glm::vec3 getRotationEuler() { return glm::eulerAngles(getTransformInfo(Rotation).rotation); }

	inline TransformReturn getTransformInfo(glm::mat4 input) { TransformReturn returnTrans; 
												glm::decompose(input, returnTrans.scale, returnTrans.rotation, returnTrans.translation, returnTrans.skew, returnTrans.perspective); 
												returnTrans.rotation = glm::conjugate(returnTrans.rotation);
												return returnTrans; };
	inline glm::mat4 GetLocalModel() {
		return  Position * Rotation * Scale;
	}
	
	//inline glm::mat4 GetWorldModel() {
	//	if ((unsigned long long)parent != 0xcccccccccccccccc) {
	//		auto test = parent->GetLocalModel() * GetLocalModel();
	//		return parent->GetLocalModel() * GetLocalModel();
	//	}
	//	return GetLocalModel();

	//}
	inline glm::mat4 GetWorldModel(Transform* tform) {
		return tform->GetLocalModel() * GetLocalModel();
	}

	inline void SetParent(Transform* funcParent) {
		parent = funcParent;
		funcParent->child = this;
	}
private:
	glm::mat4 Rotation = glm::mat4(1.0f);
	glm::mat4 Position = glm::mat4(1.0f);
	glm::mat4 Scale = glm::mat4(1.0f);
	glm::vec3 Color = glm::vec3(1.0f);
	Transform* parent;
	Transform* child;
};

