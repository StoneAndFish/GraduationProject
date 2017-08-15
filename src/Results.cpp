#include "Results.h"

Results::~Results()
{
	ClearTemplate();
}

Results * Results::Instance()
{
	return instance;
}

void Results::SetResult(string text)
{
	result = text;
}

string Results::GetResult()
{
	return result;
}

void Results::SetResultPath(string path)
{
	resultPath = path;
}

string Results::GetResultPath()
{
	return resultPath;
}

void Results::SetResultImg(ofImage img)
{
	resultImage = img;
}

void Results::GrabResult(float w, float h)
{
	resultImage.grabScreen(drawX, drawY, w, h);
}

ofImage Results::GetResultImg()
{
	return resultImage;
}

void Results::SetTemplate(Template * temp)
{
	myTemplate = temp;
}

void Results::ClearTemplate()
{
	if (myTemplate != NULL)
	{
		delete myTemplate;
		myTemplate = NULL;
	}
}

Template * Results::GetTemplate()
{
	return myTemplate;
}

void Results::SetPosition(float x, float y)
{
	drawX = x;
	drawY = y;
}

ofPoint Results::GetPosition()
{
	return ofPoint(drawX, drawY);
}

void Results::Draw()
{
	Draw(drawX, drawY);
}

void Results::Draw(float x, float y)
{
	if (myTemplate != NULL)
	{
		myTemplate->Draw(x, y);
	}
}

Results::Results()
{
	myTemplate = NULL;
}

Results* Results::instance = new Results();