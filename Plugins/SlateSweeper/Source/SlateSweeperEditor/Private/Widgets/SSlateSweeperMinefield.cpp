// This is a technical test from Eugenio Del Vecchio for Geotech, please do not share.

#include "SSlateSweeperMinefield.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SUniformGridPanel.h"
#include "SlateSweeperGameController.h"
#include "SlateSweeperGameState.h"
#include "Widgets/Layout/SScaleBox.h" //todo this doesn't fix anything


TSharedRef<SWidget> SSlateSweeperMinefield::CraftGridCellButton(int32 InCellIndex) const
{

	//todo refactor
	auto GameState = Controller->GetSlateSweeperGameState();
	auto Mines = GameState->GetMineCells();
	auto NeighbourCounts = GameState->GetCellNeighbourCounts();
	auto RevealedCells = GameState->GetRevealedCells();

	/* A Revealed cell can either be:
	* a cell showing a number
	* a hidden cell because there were no adjacient bombs
	* an exploded bomb (game over!)
	* In each case, it's disabled
	*/
	if (RevealedCells[InCellIndex])
	{
		return SNew(SButton)
			.IsEnabled(false)
			.ButtonColorAndOpacity(
				Mines[InCellIndex]
					? FSlateColor(FLinearColor::Red)
					: FSlateColor(FLinearColor::Black)
			)
			[
				SNew(SScaleBox)
				.Stretch(EStretch::ScaleToFit)
				[
					SNew(STextBlock)
					.Text(
						NeighbourCounts[InCellIndex] > 0
							? FText::AsNumber(NeighbourCounts[InCellIndex])
							: FText::GetEmpty()
					)
				]
			];
	}

	
	return SNew(SButton)
			.OnPressed_Lambda([this, InCellIndex]() //todo explain why this looks bad but is badass
			{
				if (Controller)
				{
					Controller->OnCellPressed(InCellIndex);
				}
			});
}

void SSlateSweeperMinefield::Construct(const FArguments& InArgs) //todo this looks weird
{

	if (!InArgs._Controller)
	{
		//todo throw error
		return;
	}
	Controller = InArgs._Controller;

	//todo refactor this
	auto GameState = Controller->GetSlateSweeperGameState();

	auto Height = GameState->GetMineGridHeight();
	auto Width = GameState->GetMineGridWidth();

	GridPanel = SNew(SUniformGridPanel)
				.MinDesiredSlotHeight(25.f) //todo standard for cell size? Where should I store style stuff like this?
				.MinDesiredSlotWidth(25.f);

	for (uint8 Row = 0; Row < Height; Row++)
	{
		for (uint8 Column = 0; Column < Width; Column++)
		{
			int32 CellIndex = Row * Width + Column;
			
			GridPanel->AddSlot(Column, Row)
			[
				CraftGridCellButton(CellIndex)
			];
		}
	}
	
	SBox::Construct(
		SBox::FArguments()
		[
			GridPanel.ToSharedRef() //todo this whole grid panel thing is cringe
		]
	);
	
}

void SSlateSweeperMinefield::Redraw()
{
	GridPanel->ClearChildren();
	Construct(FArguments().Controller(Controller));
}
