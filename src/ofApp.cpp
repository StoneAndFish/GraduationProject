#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofEnableAlphaBlending();
	ofSetVerticalSync(true);
	ofSetFrameRate(60);

	windowHeight = (ofGetScreenHeight() * 0.90);
	windowWidth = (ofGetScreenHeight() * 0.80 / 3.0 * 4);
	settingHeight = oPreHeight = (windowHeight * 0.70);
	settingWidth = oPreWidth = (windowWidth * 0.45);
	iOprHeight = oOprHeight = (windowHeight * 0.25);
	iOprWidth = oOprWidth = (windowWidth * 0.45);

	leftOffsetX = 0.04;
	rightOffsetX = 0.51;
	topOffsetY = 0.02;
	bottomOffsetY = 0.73;

	targetWidth = windowWidth * 0.45;
	targetHeight = targetWidth / 8.5 * 11.0;

	windowFlags |= ImGuiWindowFlags_NoMove;
	windowFlags |= ImGuiWindowFlags_NoCollapse;

	imageFlag = true;

	material = BaseMaterial::Instance();
	results = Results::Instance();

	spendTime = .0f;

	firstColor = ofColor::red;
	secondColor = ofColor::orangeRed;
	thirdColor = ofColor::orange;

	ofSetWindowShape(windowWidth, windowHeight);
	ofSetWindowTitle("GraduationProject");

	gui.setup();

	ImGui::GetIO().MouseDrawCursor = false;
	backgroundColor = ofColor(160, 200, 255);
	windowShowFlag = false;
}

void ofApp::update(){
	windowHeight = (ofGetScreenHeight() * 0.90);
	windowWidth = (ofGetScreenHeight() * 0.80 / 3.0 * 4);
	ofSetWindowShape(windowWidth, windowHeight);

	settingHeight = oPreHeight = (windowHeight * 0.70);
	settingWidth = oPreWidth = (windowWidth * 0.45);
	iOprHeight = oOprHeight = (windowHeight * 0.25);
	iOprWidth = oOprWidth = (windowWidth * 0.45);
	results->SetPosition(windowWidth * rightOffsetX, windowHeight * topOffsetY);
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofSetBackgroundColor(backgroundColor);

	ShowResults(results->GetPosition().x, results->GetPosition().y, targetWidth, targetHeight);

	gui.begin();
	{
		ShowSetting(windowWidth * leftOffsetX, windowHeight * topOffsetY, settingWidth, settingHeight);
		ShowInputOperation(windowWidth * leftOffsetX, windowHeight * bottomOffsetY, iOprWidth, iOprHeight);
		ShowOutputOperation(windowWidth * rightOffsetX, windowHeight * bottomOffsetY, oOprWidth, oOprHeight);
	}
	gui.end();
}


void ofApp::ShowSetting(float x, float y, float width, float height)
{
	ImGui::SetNextWindowPos(ImVec2(x, y));
	ImGui::SetNextWindowSize(ImVec2(width, height));
	ImGui::Begin("Settings && Preview", &windowShowFlag, windowFlags);

	ImGui::Text("Background color setting:");
	ImGui::ColorEdit3("Background", (float*)&backgroundColor);
	
	ImGui::Text("Set 1st color:");
	ImGui::SameLine();
	if (ImGui::Button("set-1"))
	{
		vector<ofColor> colorList;
		colorList.push_back(firstColor);
	
		Template* temp = results->GetTemplate();
		if (temp != NULL)
		{
			temp->SetFirstColor(colorList);
		}
	}
	ImGui::ColorEdit3("1st color", (float*)&firstColor);
	ImGui::Text("Set 2nd and 3rd color:");
	ImGui::SameLine();
	if (ImGui::Button("set-2"))
	{
		vector<ofColor> colorList;
		colorList.push_back(secondColor);
		colorList.push_back(thirdColor);

		Template* temp = results->GetTemplate();
		if (temp != NULL)
		{
			temp->SetSecondColor(colorList);
		}
	}
	ImGui::ColorEdit3("2nd color", (float*)&secondColor);
	ImGui::ColorEdit3("3rd color", (float*)&thirdColor);

	ImGui::Text("Trans between saliencyMap and backImage:");
	if (ImGui::Button("Change"))
	{
		imageFlag = !imageFlag;
	}

	ImGui::Text("\nWe'll use the following paragraph : ");
	int index = 0;
	int size = material->GetTextListSize();
	for (; index < size; index++)
	{
		string tempStr = material->GetTextOfIndex(index);
		ImGui::Text("%d. %s (Weight : %.3f)", index, (tempStr).c_str(), material->GetTextWeight(to_string(index) + "." + tempStr));
	}

	ImGui::Text("\nUse time of layout : %.3f s", spendTime);

	ImGui::End();
}

