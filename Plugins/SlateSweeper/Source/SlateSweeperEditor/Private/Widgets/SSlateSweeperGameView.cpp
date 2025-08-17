// This is a technical test from Eugenio Del Vecchio for Geotech, please do not share.

#include "SSlateSweeperGameView.h"
#include "SlateSweeperEditor.h"
#include "SlateSweeperEditorSettings.h"
#include "Widgets/SInvalidationPanel.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SUniformGridPanel.h"
#include "Widgets/Text/STextBlock.h"


TSharedRef<SWidget> SSlateSweeperGameView::CraftGridCell(int32 InCellIndex) const
{
	const FSlateSweeperGridData& ViewDataRef = *ViewData.Pin();

	/*
	 * A Revealed cell can either be:
	 * 1) a cell showing a number
	 * 2) a hidden cell because there were no adjacent bombs
	 * 3) an exploded bomb (game over!)
	 * In each case, it's disabled
	 * If it's not revealed, we craft a button instead
	 */

	// While this might look hacky, we are mimicking the style of a disabled button for consistency across our "default look" grid, without having to do heavy style setup
	const FButtonStyle* BorderButtonCamouflage = &FCoreStyle::Get().GetWidgetStyle<FButtonStyle>("Button");
	
	if (ViewDataRef.RevealedCells[InCellIndex])
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
				ViewDataRef.MineCells[InCellIndex]
					? FLinearColor::Red
					: FLinearColor::Black
				)
			.VAlign(VAlign_Center)
			.HAlign(HAlign_Center)
			[
				// We only create the text widget if there is a text to display, using the SNullWidget trick
				ViewDataRef.CellNeighbourCounts[InCellIndex] > 0
				? SNew(STextBlock)
				.SimpleTextMode(true)
				.Visibility(EVisibility::HitTestInvisible)
				.Text(FText::AsNumber(ViewDataRef.CellNeighbourCounts[InCellIndex]))
				: SNullWidget::NullWidget
			];
	}

	// We use lambda to capture cell index and avoid extra code (e.g. SButton subclass to store index or index from XY)
	return SNew(SButton)
			.OnClicked_Lambda([this, InCellIndex]
			{
				OnCellPressedDelegate.Broadcast(InCellIndex);
				return FReply::Handled();
			});
}

void SSlateSweeperGameView::PopulateGrid()
{
	const FSlateSweeperGridData& ViewDataRef = *ViewData.Pin();

	for (uint8 Row = 0; Row < ViewDataRef.GridHeight; Row++)
	{
		for (uint8 Column = 0; Column < ViewDataRef.GridWidth; Column++)
		{
			int32 CellIndex = Row * ViewDataRef.GridWidth + Column;
			
			GridPanel->AddSlot(Column, Row)
			[
				CraftGridCell(CellIndex)
			];
		}
	}
}

void SSlateSweeperGameView::Construct(const FArguments& InArgs)
{
	ViewData = InArgs._ViewData;


	if (!ViewData.IsValid())
	{
		UE_LOG(LogSlateSweeper, Error, TEXT("Failed to create SlateSweeper View. ViewData invalid"));
		return;
	}

	const FSlateSweeperGridData& ViewDataRef = *ViewData.Pin();

	if (ViewDataRef.GridWidth == 0 || ViewDataRef.GridHeight == 0)
	{
		UE_LOG(LogSlateSweeper, Error, TEXT("Failed to create SlateSweeper View. Grid size invalid"));
		return;
	}

	const USlateSweeperEditorSettings* GeneralSettings = GetDefault<USlateSweeperEditorSettings>();
	float DesiredCellSize = GeneralSettings ? GeneralSettings->DesiredCellSize : 25.f;

	GridPanel = SNew(SUniformGridPanel)
				.MinDesiredSlotHeight(DesiredCellSize)
				.MinDesiredSlotWidth(DesiredCellSize);

	PopulateGrid();

	// Don't let the grid stretch, even when dimensions aren't 1:1
	float AspectRatio = static_cast<float>(ViewDataRef.GridWidth) /  static_cast<float>(ViewDataRef.GridHeight);
	SBox::Construct(
		SBox::FArguments()
		.MaxAspectRatio(AspectRatio)
		.MinAspectRatio(AspectRatio)
		.VAlign(VAlign_Top)
		.HAlign(HAlign_Left)
		[
			/*
			 * InvalidationPanel offers a quick but effective performance boost
			 * Idle 100x100 grid sees 15 ms sliced off, on a Ryzen 9 CPU
			 * This is not to be taken as a replacement for profiling and optimisation work
			 * Which would require a completely different approach
			 */ 
			SNew(SInvalidationPanel)
			[
				SNew(SBorder)
				[
					GridPanel.ToSharedRef()
				]
			]
		]
	);
	
}

void SSlateSweeperGameView::Update(const TWeakPtr<const FSlateSweeperGridData>& NewData)
{
	if (!NewData.IsValid())
	{
		UE_LOG(LogSlateSweeper, Error, TEXT("Failed to update SlateSweeper View. ViewData invalid"));
		return;
	}
	
	/*
	 * I found that repopulating a 100x100 grid eats about 20 ms on my Ryzen 9 7940HS
	 * So optimisation would be unnecessary for the scope of this project.
	 * But I would probably start from here.
	 */
	ViewData = NewData;
	GridPanel->ClearChildren();
	PopulateGrid();
}
