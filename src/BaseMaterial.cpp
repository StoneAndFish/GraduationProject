#include "BaseMaterial.h"

BaseMaterial::BaseMaterial()
{
	isReady = false;
}

BaseMaterial* BaseMaterial::instance = new BaseMaterial();

BaseMaterial::~BaseMaterial()
{
}

BaseMaterial* BaseMaterial::Instance()
{
	return instance;
}

void BaseMaterial::SetImagePath(string imagePath)
{
	backgroundImagePath = imagePath;
}

void BaseMaterial::SetTextList(vector<string> textList)
{
	relatedWords.assign(textList.begin(), textList.end());
}

void BaseMaterial::PushText(string text)
{
	relatedWords.push_back(text);
}

string BaseMaterial::PopText()
{
	string temp;
	if (relatedWords.size() != 0)
	{
		temp = relatedWords.back();
		relatedWords.pop_back();
	}

	return temp;
}

string BaseMaterial::PopTextWithNum()
{
	string temp;
	if (relatedWords.size() != 0)
	{
		temp = relatedWords.back();
		temp = to_string(relatedWords.size() - 1) + "." + temp;
		relatedWords.pop_back();
	}

	return temp;
}

int BaseMaterial::GetTextListSize()
{
	return relatedWords.size();
}

void BaseMaterial::SetWeightMap(map<string, float> weightMap)
{
	textWeight = weightMap;
}

void BaseMaterial::PutTextWeight(string text, float weight)
{
	textWeight.insert(pair<string, float>(text, weight));
}

void BaseMaterial::PutTextWeightWithNum(string text, float weight)
{
	string temp;
	temp = to_string(relatedWords.size() - 1) + "." + text;
	PutTextWeight(temp, weight);
}

void BaseMaterial::RemoveTextWeight(string text)
{
	map<string, float>::iterator iter = textWeight.find(text);
	if (iter != textWeight.end())
	{
		textWeight.erase(iter);
	}
}

string BaseMaterial::GetImagePath()
{
	return backgroundImagePath;
}

void BaseMaterial::SetSMapPath(string sPath)
{
	saliencyMapPath = sPath;
}

string BaseMaterial::GetSMapPath()
{
	return saliencyMapPath;
}

vector<string> BaseMaterial::GetTextList()
{
	return relatedWords;
}

string BaseMaterial::GetTextOfIndex(int index)
{
	return relatedWords.at(index);
}

map<string, float> BaseMaterial::GetWeightMap()
{
	return textWeight;
}

bool BaseMaterial::loadAllImage()
{
	ofImage img;
	if (!img.load(backgroundImagePath))
	{
		std::cout << "Failed to load background image" << std::endl;
		return false;
	}

	oriImage.allocate(img.getWidth(), img.getHeight());
	oriImage.setFromPixels(img.getPixels());

	if (!img.load(saliencyMapPath))
	{
		std::cout << "Failed to load saliency map" << std::endl;
		return false;
	}

	saliencyMap.allocate(img.getWidth(), img.getHeight());
	saliencyMap.setFromPixels(img.getPixels());

	return true;
}

void BaseMaterial::SetOriImg(ofxCvColorImage oriImg)
{
	oriImage = oriImg;
}

void BaseMaterial::SetSaliencyMap(ofxCvGrayscaleImage sMap)
{
	saliencyMap = sMap;
}

void BaseMaterial::ClearOriImg()
{
	if (oriImage.bAllocated)
	{
		oriImage.clear();
	}
}

void BaseMaterial::ClearSaliencyMap()
{
	if (saliencyMap.bAllocated)
	{
		saliencyMap.clear();
	}
}

ofxCvColorImage BaseMaterial::GetOriImg()
{
	return ofxCvColorImage(oriImage);
}

ofxCvGrayscaleImage BaseMaterial::GetSaliencyMap()
{
	return ofxCvGrayscaleImage(saliencyMap);
}

void BaseMaterial::SetReady(bool ready)
{
	isReady = ready;
}

bool BaseMaterial::IsReady()
{
	return isReady;
}

float BaseMaterial::GetTextWeight(string text)
{
	return textWeight.at(text);
}

