// This is a technical test from Eugenio Del Vecchio for Geotech, please do not share.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FToolBarBuilder;
class FMenuBuilder;

class FSlateSweeperEditor  : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	/** This function will be bound to Command. */
	static void OpenSlateSweeperWindow();

	static TSharedRef<class SDockTab> SpawnSlateSweeperWindow(const class FSpawnTabArgs& SpawnTabArgs);	
private:

	void RegisterMenus();


private:
	TSharedPtr<class FUICommandList> PluginCommands;
};
