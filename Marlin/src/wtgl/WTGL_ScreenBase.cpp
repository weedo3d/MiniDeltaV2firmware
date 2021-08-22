/**
* Copyright (C) 2021 WEEDO3D Perron
*/

#include "WTGL_ScreenBase.h"
#include "WTGL_Manager.h"

void WTGL_ScreenBase::Goback()
{
	if (parent != NULL)
	{
		activated = false;
		wtgl.GotoScreen(parent);
	}
}