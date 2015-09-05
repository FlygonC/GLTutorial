#pragma once
#include <glm\glm.hpp>
#include <glm\ext.hpp>

using glm::vec3;
using glm::vec4;
using glm::mat4;

class Camera
{
protected:
	mat4 worldTransform = mat4(1);
	//mat4 viewTransform = mat4(0);
	mat4 projectionTransform = mat4(0);
	//mat4 projectionViewTransform = mat4(0);
public:
	virtual void update(float dt) = 0;
	void setPerspective(float fov, float aspectRatio, float pnear, float pfar);
	void setLookAt(vec3 from, vec3 to, vec3 up);
	void setPosition(vec3 position);
	mat4 getWorldTransform();
	mat4 getView();
	mat4 getProjection();
	mat4 getProjectionView();
	//void updateProjectionViewTransform();
};