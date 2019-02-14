#include "fbsdk.h"

// declare some static variables necescary for WorldToScreen
// don't worry about setting these manually, we'll do this dynamically in the overlay callback function
static int s_Width = 1920;
static int s_Height = 1080;
static D3DXMATRIXA16 s_ViewProj = D3DXMATRIXA16(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

// check the validity of a given pointer.  Note that IsBadReadPtr is kinda slow
bool IsValidPtr(void* addr) {
	return !IsBadReadPtr(addr, sizeof(PVOID));
}

// calculates the distance between two 3d points
float Distance3D(D3DXVECTOR3  v1, D3DXVECTOR3 v2)
{
	float x_d = (v2.x - v1.x);
	float y_d = (v2.y - v1.y);
	float z_d = (v2.z - v1.z);
	return sqrt((x_d * x_d) + (y_d * y_d) + (z_d * z_d));
}

// calculate the screen coordinates in pixels given a world coordinate
bool WorldToScreen(D3DXVECTOR3 &vLocVec4)
{
	D3DXMATRIXA16 ViewProj = s_ViewProj;
	float mX = s_Width / 2;
	float mY = s_Height / 2;

	float w = ViewProj(0, 3) * vLocVec4.x + ViewProj(1, 3) * vLocVec4.y + ViewProj(2, 3) * vLocVec4.z + ViewProj(3, 3);

	if (w < 0.65f)
	{
		vLocVec4.z = w;
		return false;
	}
	float x = ViewProj(0, 0) * vLocVec4.x + ViewProj(1, 0) * vLocVec4.y + ViewProj(2, 0) * vLocVec4.z + ViewProj(3, 0);
	float y = ViewProj(0, 1) * vLocVec4.x + ViewProj(1, 1) * vLocVec4.y + ViewProj(2, 1) * vLocVec4.z + ViewProj(3, 1);
	vLocVec4.x = (mX + mX * x / w);
	vLocVec4.y = (mY - mY * y / w);
	vLocVec4.z = w;
	return true;
}