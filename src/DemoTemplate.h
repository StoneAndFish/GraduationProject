#pragma once
#include "Template.h"

class DemoTemplate : public Template {
public :
	DemoTemplate();
	virtual ~DemoTemplate();

	virtual void InitTemplate(float w, float h);

	void PutMasthead(string text);
	void PutLeftCoverlines(string text);
	void PutRightCoverlines(string text);
	void PutByline(string text);

	TemplateRegion* GetMastheadRegion();
	TemplateRegion* GetLeftCoverlinesRegion();
	TemplateRegion* GetRightCoverlinesRegion();
	TemplateRegion* GetBylineRegion();

	void SetFirstColor(vector<ofColor> colorList);
	void SetSecondColor(vector<ofColor> colorList);

	virtual void Draw(float x, float y);

private:
	vector<ofColor> colorList1;
	vector<ofColor> colorList2;

	TemplateRegion mastheadRegion;
	TemplateRegion leftCoverLinesRegion;
	TemplateRegion rightCoverLinesRegion;
	TemplateRegion bylineRegion;
};