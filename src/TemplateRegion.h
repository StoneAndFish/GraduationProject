#pragma once
#include "TextContour.h"
#include "ofxOpencv.h"
#include "opencv2/highgui/highgui.hpp"
#include <map>
#include <time.h>

enum RegionType { Masthead, CoverLines, Byline };

class TemplateRegion {
public:
	TemplateRegion();
	~TemplateRegion();

	void LoadFontStash(string fontPath);
	void SetPosition(float x, float y);
	void SetWidth(float width);
	void SetHeight(float height);
	void SetFontSizeRange(int min, int max);
	void SetAlign(ofAlignHorz aln);
	void SetRegionType(RegionType rType);

	ofPoint GetPosition();
	float GetTX();
	float GetTY();
	float GetWidth();
	float GetHeight();
	int GetMinSize();
	int GetMaxSize();
	ofxFontStash* GetFontStash();
	ofAlignHorz GetAlign();
	RegionType GetRegionType();

	void PutOriSentence(string sentence);
	void SetOriSentence(vector<string> sList);
	void ClearOriSentence();
	vector<string> GetOriSentenceList();
	string GetOriSentence(int index);

	void PutText(string text);
	void SetTextList(vector<string> tList);
	void ClearText();
	vector<string> GetTextList();
	string GetText(int index);

	void PutContour(string key, TextContour contour);
	void SetContourMap(map<string, TextContour> cMap);
	void ClearContourMap();
	map<string, TextContour> GetContourMap();
	TextContour GetContour(string key);

	void UpdateBest(int size);

	void DrawRegion(vector<ofColor> colorList, float oriX, float oriY);

private:
	float tX, tY;
	float tWidth, tHeight;
	int minFontSize, maxFontSize;
	ofxFontStash *font;
	ofAlignHorz align;
	RegionType type;

	vector<string> oriTextList;

	vector<string> textList;
	map<string, TextContour> contourMap;

	vector<string> bestTextList;
	map<string, TextContour> bestContourMap;
	int bestFontSize;
};