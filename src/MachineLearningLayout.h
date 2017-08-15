#pragma once
#include "Layout.h"

class MachineLearningLayout : public Layout {
public:
	MachineLearningLayout();
	virtual ~MachineLearningLayout();

	virtual void BeginLayout(float width, float height, BaseMaterial *m, Results *r);
};