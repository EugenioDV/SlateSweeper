// This is a technical test from Eugenio Del Vecchio for Geotech, please do not share.

#include "SlateSweeperGameController.h"
#include "SlateSweeperGameState.h"
#include "Widgets/SSlateSweeperMinefieldView.h"




FSlateSweeperGameController::FSlateSweeperGameController(uint8 InMineGridWidth, uint8 InMineGridHeight, int32 InTotalMines)
	:	GameState (MakeShared<FSlateSweeperGameState>(InMineGridWidth, InMineGridHeight, InTotalMines))
{
	//GameView->AddOnCellPressed(FOnCellPressed::FDelegate::CreateSP(this, &FSlateSweeperGameController::HandleCellPressed));
}

//todo turn this into a delegate architecture
void FSlateSweeperGameController::OnCellPressed(int32 CellIndex)
{
	GameState->PressCell(CellIndex);
	GameView->Redraw(); //todo this is a temp solution just to test the game
}

TWeakPtr<SWidget> FSlateSweeperGameController::GetOrCreateGameView()
{
	if (!GameView.IsValid())
	{
		GameView = SNew(SSlateSweeperMinefieldView).ViewData(GameState->GetViewData());
	}
	
	return GameView->AsWeak();
}

TWeakPtr<FSlateSweeperGameState> FSlateSweeperGameController::GetSlateSweeperGameState() const
{
	return GameState;
}
