// This is a technical test from Eugenio Del Vecchio for Geotech, please do not share.

#include "SSlateSweeperMinefieldView.h"

#include "SlateSweeperEditor.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SUniformGridPanel.h"
#include "Widgets/Layout/SScaleBox.h" //todo this doesn't fix anything

TSharedRef<SWidget> SSlateSweeperMinefieldView::CraftGridCellButton(int32 InCellIndex) const
{
	/*
	 * A Revealed cell can either be:
	 * 1) a cell showing a number
	 * 2) a hidden cell because there were no adjacient bombs
	 * 3) an exploded bomb (game over!)
	 * In each case, it's disabled
	 */
	
	if (ViewData.RevealedCells[InCellIndex])
	{
		return SNew(SButton)
			.IsEnabled(false)
			.ButtonColorAndOpacity(
				ViewData.MineCells[InCellIndex]
					? FSlateColor(FLinearColor::Red)
					: FSlateColor(FLinearColor::Black)
			)
			[
				SNew(STextBlock)
				.Text(
					ViewData.RevealedCellNeighbourCounts[InCellIndex] > 0
						? FText::AsNumber(ViewData.RevealedCellNeighbourCounts[InCellIndex])
						: FText::GetEmpty()
				)
			];
	}

	// We use lambda to capture cell index and avoid extra code (e.g. SButton subclass to store index or index from XY)
	return SNew(SButton)
			.OnPressed_Lambda([this, InCellIndex]
			{
				OnCellPressedDelegate.Broadcast(InCellIndex);
			});
}

void SSlateSweeperMinefieldView::Construct(const FArguments& InArgs) //todo this looks weird
{
	ViewData = InArgs._ViewData;

	if (ViewData.FieldGridWidth == 0 || ViewData.FieldGridHeight == 0)
	{
		UE_LOG(LogSlateSweeper, Error, TEXT("Failed to create Minefield View. Grid size invalid"));
		return;
	}

	GridPanel = SNew(SUniformGridPanel)
				.MinDesiredSlotHeight(25.f) //todo standard for cell size? Where should I store style stuff like this?
				.MinDesiredSlotWidth(25.f);

	for (uint8 Row = 0; Row < ViewData.FieldGridHeight; Row++)
	{
		for (uint8 Column = 0; Column < ViewData.FieldGridWidth; Column++)
		{
			int32 CellIndex = Row * ViewData.FieldGridWidth + Column;
			
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

void SSlateSweeperMinefieldView::Update(const FSlateSweeperViewData& NewData)
{
	GridPanel->ClearChildren();
	Construct(FArguments().ViewData(NewData));
}
