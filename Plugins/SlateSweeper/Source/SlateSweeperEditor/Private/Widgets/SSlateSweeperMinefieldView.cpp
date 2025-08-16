// This is a technical test from Eugenio Del Vecchio for Geotech, please do not share.

#include "SSlateSweeperMinefieldView.h"

#include "SlateSweeperEditor.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SUniformGridPanel.h"
#include "Widgets/Text/STextBlock.h"

TSharedRef<SWidget> SSlateSweeperMinefieldView::CraftGridCellButton(int32 InCellIndex) const
{
	/*
	 * A Revealed cell can either be:
	 * 1) a cell showing a number
	 * 2) a hidden cell because there were no adjacient bombs
	 * 3) an exploded bomb (game over!)
	 * In each case, it's disabled
	 */

	// While this might look hacky, we are mimicking the style of a disabled button for consistency across our "default look" grid, without having to do heavy style setup
	const FButtonStyle* BorderButtonCamouflage = &FCoreStyle::Get().GetWidgetStyle<FButtonStyle>("Button");
	
	if (ViewData.RevealedCells[InCellIndex])
	{
		return SNew(SBorder)
			.Visibility(EVisibility::HitTestInvisible)
			.BorderImage(
				BorderButtonCamouflage
				? &BorderButtonCamouflage->Disabled
				: FCoreStyle::Get().GetBrush("WhiteBrush")
				)
			.BorderBackgroundColor
				(
				ViewData.MineCells[InCellIndex]
					? FLinearColor::Red
					: FLinearColor::Black
				)
			.VAlign(VAlign_Center)
			.HAlign(HAlign_Center)
			[
				// We only create the text widget if there is a text to display, using the SNullWidget trick
				ViewData.RevealedCellNeighbourCounts[InCellIndex] > 0
				? SNew(STextBlock)
				.Visibility(EVisibility::HitTestInvisible)
				.Text(FText::AsNumber(ViewData.RevealedCellNeighbourCounts[InCellIndex]))
				.AutoWrapText(false)
				.Margin(0.f)
				.ApplyLineHeightToBottomLine(false)
				.Justification(ETextJustify::Center)
				.Clipping(EWidgetClipping::OnDemand)
				: SNullWidget::NullWidget
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

	if (ViewData.GridWidth == 0 || ViewData.GridHeight == 0)
	{
		UE_LOG(LogSlateSweeper, Error, TEXT("Failed to create Minefield View. Grid size invalid"));
		return;
	}

	GridPanel = SNew(SUniformGridPanel)
				.MinDesiredSlotHeight(25.f) //todo standard for cell size? Where should I store style stuff like this?
				.MinDesiredSlotWidth(25.f);

	for (uint8 Row = 0; Row < ViewData.GridHeight; Row++)
	{
		for (uint8 Column = 0; Column < ViewData.GridWidth; Column++)
		{
			int32 CellIndex = Row * ViewData.GridWidth + Column;
			
			GridPanel->AddSlot(Column, Row)
			[
				CraftGridCellButton(CellIndex)
			];
		}
	}

	// Don't let the grid stretch, even when dimensions aren't 1:1
	float AspectRatio = static_cast<float>(ViewData.GridWidth) / ViewData.GridHeight;
	SBox::Construct(
		SBox::FArguments()
		[
			GridPanel.ToSharedRef() //todo this whole grid panel thing is cringe
		]
		.MaxAspectRatio(AspectRatio)
		.MinAspectRatio(AspectRatio)
	);
	
}

void SSlateSweeperMinefieldView::Update(const FSlateSweeperViewData& NewData)
{
	GridPanel->ClearChildren();
	Construct(FArguments().ViewData(NewData));
}
