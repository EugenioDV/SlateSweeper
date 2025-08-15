// This is a technical test from Eugenio Del Vecchio for Geotech, please do not share.

#include "SlateSweeperGameController.h"
#include "SlateSweeperGameState.h"
#include "Widgets/SSlateSweeperMinefield.h"




FSlateSweeperGameController::FSlateSweeperGameController(uint8 InMineGridWidth, uint8 InMineGridHeight, int32 InTotalMines)
	:	GameState (MakeShared<FSlateSweeperGameState>(InMineGridWidth, InMineGridHeight, InTotalMines)),
		GameView(SNew(SSlateSweeperMinefield).Controller(this))
{
}

const TSharedRef<SSlateSweeperMinefield>& FSlateSweeperGameController::GetSlateSweeperMinefield() const
{
	return GameView;
}

//todo turn this into a delegate architecture
void FSlateSweeperGameController::OnCellPressed(int32 CellIndex)
{
	GameState->PressCell(CellIndex);
	GameView->Redraw(); //todo this is a temp solution just to test the game
}

const TSharedRef<class FSlateSweeperGameState>& FSlateSweeperGameController::GetSlateSweeperGameState() const
{
	return GameState;
}
