#include "C_Camera.h"
#include "GameObject.h"

C_Camera::C_Camera(GameObject* gameObject) : Component(Component::Type::Camera, gameObject)
{
	frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);
	frustum.SetPos(float3(10, 50, 0));
	frustum.SetFront(float3::unitZ);
	frustum.SetUp(float3::unitY);

	frustum.SetViewPlaneDistances(0.1f, 1000.0f);
	frustum.SetPerspective(1.0f, 1.0f);

	update_projection = true;
}
C_Camera::~C_Camera()
{

}

//Frustum variable management ----
float C_Camera::GetNearPlane() const
{
	return frustum.NearPlaneDistance();
}

float C_Camera::GetFarPlane() const
{
	return frustum.FarPlaneDistance();
}

//Returns FOV in degrees
float C_Camera::GetFOV() const
{
	return frustum.VerticalFov() * RADTODEG;
}

float C_Camera::GetAspectRatio() const
{
	return frustum.AspectRatio();
}

void C_Camera::SetNearPlane(float distance)
{
	if (distance > 0 && distance < frustum.FarPlaneDistance())
		frustum.SetViewPlaneDistances(distance, frustum.FarPlaneDistance());
}

void C_Camera::SetFarPlane(float distance)
{
	if (distance > 0 && distance > frustum.NearPlaneDistance())
		frustum.SetViewPlaneDistances(frustum.NearPlaneDistance(), distance);
}

//Setting vertical FOV in degrees 
void C_Camera::SetFOV(float fov)
{
	fov *= DEGTORAD;
	frustum.SetVerticalFovAndAspectRatio(fov, frustum.AspectRatio());
}

void C_Camera::SetAspectRatio(float ar)
{
	float horizontalFov = frustum.HorizontalFov();
	frustum.SetHorizontalFovAndAspectRatio(horizontalFov, ar);
	//frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) * ar);
}
//--------------------------------

void C_Camera::Look(const float3& position)
{
	float3 vector = position - frustum.Pos();

	float3x3 matrix = float3x3::LookAt(frustum.Front(), vector.Normalized(), frustum.Up(), float3::unitY);

	frustum.SetFront(matrix.MulDir(frustum.Front()).Normalized());
	frustum.SetUp(matrix.MulDir(frustum.Up()).Normalized());
}

float * C_Camera::GetOpenGLViewMatrix()
{
	static float4x4 m;

	m = frustum.ViewMatrix();
	m.Transpose();

	return (float*)m.v;
}

float * C_Camera::GetOpenGLProjectionMatrix()
{
	static float4x4 m;

	m = frustum.ProjectionMatrix();
	m.Transpose();

	return (float*)m.v;
}
