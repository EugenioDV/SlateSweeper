// This is a technical test from Eugenio Del Vecchio for Geotech, please do not share.

#pragma once
#include "Widgets/Layout/SBox.h"
 
class SSlateSweeperMinefield : public SBox //todo figure out if sbox is the best, sstackablebox seems like best but it's actually hard, sboxpanel is what vertical and horizontal boxes are derived from so perhaps I could use that
{
public:
	
	SLATE_BEGIN_ARGS(SSlateSweeperMinefield)
	{}
	SLATE_END_ARGS()
	
	void Construct(const FArguments& InArgs);	
};