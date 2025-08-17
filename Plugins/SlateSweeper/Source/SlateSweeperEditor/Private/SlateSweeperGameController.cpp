// This is a technical test from Eugenio Del Vecchio for Geotech, please do not share.

#include "SlateSweeperGameController.h"
#include "SlateSweeperGameState.h"
#include "Widgets/SSlateSweeperMinefieldView.h"




FSlateSweeperGameController::FSlateSweeperGameController(const FSlateSweeperNewGameSettings& InGameSettings)
	: GameState(MakeShared<FSlateSweeperGameState>(InGameSettings))
{
}

void FSlateSweeperGameController::HandleOnCellPressed(int32 CellIndex)
{
	if (GameState->RevealCell(CellIndex) == ESlateSweeperCellRevealOutcome::HitMine)
	{
		GameState->RevealAllCells();
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("You hit a mine! Game over.")));
	}
	
	GameView->Update(GameState->GetGridData());
}

TWeakPtr<SSlateSweeperMinefieldView> FSlateSweeperGameController::GetOrCreateGameView()
{
	if (!GameView.IsValid())
	{
		GameView = SNew(SSlateSweeperMinefieldView).ViewData(GameState->GetGridData());

		// Lengthy registration but ensures delegate safety 
		GameView->RegisterOnCellPressed
		(
			FOnSlateSweeperCellPressed::FDelegate::CreateRaw
			(
				this,
				&FSlateSweeperGameController::HandleOnCellPressed
			)
		);
	}
	
	return GameView;
}

TWeakPtr<FSlateSweeperGameState> FSlateSweeperGameController::GetSlateSweeperGameState() const
{
	return GameState;
}
