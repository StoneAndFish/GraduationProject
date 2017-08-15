#pragma once
#include "Layout.h"
#include "Results.h"
#include "ofImage.h"
#include "octree.h"

#define MAX_COLOR 1024

class ClassicalLayout : public Layout {
public :
	ClassicalLayout();
	virtual ~ClassicalLayout();

	virtual void BeginLayout(float width, float height, BaseMaterial *m, Results *r);

protected:
	void ImageComposition(float width, float height);

	void Typography(float width, float height, IplImage *sMap);
	void InitTemplate(float w, float h);
	float MinimizeEnergy(IplImage *sMap);
	float MinimizeEnergy_Masthead(TemplateRegion *region, IplImage *sMap, float freeSpace);
	float MinimizeEnergy_Coverlines(TemplateRegion *region, IplImage *sMap, float freeSpace);
	float MinimizeEnergy_Byline(TemplateRegion *region, IplImage *sMap, float freeSpace);
	float ComputeInstrutionEnergy(TemplateRegion *region, IplImage *sMap);
	float ComputeUtilizationEnergy(TemplateRegion *region, IplImage *sMap);
	
	void ColorDesign(IplImage *oriImage);
	void ColorDesignByImg(IplImage *oriImage);
	ofColor GetMainColorAvg(IplImage *oriImage);
	ofColor GetMainColorOcTree(IplImage *oriImage);
	void ColorDesignByRegion(IplImage *oriImage);
	ofColor GetMainColorOcTree(int pixCount, uchar* uImage, int index = 0);

	void Split(const string &input, vector<string> &output, const string &c);
	string WordMerge(const vector<string> word, int start, int end);

private:
	BaseMaterial* material;
	Results* results;

	ofxCvColorImage tempOri;
	ofxCvGrayscaleImage tempSMap;
	IplImage *ori;
	IplImage *smap;

	DemoTemplate* demoTemp;
};