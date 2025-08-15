// This is a technical test from Eugenio Del Vecchio for Geotech, please do not share.

#include "SlateSweeperGame.h"
#include "SlateSweeperGameState.h"
#include "Widgets/SSlateSweeperMinefield.h"




FSlateSweeperGame::FSlateSweeperGame(uint8 InMineGridWidth, uint8 InMineGridHeight, int32 InTotalMines)
	:	GameState (MakeShared<FSlateSweeperGameState>(InMineGridWidth, InMineGridHeight, InTotalMines)),
		GameView(SNew(SSlateSweeperMinefield).Controller(this))
{
}

const TSharedRef<SSlateSweeperMinefield>& FSlateSweeperGame::GetSlateSweeperMinefield() const
{
	return GameView;
}

const TSharedRef<class FSlateSweeperGameState>& FSlateSweeperGame::GetSlateSweeperGameState() const
{
	return GameState;
}
