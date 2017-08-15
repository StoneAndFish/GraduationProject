#pragma once
#include "ofxOpenCv.h"
#include <vector>
#include <map>

using namespace std;

class BaseMaterial {
public:
	virtual ~BaseMaterial();
	static BaseMaterial* Instance();

	// we just save the path of inputImage
	void SetImagePath(string imagePath);
	string GetImagePath();
	void SetSMapPath(string sPath);
	string GetSMapPath();

	void SetTextList(vector<string> textList);
	void PushText(string text);
	string PopText();
	string PopTextWithNum();
	int GetTextListSize();
	string GetTextOfIndex(int index);
	vector<string> GetTextList();


	void SetWeightMap(map<string, float> weightMap);
	void PutTextWeight(string text, float weight);
	void PutTextWeightWithNum(string text, float weight);
	void RemoveTextWeight(string text);
	float GetTextWeight(string text);
	map<string, float> GetWeightMap();

	bool loadAllImage();
	void SetOriImg(ofxCvColorImage oriImg);
	void SetSaliencyMap(ofxCvGrayscaleImage sMap);
	void ClearOriImg();
	void ClearSaliencyMap();
	ofxCvColorImage GetOriImg();
	ofxCvGrayscaleImage GetSaliencyMap();

	void SetReady(bool ready);
	bool IsReady();

protected:
	BaseMaterial();

private:
	static BaseMaterial* instance;

	string backgroundImagePath;
	string saliencyMapPath;
	vector<string> relatedWords;
	map<string, float> textWeight;

	ofxCvColorImage oriImage;
	ofxCvGrayscaleImage saliencyMap;

	bool isReady;
};