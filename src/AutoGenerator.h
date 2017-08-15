#pragma once
#include "DemoLayout.h"
#include "ClassicalLayout.h"
#include "MachineLearningLayout.h"
#include "BaseMaterial.h"

class AutoGenerator {
public:
	AutoGenerator(string layoutMethod);
	virtual ~AutoGenerator();
	
	void Generation();

	void SetResultWidth(int width);
	void SetResultHeight(int height);
	int GetResultWidth();
	int GetResultHeight();

private:
	int resultWidth;
	int resultHeight;

	Layout* layout;
	BaseMaterial* material;
	Results* results;
};