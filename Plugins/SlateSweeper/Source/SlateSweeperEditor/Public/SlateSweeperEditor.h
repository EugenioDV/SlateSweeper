// This is a technical test from Eugenio Del Vecchio for Geotech, please do not share.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FToolBarBuilder;
class FMenuBuilder;

DECLARE_LOG_CATEGORY_EXTERN(LogSlateSweeper, Log, All);

/*
 * todo desc This guy acts as a composition root and lifetime owner (create controller, create tab)
 * todo final checks verify it compiles on 5.6
 * todo search // and /* for consistent-looking comments
 * todo check all #includes 
 * todo solve the const riddle: interface, friend or superclass pass are all options
 * const ref to shareptr<class>: can't modify the pointer itself, but can modify the object it points to
 * const ref to sharedptr<const class>: can't modify either. Do we want to expose view and state to outside modders?
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
