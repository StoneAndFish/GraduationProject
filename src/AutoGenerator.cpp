#include "AutoGenerator.h"

AutoGenerator::AutoGenerator(string layoutMethod)
{
	if (!strcmp(layoutMethod.c_str(), "ClassicalLayout"))
	{
		layout = new ClassicalLayout();
	}
	else if (!strcmp(layoutMethod.c_str(), "MachineLearningLayout"))
	{
		layout = new MachineLearningLayout();
	}
	else if (!strcmp(layoutMethod.c_str(), "DemoLayout"))
	{
		layout = new DemoLayout();
	}
	else
	{
		layout = NULL;
	}

	material = BaseMaterial::Instance();
	results = Results::Instance();
}

AutoGenerator::~AutoGenerator()
{
	if (layout != NULL)
	{
		delete layout;
		layout = NULL;
	}
}

void AutoGenerator::Generation()
{
	if(layout != NULL)
	{
		layout->BeginLayout((float)resultWidth, (float)resultHeight, material, results);
	}
	else
	{
		
	}
}

void AutoGenerator::SetResultWidth(int width)
{
	resultWidth = width;
}

void AutoGenerator::SetResultHeight(int height)
{
	resultHeight = height;
}

int AutoGenerator::GetResultWidth()
{
	return resultWidth;
}

int AutoGenerator::GetResultHeight()
{
	return resultHeight;
}
