#pragma once
#include "TemplateRegion.h"

class Template {
public:
	virtual ~Template();

	virtual void InitTemplate(float w, float h) = 0;
	virtual void Draw(float x, float y) = 0;

	virtual void SetFirstColor(vector<ofColor> colorList) = 0;
	virtual void SetSecondColor(vector<ofColor> colorList) = 0;

protected:
	Template();
};