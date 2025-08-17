// This is a technical test from Eugenio Del Vecchio for Geotech, please do not share.

#pragma once
#include "SlateSweeperTypes.h"

/*
 * This class represents a minesweeper game that is being played
 * In this architecture, it spawns/owns the "view" (a slate widget) and the "model" (state)
 * If we were to implement cooperation with other modules, we'd do it here
 */

class FSlateSweeperGameController
{
public:
	
	FSlateSweeperGameController(const FSlateSweeperNewGameSettings& InGameSettings);
	TWeakPtr<class SSlateSweeperMinefieldView> GetOrCreateGameView();
	TWeakPtr<class FSlateSweeperGameState> GetSlateSweeperGameState() const;

private:
	
	// While technically this function can be const, it changes too many things
	void HandleOnCellPressed(int32 CellIndex);

	// While the view is not essential and just presents the data, the controller has no reason to exist without a game state
	TSharedRef<class FSlateSweeperGameState, ESPMode::ThreadSafe> GameState;
	TSharedPtr<class SSlateSweeperMinefieldView, ESPMode::ThreadSafe> GameView;
};
