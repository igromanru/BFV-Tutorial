#include "Utilities.h"
//https://i.imgur.com/p7K8YwV.jpg
// include our utilities header, which has special math functions and includes the fbsdk, dx9ov, and PlayerManager

// some helper pre-processor directives to make sure you build it right
#ifdef _DEBUG
#error Compile in Release mode
#endif
#ifndef _WIN64
#error Compile as 64 bit
#endif
#ifndef _WINDLL
#error Compile as a DLL
#endif

#ifndef __D3DX9_H__
#error you must include the DirectX SDK
// go to Project->Properties->VC++ Directories, and add 
//		$(DXSDK_DIR)\Include;
// and
//		$(DXSDK_DIR)\Lib\x64;
// to the Include and Library directories, respectively
// should reside in C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)
#endif

// we'll use this to simplifiy checking of settings.  Hover over an implimentation to see what it expands to
#define checkSetting(teamval, enemyval) (((pLocalPlayer->teamId == pPlayer->teamId) && teamval) || ((pLocalPlayer->teamId != pPlayer->teamId) && enemyval))

// be lazy and paste colors
#define pasteColor(x) x.r, x.g, x.b
#define pasteColorA(x) x.r, x.g, x.b, x.a

// chance these settings to your liking
namespace Settings {
	// our colors to set players to
	D3DXCOLOR teamColor = D3DXCOLOR(0, 255, 0, 255);
	D3DXCOLOR occludedEnemyColor = D3DXCOLOR(255, 0, 0, 255);
	D3DXCOLOR visibleEnemyColor = D3DXCOLOR(255, 255, 0, 255);

	// the boxes to be drawn around enemies
	bool enemyBoxes = true;
	bool teamBoxes = true;

	// draw their names above their heads
	bool enemyNames = true;
	bool teamNames = true;

	// draw the distance from our player to theirs
	bool enemyDistance = true;
	bool teamDistance = true;

	// draw lines from the bottom of the screen to their foot
	bool enemySnaplines = true;
	bool teamSnaplines = true;

	// draw healthbars next to the players
	bool enemyHealth = true;
	bool teamHealth = true;

	// the width of our lines to be drawn, for boxes and snaplines
	int lineWidth = 2;
}


