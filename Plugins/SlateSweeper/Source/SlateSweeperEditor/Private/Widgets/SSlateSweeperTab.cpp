// This is a technical test from Eugenio Del Vecchio for Geotech, please do not share.

#include "SSlateSweeperTab.h"

#include "SlateSweeperEditor.h"
#include "SlateSweeperGameController.h"
#include "SlateSweeperSettings.h"
#include "SSlateSweeperMenu.h"
#include "SSlateSweeperMinefieldView.h"

/*
 * TODO style start
 * Access editor row width
 * text size
 * highlight color
 * default background color
 * padding values (won't be easy since they use indentation widgets)
 * perhaps a spliter or a drop down thing for the menu so that there is more space for playing when collapsed
 * todo style end
 *
 */

void SSlateSweeperTab::Construct(const FArguments& InArgs)
{
	SlateSweeperModule = InArgs._Module;

	check (SlateSweeperModule)

	SDockTab::Construct(
		SDockTab::FArguments()
		.TabRole(ETabRole::NomadTab)
		[
			SNew(SVerticalBox)
			+SVerticalBox::Slot()
			.FillHeight(1.f)
			[
				SNew(SSlateSweeperMenu)
				.OnStartGameClicked(this, &SSlateSweeperTab::OnStartNewGamePressed)
			]
			+SVerticalBox::Slot()
			.FillHeight(1.f) 
			[
				SAssignNew(MinefieldContainer, SBox)
			]
		]);
	
	// Attempt to recover the previous game in case of a closed-reopened tab
	CurrentGame = SlateSweeperModule->GetCurrentGameController();
	if (CurrentGame.IsValid())
	{
		MinefieldContainer->SetContent(CurrentGame.Pin()->GetOrCreateGameView().Pin().ToSharedRef());
	}
}

void SSlateSweeperTab::OnStartNewGamePressed(uint8 GridWidth, uint8 GridHeight, int32 TotalMines)
{
	CurrentGame = SlateSweeperModule->StartNewGame(GridWidth, GridHeight, TotalMines);
	
	if (!CurrentGame.IsValid())
	{
		UE_LOG(LogSlateSweeper, Error, TEXT("Failed to start new game. Valid checks failed."));
		return;
	}
	
	MinefieldContainer->SetContent(CurrentGame.Pin()->GetOrCreateGameView().Pin().ToSharedRef());
}

