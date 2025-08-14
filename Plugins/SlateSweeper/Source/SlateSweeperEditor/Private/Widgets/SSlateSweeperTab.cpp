// This is a technical test from Eugenio Del Vecchio for Geotech, please do not share.

#include "SSlateSweeperTab.h"
#include "Widgets/Input/SSpinBox.h"
#include "SSlateSweeperMinefield.h"
#include "Widgets/Layout/SScaleBox.h"


#define LOCTEXT_NAMESPACE "FSlateSweeperEditorModule"


//TODO style
/*
 *Access editor row width
 *text size
 *highlight color
 *default background color
 *padding values (won't be easy since they use indentation widgets)
 *perhaps a spliter or a drop down thing for the menu so that there is more space for playing when collapsed
 * check if they do the separation thing in ndisplay widgets aka SNew( SBox ) instead of SNew(SBox)
 *
 *
 *
 */

/* Minimalistic helper for clean and consistent-looking rows */
TSharedRef<SHorizontalBox> ConstructTabRow(const TSharedRef<SWidget>& LeftWidget, const TSharedRef<SWidget>& RightWidget)
{
	return SNew(SHorizontalBox)
		+SHorizontalBox::Slot()
		[
			SNew(SSplitter)
			+SSplitter::Slot()
			[
				LeftWidget
			]
			+SSplitter::Slot()
			[
				RightWidget
			]
		];
}

void SSlateSweeperTab::Construct(const FArguments& InArgs)
{
	SDockTab::Construct(
		SDockTab::FArguments()
		.TabRole(ETabRole::NomadTab) //todo not sure if tab role should be hardcoded here since stuff is quite scattered
		[
			SNew(SVerticalBox) 
			+SVerticalBox::Slot() //todo this is repeated = bad
			.VAlign(VAlign_Top)
			.AutoHeight()
			[
				ConstructTabRow
				(
					SNew(STextBlock)
					.Text(LOCTEXT("Dimensions", "Dimensions")),
					SNew(SHorizontalBox)
					+SHorizontalBox::Slot()
					.HAlign(HAlign_Left) //todo verify these or just refactor it all into a decent ConstructTabRow
					.AutoWidth()
					[
						SNew(STextBlock)
						.Text(LOCTEXT("Width", "Width"))
					]
					+SHorizontalBox::Slot()
					.HAlign(HAlign_Left)
					[
						SNew(SSpinBox<uint8>)
					]
					+SHorizontalBox::Slot()
					.HAlign(HAlign_Left)
					.AutoWidth()
					[
						SNew(STextBlock)
						.Text(LOCTEXT("Height", "Height"))
					]
					+SHorizontalBox::Slot()
					.HAlign(HAlign_Left)
					[
						SNew(SSpinBox<uint8>)
					]
				)
			]
			+SVerticalBox::Slot()
			.VAlign(VAlign_Top)
			.AutoHeight()
			[
				ConstructTabRow
				(
					SNew(STextBlock)
					.Text(LOCTEXT("NumberOfMines", "Number of Mines")),
					SNew(SSpinBox<uint8>)
				)
			]
			+SVerticalBox::Slot()
			.VAlign(VAlign_Top)
			.HAlign(HAlign_Left)
			.AutoHeight()
			[
				SNew(SButton)
				.Text(LOCTEXT("Start New Session", "Start New Session"))
			]
			+SVerticalBox::Slot()
			.VAlign(VAlign_Top)
			.HAlign(HAlign_Left)
			[
				SNew(SScaleBox)
				.Stretch(EStretch::ScaleToFit)
				[
					SNew(SSlateSweeperMinefield)
				]
			]
		]);
}


#undef  LOCTEXT_NAMESPACE
