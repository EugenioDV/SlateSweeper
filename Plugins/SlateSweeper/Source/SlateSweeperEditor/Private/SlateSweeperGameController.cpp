// This is a technical test from Eugenio Del Vecchio for Geotech, please do not share.

#include "SlateSweeperGameController.h"
#include "SlateSweeperGameState.h"
#include "Widgets/SSlateSweeperMinefieldView.h"




FSlateSweeperGameController::FSlateSweeperGameController(uint8 InMineGridWidth, uint8 InMineGridHeight, int32 InTotalMines)
	: GameState(MakeShared<FSlateSweeperGameState>(InMineGridWidth, InMineGridHeight, InTotalMines))
{
}

void FSlateSweeperGameController::HandleOnCellPressed(int32 CellIndex)
{
	GameState->RevealCell(CellIndex);
	GameView->Update(GameState->GetViewData()); //todo this is a temp solution just to test the game
}

TWeakPtr<SSlateSweeperMinefieldView> FSlateSweeperGameController::GetOrCreateGameView()
{
	if (!GameView.IsValid())
	{
		GameView = SNew(SSlateSweeperMinefieldView).ViewData(GameState->GetViewData());

		// Lengthy registration but ensures delegate safety 
		GameView->RegisterOnCellPressed
		(
			SSlateSweeperMinefieldView::FOnCellPressed::FDelegate::CreateRaw
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
