// This is a technical test from Eugenio Del Vecchio for Geotech, please do not share.

#include "SSlateSweeperTab.h"

#include "SlateSweeperEditor.h"
#include "SlateSweeperGameController.h"
#include "SSlateSweeperMenu.h"
#include "SSlateSweeperMinefieldView.h"


void SSlateSweeperTab::Construct(const FArguments& InArgs)
{
	FSlateSweeperEditor& SlateSweeperModule =  FModuleManager::LoadModuleChecked<FSlateSweeperEditor>("SlateSweeperEditor");

	SDockTab::Construct(
		SDockTab::FArguments()
		.TabRole(ETabRole::NomadTab)
		[
			SNew(SVerticalBox)
			+SVerticalBox::Slot()
			.AutoHeight()
			.VAlign(VAlign_Top)
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

void SSlateSweeperTab::OnStartNewGamePressed(uint8 GridWidth, uint8 GridHeight, int32 TotalMines)
{
	FSlateSweeperEditor& SlateSweeperModule =  FModuleManager::LoadModuleChecked<FSlateSweeperEditor>("SlateSweeperEditor");
	
	CurrentGame = SlateSweeperModule.StartNewGame(GridWidth, GridHeight, TotalMines);
	
	if (!CurrentGame.IsValid())
	{
		UE_LOG(LogSlateSweeper, Error, TEXT("Failed to start new game. Valid checks failed."));
		return;
	}
	
	MinefieldContainer->SetContent(CurrentGame.Pin()->GetOrCreateGameView().Pin().ToSharedRef());
}

