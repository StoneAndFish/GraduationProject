#pragma once
#include "Layout.h"
#include "Results.h"
#include "ofImage.h"

class DemoLayout : public Layout {
public: 
	DemoLayout();
	virtual ~DemoLayout();

	virtual void BeginLayout(float width, float height, BaseMaterial *m, Results *r);

protected:
	void Typography(float width, float height, IplImage *sMap);
	void InitTemplate(float w, float h);
	float MinimizeEnergy(IplImage *sMap);
	float MinimizeEnergy_Masthead(TemplateRegion *region, IplImage *sMap, float freeSpace);
	float MinimizeEnergy_Coverlines(TemplateRegion *region, IplImage *sMap, float freeSpace);
	float MinimizeEnergy_Byline(TemplateRegion *region, IplImage *sMap, float freeSpace);
	float ComputeInstrutionEnergy(TemplateRegion *region, IplImage *sMap);
	float ComputeUtilizationEnergy(TemplateRegion *region, IplImage *sMap);

	void Split(const string &input, vector<string> &output, const string &c);
	string WordMerge(const vector<string> word, int start, int end);

private:
	BaseMaterial* material;
	Results* results;

	DemoTemplate* demoTemp;
};