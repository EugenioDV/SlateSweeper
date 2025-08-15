// This is a technical test from Eugenio Del Vecchio for Geotech, please do not share.
#include "SSlateSweeperMinefield.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SUniformGridPanel.h"
#include "SlateSweeperGame.h"
#include "SlateSweeperGameState.h"


TSharedRef<SButton> CraftGridCellButton(int32 ButtonIndex){
	
	return SNew(SButton);
}

void SSlateSweeperMinefield::Construct(const FArguments& InArgs) //todo this looks weird
{

	if (!InArgs._Controller)
	{
		//todo throw error
		return;
	}
	Controller = InArgs._Controller;

	auto GameState = Controller->GetSlateSweeperGameState();

	auto Height = GameState->GetMineGridHeight();
	auto Width = GameState->GetMineGridWidth();
	auto Mines = GameState->GetMineCells();
	auto NeighbourCounts = GameState->GetCellNeighbourCounts();
	
	TSharedRef<SUniformGridPanel> GridPanel =
		SNew(SUniformGridPanel)
		.MinDesiredSlotHeight(25.f) //todo standard for cell size? Where should I store style stuff like this?
		.MinDesiredSlotWidth(25.f);

	for (uint8 Row = 0; Row < Height; Row++)
	{
		for (uint8 Column = 0; Column < Width; Column++)
		{
			int32 CellIndex = Row * Width + Column;
			
			GridPanel->AddSlot(Column, Row)
			[
				SNew(SButton)
				.ButtonColorAndOpacity(Mines[CellIndex] ? FSlateColor(FLinearColor::Red) : FSlateColor(FLinearColor::Black))
				.Text(
					NeighbourCounts[CellIndex] > 0
					? FText::AsNumber(NeighbourCounts[CellIndex])
					: FText::GetEmpty()
					)
			];
		}
	}
	
	SBox::Construct(
		SBox::FArguments()
		[
		GridPanel
		]
	);
	
}
