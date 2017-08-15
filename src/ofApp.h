#pragma once

#include "ofMain.h"
#include "ofxImGui.h"
#include "BaseMaterial.h"
#include "Results.h"
#include "AutoGenerator.h"

#ifdef MEASURE_PERFORMANCE
	#include "ofxTimeMeasurements.h"
#else
	#define TIME_SAMPLE_START ;
	#define TIME_SAMPLE_STOP ;
#endif

#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void ShowSetting(float x, float y, float width, float height);
		void ShowInputOperation(float x, float y, float width, float height);
		void ShowResults(float x, float y, float width, float height);
		void ShowOutputOperation(float x, float y, float width, float heght);

		ofxImGui::Gui gui;

		ImVec4 backgroundColor;
		bool windowShowFlag;

		float windowWidth;
		float windowHeight;
		float settingWidth, settingHeight;
		float iOprWidth, iOprHeight;
		float oPreWidth, oPreHeight;
		float oOprWidth, oOprHeight;
		float leftOffsetX, rightOffsetX;
		float topOffsetY, bottomOffsetY;
		int targetWidth, targetHeight;

		int windowFlags;
		bool imageFlag;

		string backImgPath;
		string sMapPath;
		string sOutputPath;

		BaseMaterial* material;
		Results* results;

		double spendTime;
		ImVec4 firstColor;
		ImVec4 secondColor;
		ImVec4 thirdColor;
};
