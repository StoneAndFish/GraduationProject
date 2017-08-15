#include "DemoTemplate.h"

DemoTemplate::DemoTemplate()
{
}

DemoTemplate::~DemoTemplate()
{
}

void DemoTemplate::InitTemplate(float w, float h)
{
	mastheadRegion.SetAlign(OF_ALIGN_HORZ_CENTER);
	mastheadRegion.SetFontSizeRange(10, 1024);
	mastheadRegion.SetRegionType(Masthead);

	leftCoverLinesRegion.SetAlign(OF_ALIGN_HORZ_LEFT);
	leftCoverLinesRegion.SetFontSizeRange(6, 36);
	leftCoverLinesRegion.SetRegionType(CoverLines);

	rightCoverLinesRegion.SetAlign(OF_ALIGN_HORZ_RIGHT);
	rightCoverLinesRegion.SetFontSizeRange(6, 36);
	rightCoverLinesRegion.SetRegionType(CoverLines);

	bylineRegion.SetAlign(OF_ALIGN_HORZ_CENTER);
	bylineRegion.SetFontSizeRange(8, 48);
	bylineRegion.SetRegionType(Byline);

	float tempW, tempH, tempX, tempY;
	tempX = w * 0.05;
	tempW = w * 0.440;
	tempY = h * 0.175;
	tempH = h * 0.660;
	leftCoverLinesRegion.SetPosition(tempX, tempY);
	leftCoverLinesRegion.SetHeight(tempH);
	leftCoverLinesRegion.SetWidth(tempW);

	tempX = w * 0.51;
	tempW = w * 0.440;
	tempY = h * 0.175;
	tempH = h * 0.660;
	rightCoverLinesRegion.SetPosition(tempX, tempY);
	rightCoverLinesRegion.SetHeight(tempH);
	rightCoverLinesRegion.SetWidth(tempW);

	tempX = w * 0.05;
	tempW = w * 0.90;
	tempY = h * 0.022;
	tempH = h * 0.133;
	mastheadRegion.SetPosition(tempX, tempY);
	mastheadRegion.SetHeight(tempH);
	mastheadRegion.SetWidth(tempW);

	tempX = w * 0.05;
	tempW = w * 0.90;
	tempY = h * 0.855;
	tempH = h * 0.133;
	bylineRegion.SetPosition(tempX, tempY);
	bylineRegion.SetHeight(tempH);
	bylineRegion.SetWidth(tempW);

	mastheadRegion.LoadFontStash("font/Times_New_Roman.ttf");
	leftCoverLinesRegion.LoadFontStash("font/Garamond_Macron_Bold.TTF");
	rightCoverLinesRegion.LoadFontStash("font/Garamond_Macron_Bold.TTF");
	bylineRegion.LoadFontStash("font/Times_New_Roman.ttf");

	vector<ofColor> cList1;
	cList1.push_back(ofColor::red);
	colorList1.swap(cList1);

	vector<ofColor> cList2;
	cList2.push_back(ofColor::orangeRed);
	cList2.push_back(ofColor::orange);
	colorList2.swap(cList2);
}

void DemoTemplate::PutMasthead(string text)
{
	mastheadRegion.PutOriSentence(text);
}

void DemoTemplate::PutLeftCoverlines(string text)
{
	leftCoverLinesRegion.PutOriSentence(text);
}

void DemoTemplate::PutRightCoverlines(string text)
{
	rightCoverLinesRegion.PutOriSentence(text);
}

void DemoTemplate::PutByline(string text)
{
	bylineRegion.PutOriSentence(text);
}

TemplateRegion * DemoTemplate::GetMastheadRegion()
{
	return &mastheadRegion;
}

TemplateRegion * DemoTemplate::GetLeftCoverlinesRegion()
{
	return &leftCoverLinesRegion;
}

TemplateRegion * DemoTemplate::GetRightCoverlinesRegion()
{
	return &rightCoverLinesRegion;
}

TemplateRegion * DemoTemplate::GetBylineRegion()
{
	return &bylineRegion;
}

void DemoTemplate::SetFirstColor(vector<ofColor> colorList)
{
	colorList1.assign(colorList.begin(), colorList.end());
}

void DemoTemplate::SetSecondColor(vector<ofColor> colorList)
{
	colorList2.assign(colorList.begin(), colorList.end());
}

void DemoTemplate::Draw(float x, float y)
{
	mastheadRegion.DrawRegion(colorList1, x, y);
	leftCoverLinesRegion.DrawRegion(colorList2, x, y);
	rightCoverLinesRegion.DrawRegion(colorList2, x, y);
	bylineRegion.DrawRegion(colorList2, x, y);
}
