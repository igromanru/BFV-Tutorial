#include <Windows.h>
#pragma comment(lib, "PlayerManager.lib")

class PlayerManager	{
public:
	static void* GetLocalPlayer(void);
	static void* GetPlayerById(int); 
	static void Detach(void);
};