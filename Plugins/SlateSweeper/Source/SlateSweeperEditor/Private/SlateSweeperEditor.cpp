// This is a technical test from Eugenio Del Vecchio for Geotech, please do not share.

#include "SlateSweeperEditor.h"
#include "SlateSweeperStyle.h"
#include "SlateSweeperCommands.h"
#include "Widgets/SSlateSweeperTab.h"
#include "Misc/MessageDialog.h"
#include "ToolMenus.h"

static const FName SlateSweeperTabName("SlateSweeper");

#define LOCTEXT_NAMESPACE "FSlateSweeperEditorModule"

void FSlateSweeperEditor::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FSlateSweeperStyle::Initialize();
	FSlateSweeperStyle::ReloadTextures();

	FSlateSweeperCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FSlateSweeperCommands::Get().OpenSlateSweeperWindow,
		FExecuteAction::CreateStatic(&FSlateSweeperEditor::OpenSlateSweeperWindow),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FSlateSweeperEditor::RegisterMenus));

	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(SlateSweeperTabName, FOnSpawnTab::CreateStatic(&FSlateSweeperEditor::SpawnSlateSweeperWindow))
	.SetDisplayName(LOCTEXT("FSlateSweeperTabTitle", "SlateSweeper"))
	.SetMenuType(ETabSpawnerMenuType::Hidden);
	
}

void FSlateSweeperEditor::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FSlateSweeperStyle::Shutdown();

	FSlateSweeperCommands::Unregister();
}

void FSlateSweeperEditor::OpenSlateSweeperWindow()
{
	FGlobalTabmanager::Get()->TryInvokeTab(SlateSweeperTabName);
}

TSharedRef<SDockTab> FSlateSweeperEditor::SpawnSlateSweeperWindow(const class FSpawnTabArgs& SpawnTabArgs)
{
	return SNew(SSlateSweeperTab);
}

void FSlateSweeperEditor::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("PluginTools");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FSlateSweeperCommands::Get().OpenSlateSweeperWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSlateSweeperEditor, SlateSweeper)