// this function will get called once every frame of the overlay
void drawLoop(Dxo* dxo, int width, int height) {
	// set our static variables for W2S to work
	s_Width = width;
	s_Height = height;

	// Get the view projection matrix
	GameRenderer* pGameRenderer = GameRenderer::GetInstance();
	// check that the RenderView class is valid
	if (!IsValidPtr(pGameRenderer) || !IsValidPtr(pGameRenderer->renderView)) return;
	// set our static ViewProjection for W2S to work
	s_ViewProj = pGameRenderer->renderView->viewProj;

	// Get the LocalPlayer
	ClientPlayer* pLocalPlayer = (ClientPlayer*)PlayerManager::GetLocalPlayer();

	// make sure the player is valid
	if (!IsValidPtr(pLocalPlayer)) return;
	if (!IsValidPtr(pLocalPlayer->name)) return;

	// Get the LocalSoldier
	ClientSoldierEntity* pLocalSoldier = pLocalPlayer->clientSoldierEntity.GetData();

	// make sure the soldier is valid
	if (!IsValidPtr(pLocalSoldier)) return;

	// start our player loop
	for (size_t i = 0; i < 64; i++) {

		// Get the player by it's index
		ClientPlayer* pPlayer = (ClientPlayer*)PlayerManager::GetPlayerById(i);

		// check the pointer's validity
		if (!IsValidPtr(pPlayer)) continue;
		
		// make sure the player isn't the localplayer
		if (pPlayer == pLocalPlayer) continue;

		// get the ClientSoldierEntity
		ClientSoldierEntity* pSoldier = pPlayer->clientSoldierEntity.GetData();

		// check it's pointer to make sure they're spawned
		if (!IsValidPtr(pSoldier)) continue;

		// check that their health component is valid
		if (!IsValidPtr(pSoldier->healthComponent)) continue;

		// check that their health is above 0
		if (pSoldier->healthComponent->currentHealth <= 0) continue;

		// set up our color to render with
		D3DXCOLOR chosenColor;
		// if the team IDs are the same
		if (pPlayer->teamId == pLocalPlayer->teamId) {
			// set the color to the predefined teamcolor
			chosenColor = Settings::teamColor;
		}
		else {
			// the soldier is an enemy, now check if they're occluded (not visible)
			if (pSoldier->occluded) {
				// if they aren't visible, set the color to their occluded color
				chosenColor = Settings::occludedEnemyColor;
			}
			else {
				// otherwise set it to Settings::visibleEnemyColor
				chosenColor = Settings::visibleEnemyColor;
			}
		}

		// get the position of the players foot, and their head
		D3DXVECTOR3 footLocation = pSoldier->location;
		D3DXVECTOR3 headLocation = footLocation;
		// hardcode a head height offset, based on pose

		// poseType 0 = standing
		// poseType 1 = crouching
		// poseType 2 = prone
		if (pSoldier->poseType == 0) headLocation.y += 1.6;
		if (pSoldier->poseType == 1) headLocation.y += 1;
		if (pSoldier->poseType == 2) headLocation.y += .5;
		

		// Convert their world coordinates to screen coordinates
		if (WorldToScreen(footLocation) && WorldToScreen(headLocation)) {
			// calculate the pixels between the top and bottom of the soldier
			float w2sHeight = Distance3D(footLocation, headLocation);
			float w2sWidth = w2sHeight;
			// adjust the w2sWidth so the boxes aren't proportional
			if (pSoldier->poseType == 0) w2sWidth /= 2;
			if (pSoldier->poseType == 1) w2sWidth /= 1.5;

			// get the (world) distance between the localplayer and the enemy player
			float distanceToPlayer = Distance3D(pLocalSoldier->location, pSoldier->location);

			// if the localplayer and the pPlayer are on the same teams and teamboxes are enabled,
			// or
			// if the localplayer and the pPlayer are on opposite teams and enemyboxes are enabled:
			if checkSetting(Settings::teamBoxes, Settings::enemyBoxes) {
				// draw a box around them
				dxo->DrawBox(headLocation.x - w2sWidth / 2, headLocation.y, w2sWidth, w2sHeight, Settings::lineWidth, pasteColorA(chosenColor));
			}

			// check if snapliens are to be drawn
			if (checkSetting(Settings::teamSnaplines, Settings::enemySnaplines)) {
				// if so, draw a line from the middle bottom of the screen, to their foot location
				dxo->DrawLine(s_Width / 2, s_Height, footLocation.x, footLocation.y, Settings::lineWidth, pasteColor(chosenColor));
			}

			// draw their names
			if (checkSetting(Settings::teamNames, Settings::enemyNames)) {
				dxo->DrawString(pPlayer->name, headLocation.x, headLocation.y + 20, pasteColor(chosenColor));
			}

			// draw distances
			if (checkSetting(Settings::teamDistance, Settings::enemyDistance)) {
				dxo->DrawString(std::to_string((int)distanceToPlayer).c_str(), footLocation.x, footLocation.y + 20, pasteColor(chosenColor));
			}

			// draw healthBars
			if (checkSetting(Settings::teamHealth, Settings::enemyHealth)) {
				// divide the health
				float health = (pSoldier->healthComponent->currentHealth / pSoldier->healthComponent->maxHealth) * 255;
				
				// Calculate the color to draw the boxes
				D3DXCOLOR boxColor(255 - health, health, 0, 255);
				float border = 2;
				float box_width = 10;
				float boxOffset = w2sWidth * .75f;
				// draw a black box first
				dxo->DrawFilledBox(headLocation.x + boxOffset, headLocation.y, 10, w2sHeight, 0, 0, 0, 255);
				// now draw a colored box with a height value based off of their health
				dxo->DrawFilledBox(headLocation.x + boxOffset + border, headLocation.y + border, box_width - (2 * border), (w2sHeight - (2 * border))*(health / 255), pasteColorA(boxColor));
			}


		} // end of w2s check
	} // end of player-loop
} // end of drawing callback

// the thread where we'll be setting up our hack stuff
DWORD WINAPI hackThread(LPVOID lpParam) {

	// Create our overlay and start rendering
	// The first arg, drawLoop, is the address of our callback function
	//		it'll be called once per frame of our overlay
	Dxo::SetupRenderer(drawLoop);

	// keep this thread idling
	for (;;) { Sleep(500); }
}

// the DllMain, the entry point of our module
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	// check if the dll was attached to a process
	if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
		// create a thread to set up our stuff.  Pass the handle to our module for ejection
		CreateThread(NULL, NULL, hackThread, hModule, NULL, NULL);
	}

	// check if the dll is being unloaded
	if (ul_reason_for_call == DLL_PROCESS_DETACH) {
		// need to unhook our PlayerManager stuff
		PlayerManager::Detach();
	}
	return TRUE;
}