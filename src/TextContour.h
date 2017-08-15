#pragma once
#include <iostream>
#include "ofxFontStash.h"
#include <vector>

using namespace std;

class TextContour {
public:
	TextContour();
	~TextContour();

	void SetFont(ofxFontStash *fontStash);
	void SetAlign(ofAlignHorz aln);
	void SetFontSize(int size);
	void SetCoord(float xx, float yy);
	void SetBound(float minx, float miny, float maxx, float maxy);

	void PutText(string text);
	string GetText();
	void ClearText();

	bool ComputeContour();
	vector<ofRectangle> GetContour();

	ofPoint GetPosition();

	bool JudgeBound(ofRectangle rect);

private:
	vector<string> textVector;
	vector<ofRectangle> contour;

	ofxFontStash *font;
	ofAlignHorz align;
	int fontSize;

	float x, y;
	float drawX, drawY;
	float minX, minY, maxX, maxY;
};