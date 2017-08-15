#pragma once
#include "ofImage.h"
#include "DemoTemplate.h"
#include <string>

using namespace std;

class Results {
public:
	virtual ~Results();

	static Results* Instance();

	void SetResult(string text);
	string GetResult();
	void SetResultPath(string path);
	string GetResultPath();
	void SetResultImg(ofImage img);
	void GrabResult(float w, float h);
	ofImage GetResultImg();

	void SetTemplate(Template* temp);
	void ClearTemplate();
	Template* GetTemplate();

	void SetPosition(float x, float y);
	ofPoint GetPosition();

	void Draw();
	void Draw(float x, float y);

protected:
	Results();

private:
	static Results* instance;

	string result;
	string resultPath;
	ofImage resultImage;

	Template* myTemplate;

	float drawX, drawY;
};