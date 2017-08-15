#include "TemplateRegion.h"

TemplateRegion::TemplateRegion()
{
	font = new ofxFontStash();
}

TemplateRegion::~TemplateRegion()
{
	if (font != NULL)
	{
		delete font;
		font = NULL;
	}
}

void TemplateRegion::LoadFontStash(string fontPath)
{
	font->setup(fontPath, 1.f, 1024, false, 8, 1.5f);
}

void TemplateRegion::SetPosition(float x, float y)
{
	tX = x;
	tY = y;
}

void TemplateRegion::SetWidth(float width)
{
	tWidth = width;
}

void TemplateRegion::SetHeight(float height)
{
	tHeight = height;
}

void TemplateRegion::SetFontSizeRange(int min, int max)
{
	minFontSize = min;
	maxFontSize = max;
}

void TemplateRegion::SetAlign(ofAlignHorz aln)
{
	align = aln;
}

void TemplateRegion::SetRegionType(RegionType rType)
{
	type = rType;
}

ofPoint TemplateRegion::GetPosition()
{
	return ofPoint(tX, tY);
}

float TemplateRegion::GetTX()
{
	return tX;
}

float TemplateRegion::GetTY()
{
	return tY;
}

float TemplateRegion::GetWidth()
{
	return tWidth;
}

float TemplateRegion::GetHeight()
{
	return tHeight;
}

int TemplateRegion::GetMinSize()
{
	return minFontSize;
}

int TemplateRegion::GetMaxSize()
{
	return maxFontSize;
}

ofxFontStash* TemplateRegion::GetFontStash()
{
	return font;
}

ofAlignHorz TemplateRegion::GetAlign()
{
	return align;
}

RegionType TemplateRegion::GetRegionType()
{
	return type;
}

void TemplateRegion::PutOriSentence(string sentence)
{
	oriTextList.push_back(sentence);
}

void TemplateRegion::SetOriSentence(vector<string> sList)
{
	oriTextList.assign(sList.begin(), sList.end());
}

void TemplateRegion::ClearOriSentence()
{
	if (!oriTextList.empty())
	{
		oriTextList.clear();
	}
}

vector<string> TemplateRegion::GetOriSentenceList()
{
	return oriTextList;
}

string TemplateRegion::GetOriSentence(int index)
{
	if(oriTextList.size() != 0)
	{
		return oriTextList.at(index);
	}
	else
	{
		return "";
	}
}

void TemplateRegion::PutText(string text)
{
	textList.push_back(text);
}

void TemplateRegion::SetTextList(vector<string> tList)
{
	textList.assign(tList.begin(), tList.end());
}

void TemplateRegion::ClearText()
{
	if (!textList.empty())
	{
		textList.clear();
	}
}

vector<string> TemplateRegion::GetTextList()
{
	return textList;
}

string TemplateRegion::GetText(int index)
{
	return textList.at(index);
}

void TemplateRegion::PutContour(string key, TextContour contour)
{
	contourMap[key] = contour;
}

void TemplateRegion::SetContourMap(map<string, TextContour> cMap)
{
	contourMap = cMap;
}

void TemplateRegion::ClearContourMap()
{
	if (!contourMap.empty())
	{
		contourMap.clear();
	}
}

map<string, TextContour> TemplateRegion::GetContourMap()
{
	return contourMap;
}

TextContour TemplateRegion::GetContour(string key)
{
	return contourMap.at(key);
}

void TemplateRegion::UpdateBest(int size)
{
	bestTextList.assign(textList.begin(), textList.end());
	bestContourMap = contourMap;
	bestFontSize = size;
}

void TemplateRegion::DrawRegion(vector<ofColor> colorList, float oriX, float oriY)
{
	for (int i = 0; i < bestTextList.size(); i++)
	{
		ofPushMatrix();
		
		ofSetColor(colorList.at(i % colorList.size()));
		string tempText = bestTextList.at(i);
		ofPoint tempPoint = bestContourMap[tempText].GetPosition();
		ofRectangle rect = font->drawMultiLine(tempText, bestFontSize, tempPoint.x + oriX, tempPoint.y + oriY, align, 0.0f);
		ofSetColor(ofColor::white);

		ofPopMatrix();
	}
}
