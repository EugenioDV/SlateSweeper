// This is a technical test from Eugenio Del Vecchio for Geotech, please do not share.

#pragma once

/*
 * This class represents a minesweeper game that is being played
 * In this architecture, it spawns/owns the "view" (a slate widget) and the "model" (state)
 * It is the only gateway to access the view and the model
 */

class FSlateSweeperGameController
{
public:
	
	FSlateSweeperGameController(uint8 InMineGridWidth, uint8 InMineGridHeight, int32 InTotalMines);
	TWeakPtr<SWidget> GetOrCreateGameView();
	TWeakPtr<class FSlateSweeperGameState> GetSlateSweeperGameState() const;
	void OnCellPressed(int32 CellIndex);

private:
	
	// While the view is not essential and just presents the data, the controller has no reason to exist without a game state
	TSharedRef<class FSlateSweeperGameState, ESPMode::ThreadSafe> GameState;
	TSharedPtr<class SSlateSweeperMinefieldView, ESPMode::ThreadSafe> GameView;
};
