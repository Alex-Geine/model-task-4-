#include "pch.h"
#include "Drawer.h"


#define _USE_MATH_DEFINES
#include  <math.h>

IMPLEMENT_DYNAMIC(Drawer, CStatic)

Drawer::Drawer()
{	
}

Drawer::~Drawer()
{	
}


BEGIN_MESSAGE_MAP(Drawer, CStatic)

END_MESSAGE_MAP()

void Drawer::GetContr(Controller* control) {
	
	this->control = control;
}

void Drawer::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{	
	if (draw) {
		if (drawerID == 1)
			control->DrawMainGr(lpDrawItemStruct);
		if (drawerID == 2)
			control->Draw3D(lpDrawItemStruct);
		if(drawerID == 3)
			control->DrawSpectrum(lpDrawItemStruct);
	}	
	
}