void ofApp::ShowInputOperation(float x, float y, float width, float height)
{
	ImGui::SetNextWindowPos(ofVec2f(x, y));
	ImGui::SetNextWindowSize(ofVec2f(width, height));
	ImGui::Begin("Operation about input", &windowShowFlag, windowFlags);

	ImGui::Text("Input the relevant text and weight :");
	static float fBuf = 1.0f;
	ImGui::InputFloat("Weight", &fBuf, 0.0f, 0.0, 3);
	static char buf[256] = "";
	ImGui::InputText("Text", buf, IM_ARRAYSIZE(buf));
	if (ImGui::Button("Add Text"))
	{
		if (strcmp(buf, ""))
		{
			string tempBuf(buf);
			material->PushText(tempBuf);
			material->PutTextWeightWithNum(tempBuf, fBuf);
			buf[0] = '\0';
			fBuf = 1.0f;
		}
	}
	if (ImGui::Button("Delete Last"))
	{
		string temp;
		temp = material->PopTextWithNum();
		material->RemoveTextWeight(temp);
	}

	ImGui::Text("\nImport background image :");
	ImGui::Text("(Note that the path only in English)");
	if (ImGui::Button("bPath"))
	{
		ofFileDialogResult loadDialog = ofSystemLoadDialog("open", false, "../../data/img");
		if (!loadDialog.bSuccess)
		{
		}
		else
		{
			backImgPath = loadDialog.getPath();
		}
	}
	ImGui::SameLine();
	ImGui::InputText("(bPath)", (char*)backImgPath.c_str(), IM_ARRAYSIZE(backImgPath.c_str()));
	ImGui::Text("Import saliency map :");
	if (ImGui::Button("sPath"))
	{
		ofFileDialogResult loadDialog = ofSystemLoadDialog("open", false, "../../data/img");
		if (!loadDialog.bSuccess)
		{
		}
		else
		{
			sMapPath = loadDialog.getPath();
		}
	}
	ImGui::SameLine();
	ImGui::InputText("(sPath)", (char*)sMapPath.c_str(), IM_ARRAYSIZE(sMapPath.c_str()));
	if (ImGui::Button("Import File"))
	{
		material->SetImagePath(backImgPath);
		material->SetSMapPath(sMapPath);
		material->SetReady(material->loadAllImage());
	}

	ImGui::SameLine();
	if (ImGui::Button("Remove Path"))
	{
		backImgPath = "";
		sMapPath = "";
	}
	ImGui::SameLine();
	if (ImGui::Button("Remove Image"))
	{
		material->SetImagePath("");
		material->ClearOriImg();
		material->SetSMapPath("");
		material->ClearSaliencyMap();
		material->SetReady(false);
	}

	ImGui::End();
}

void ofApp::ShowResults(float x, float y, float width, float height)
{
	if (imageFlag)
	{
		material->GetOriImg().draw(x, y, width, height);
	}
	else
	{
		material->GetSaliencyMap().draw(x, y, width, height);
	}

	results->Draw(x, y);
}

void ofApp::ShowOutputOperation(float x, float y, float width, float height)
{
	ImGui::SetNextWindowPos(ofVec2f(x, y));
	ImGui::SetNextWindowSize(ofVec2f(width, height));
	ImGui::Begin("Operation about output", &windowShowFlag, windowFlags);
	ImGui::Text("Target size : ");
	ImGui::InputInt("Width ", &targetWidth);
	ImGui::InputInt("Height ", &targetHeight);
	const char* layoutMethod[] = { "DemoLayout", "ClassicalLayout", "MachineLearningLayout" };
	static int item = 0;
	ImGui::Combo("Method", &item, layoutMethod, IM_ARRAYSIZE(layoutMethod));
	if (ImGui::Button("Start Layout"))
	{
		clock_t start, stop;
		start = clock();

		results->ClearTemplate();

		if (material->IsReady())
		{
			AutoGenerator generator(layoutMethod[item]);
			generator.SetResultHeight(targetHeight);
			generator.SetResultWidth(targetWidth);
			generator.Generation();
		}

		stop = clock();

		spendTime = (double)(stop - start) / CLOCKS_PER_SEC;
	}

	ImGui::Text("\nExport the current result to this path :");
	ImGui::Text("(Note that the path only in English)");
	if (ImGui::Button("Path"))
	{
		ofFileDialogResult saveDialog = ofSystemSaveDialog("*.jpg", "export");
		if (!saveDialog.bSuccess)
		{
		}
		else
		{
			sOutputPath = saveDialog.getPath();
		}
	}
	ImGui::SameLine();
	ImGui::InputText("(oPath)", (char*)sOutputPath.c_str(), IM_ARRAYSIZE(sOutputPath.c_str()));

	if (ImGui::Button("Export File"))
	{
		results->GrabResult(targetWidth, targetHeight);
		results->GetResultImg().save(sOutputPath);
	}
	ImGui::SameLine();
	if (ImGui::Button("Remove Path"))
	{
		sOutputPath = "";
	}
	ImGui::SameLine();
	if (ImGui::Button("Remove Result"))
	{
		results->SetResult("");
		results->ClearTemplate();
	}

	ImGui::End();
}
