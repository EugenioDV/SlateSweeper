// This is a technical test from Eugenio Del Vecchio for Geotech, please do not share.

#include "SlateSweeperGameController.h"
#include "SlateSweeperGameModel.h"
#include "Widgets/SSlateSweeperGameView.h"


FSlateSweeperGameController::FSlateSweeperGameController(const FSlateSweeperNewGameSettings& InGameSettings)
	: GameModel(MakeShared<FSlateSweeperGameModel>(InGameSettings))
{
}

void FSlateSweeperGameController::HandleOnCellPressed(int32 CellIndex)
{
	if (GameModel->RevealCell(CellIndex) == ESlateSweeperCellRevealOutcome::HitMine)
	{
		GameModel->RevealAllCells();
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("You hit a mine! Game over.")));
	}
	
	GameView->Update(GameModel->GetGridData());
}

TWeakPtr<SSlateSweeperGameView> FSlateSweeperGameController::GetOrCreateGameView()
{
	if (!GameView.IsValid())
	{
		GameView = SNew(SSlateSweeperGameView).ViewData(GameModel->GetGridData());

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

TWeakPtr<FSlateSweeperGameModel> FSlateSweeperGameController::GetSlateSweeperGameModel() const
{
	return GameModel;
}
