// This is a technical test from Eugenio Del Vecchio for Geotech, please do not share.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FToolBarBuilder;
class FMenuBuilder;



/*
 * This guy acts as a composition root and lifetime owner (create controller, create tab)
 * Todo the tab is not the view, but then what is it? the view is the game itself, the minefield, the tab is like a settings menu that
 * Talks to what? Does it hold the values? Does it matter at this scope?
 *  * todo check all function input to be In or Out and when not to be, use const ref when needed and debate const value
 *			aka IN is used in constructors for values that are taken IN (aka saved), out is used in functions for references to write to, all the rest is without in/pit
* todo remove all class pre-function input in cpp files
* todo all that stuff about FORCEINLINE and so on, I never use it =/
 */

class FSlateSweeperEditor  : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	/** This function will be bound to Command. */
	static void OpenSlateSweeperWindow();

	TSharedRef<class SDockTab> SpawnSlateSweeperWindow(const class FSpawnTabArgs& SpawnTabArgs);
	TWeakPtr<class FSlateSweeperGameController,  ESPMode::ThreadSafe> StartNewGame(uint8 GameMineGridWidth, uint8 GameMineGridHeight, int32 GameTotalMines);
	
	TWeakPtr<class FSlateSweeperGameController,  ESPMode::ThreadSafe> GetCurrentGameController();
private:
	void RegisterMenus();

private:
	TSharedPtr<class FUICommandList> PluginCommands;
	
	//While this is the sole "strong" reference to this resource, there is no way to use UniquePtr without risk of dangling pointers
	TSharedPtr<class FSlateSweeperGameController,  ESPMode::ThreadSafe> CurrentGame;
};
