#pragma once
#include "Results.h"
#include "BaseMaterial.h"
#include <map>

class Layout {
public:
	virtual ~Layout();

	virtual void BeginLayout(float width, float height, BaseMaterial *m, Results *r) = 0;

protected:
	Layout();
};