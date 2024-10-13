#ifndef EASTER_H
#define EASTER_H

#include "WindowBase.h"

class Easter : public WindowBase
{
public:
	Easter() = default;
	~Easter() = default;

private:
	virtual void Init() final;
	virtual void Update() final;

	bool showDemoWindow = {};
	bool showAnotherWindow = {};
};

#endif