// This is a technical test from Eugenio Del Vecchio for Geotech, please do not share.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "SlateSweeperTypes.h"

class FToolBarBuilder;
class FMenuBuilder;

DECLARE_LOG_CATEGORY_EXTERN(LogSlateSweeper, Log, All);

/*
 * The module acts as a composition root and lifetime owner (create controller, create tab)
 * The rest is mostly boilerplate code for the toolbar button and so forth
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
	TWeakPtr<class FSlateSweeperGameController,  ESPMode::ThreadSafe> StartNewGame(const FSlateSweeperNewGameSettings& GameSettings);
	
	TWeakPtr<class FSlateSweeperGameController,  ESPMode::ThreadSafe> GetCurrentGameController();
private:
	void RegisterMenus();

private:
	TSharedPtr<class FUICommandList> PluginCommands;
	
	//While this is the sole "strong" reference to this resource, there is no way to use UniquePtr without risk of dangling pointers
	TSharedPtr<class FSlateSweeperGameController,  ESPMode::ThreadSafe> CurrentGame;
};
