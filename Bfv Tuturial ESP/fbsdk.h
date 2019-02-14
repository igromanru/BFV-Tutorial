#include <Windows.h>
#include <iostream>

// Include our PlayerManager Code
#include "PlayerManager.h"

// Include our DX9 Overlay Code, along with all the DX9 includes
#include "DX9OV.h"

// define our static offsets
//		with this cheat, GameRenderer is the only static offset needed.  The PlayerManager lib uses sig scanning
#define OFFSET_GAMERENDERER 0x1444b5158 

// the class for a WeakPtr
template<class T> class WeakPtr
{
private:
	T** m_ptr;
public:
	T* GetData()
	{
		if (!IsValidPtr(m_ptr))
		{
			return nullptr;
		}
		if (!*m_ptr)
		{
			return nullptr;
		}
		T* ptr = *m_ptr;
		return (T*)((DWORD64)ptr - 0x8);
	}
};

// these are the classes reversed from the game, using ReClass

// Created with ReClass.NET by KN4CK3R

class HealthComponent
{
public:
	char pad_0000[32]; //0x0000
	float currentHealth; //0x0020
	float maxHealth; //0x0024
	char pad_0028[1056]; //0x0028
}; //Size: 0x0448

class ClientSoldierEntity
{
public:
	char pad_0000[584]; //0x0000
	class HealthComponent* healthComponent; //0x0248
	char pad_0250[1544]; //0x0250
	uint8_t poseType; //0x0858
	char pad_0859[194]; //0x0859
	uint8_t occluded; //0x091B
	char pad_091C[532]; //0x091C
	D3DXVECTOR3 location; //0x0B30
	char pad_0B3C[1296]; //0x0B3C
}; //Size: 0x104C


class ClientPlayer
{
public:
	char pad_0000[24]; //0x0000
	char* name; //0x0018
	char pad_0020[7208]; //0x0020
	uint8_t teamId; //0x1C48
	char pad_1C49[255]; //0x1C49
	WeakPtr<ClientSoldierEntity> clientSoldierEntity; //0x1D48
	char pad_1D50[752]; //0x1D50
}; //Size: 0x2040

class RenderView
{
public:
	char pad_0000[1264]; //0x0000
	D3DXMATRIXA16 viewProj; //0x04F0
	char pad_0530[368]; //0x0530
	D3DXVECTOR3 cameraPos; //0x06A0
	char pad_06AC[4632]; //0x06AC
}; //Size: 0x05C0

class GameRenderer
{
public:
	char pad_0000[96]; //0x0000
	class RenderView* renderView; //0x0060
	char pad_0068[32]; //0x0068
	static GameRenderer* GetInstance()
	{
		return *(GameRenderer**)OFFSET_GAMERENDERER;
	}
}; //Size: 0x0088
