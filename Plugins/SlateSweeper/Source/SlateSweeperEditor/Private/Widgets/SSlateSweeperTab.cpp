// This is a technical test from Eugenio Del Vecchio for Geotech, please do not share.

#include "SSlateSweeperTab.h"
#include "SlateSweeperEditor.h"
#include "SlateSweeperEditorSettings.h"
#include "SlateSweeperGameController.h"
#include "SSlateSweeperMenu.h"
#include "SSlateSweeperMinefieldView.h"


void SSlateSweeperTab::Construct(const FArguments& InArgs)
{
	FSlateSweeperEditor& SlateSweeperModule =  FModuleManager::LoadModuleChecked<FSlateSweeperEditor>("SlateSweeperEditor");
	const USlateSweeperEditorSettings* GeneralSettings = GetDefault<USlateSweeperEditorSettings>();

	SDockTab::Construct(
		SDockTab::FArguments()
		.TabRole(ETabRole::NomadTab)
		[
			SNew(SVerticalBox)
			+SVerticalBox::Slot()
			.AutoHeight()
			.VAlign(VAlign_Top)
			// Leave some space at the bottom so the grid appears separated from the menu
			.Padding(FMargin(0.f, 0.f, 0.f, GeneralSettings ? GeneralSettings->DesiredCellSize/2.f : 10.f))
			[
				SNew(SSlateSweeperMenu)
				.OnStartGameClicked(this, &SSlateSweeperTab::OnStartNewGamePressed)
			]
			+SVerticalBox::Slot()
			.FillHeight(1.f)
			.VAlign(VAlign_Top)
			.HAlign(HAlign_Left)
			[
				SAssignNew(MinefieldContainer, SBox)
			]
		]);
	
	// Attempt to recover the previous game in case of a closed-reopened tab
	CurrentGame = SlateSweeperModule.GetCurrentGameController();
	if (CurrentGame.IsValid())
	{
		MinefieldContainer->SetContent(CurrentGame.Pin()->GetOrCreateGameView().Pin().ToSharedRef());
	}
}

void SSlateSweeperTab::OnStartNewGamePressed(const FSlateSweeperNewGameSettings& GameSettings)
{
	FSlateSweeperEditor& SlateSweeperModule =  FModuleManager::LoadModuleChecked<FSlateSweeperEditor>("SlateSweeperEditor");
	
	CurrentGame = SlateSweeperModule.StartNewGame(GameSettings);
	
	if (!CurrentGame.IsValid())
	{
		UE_LOG(LogSlateSweeper, Error, TEXT("Failed to start new game. Valid checks failed."));
		return;
	}
	
	MinefieldContainer->SetContent(CurrentGame.Pin()->GetOrCreateGameView().Pin().ToSharedRef());
}

