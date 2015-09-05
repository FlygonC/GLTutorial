#include "Camera.h"


void Camera::setPerspective(float fov, float aspectRatio, float pnear, float pfar)
{
	float s = 1 / (pnear*tan(fov / 2));
	projectionTransform[0][0] = aspectRatio * s;
	projectionTransform[1][1] = s;
	projectionTransform[2][2] = -((pfar + pnear) / (pfar - pnear));
	projectionTransform[3][2] = -((2 * pfar*pnear) / (pfar - pnear));
	projectionTransform[2][3] = -1;
}
void Camera::setLookAt(vec3 from, vec3 to, vec3 up)
{
	worldTransform = glm::lookAt(from, to, up);
}
void Camera::setPosition(vec3 position)
{
	worldTransform *= glm::translate(position);
}

mat4 Camera::getWorldTransform() { return worldTransform; }
mat4 Camera::getView() { return glm::inverse(worldTransform); }
mat4 Camera::getProjection() { return projectionTransform; }
mat4 Camera::getProjectionView() { return projectionTransform * getView(); }
