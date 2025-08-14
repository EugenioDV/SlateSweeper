// This is a technical test from Eugenio Del Vecchio for Geotech, please do not share.
#include "SSlateSweeperMinefield.h"

#include "Widgets/Layout/SUniformGridPanel.h"

void SSlateSweeperMinefield::Construct(const FArguments& InArgs) //todo this looks weird
{
	uint8 Width = 20; //every row will have this number of widgets
	uint8 Height = 17; //at this many rows we stop adding
	
	TSharedRef<SUniformGridPanel> GridPanel =
		SNew(SUniformGridPanel)
		.MinDesiredSlotHeight(25.f) //todo standard for cell size?
		.MinDesiredSlotWidth(25.f);

	for (uint8 row = 0; row < Height; row++)
	{
		for (uint8 col = 0; col < Width; col++)
		{
			GridPanel->AddSlot(col, row)
			[
				SNew(SButton)
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
