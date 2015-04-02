#pragma once

#include "../common/SDLWindowBase.h"

class Draw1pxWindow : public SDLWindowBase
{
public:
	Draw1pxWindow();
	virtual void draw();
};

