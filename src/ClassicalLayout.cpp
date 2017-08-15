#include "ClassicalLayout.h"

ClassicalLayout::ClassicalLayout()
{
	demoTemp = new DemoTemplate();
}

ClassicalLayout::~ClassicalLayout()
{
}

void ClassicalLayout::BeginLayout(float width, float height, BaseMaterial *m, Results *r)
{
	material = m;
	results = r;

	results->SetResult("layoutMethod : ClassicalLayout");

	ImageComposition(width, height);
	Typography(width, height, smap);
	ColorDesign(ori);

	results->SetTemplate(demoTemp);
}

void ClassicalLayout::ImageComposition(float width, float height)
{
	tempOri = material->GetOriImg();
	tempSMap = material->GetSaliencyMap();

	tempOri.resize(width, height);
	tempSMap.resize(width, height);

	ori = tempOri.getCvImage();
	smap = tempSMap.getCvImage();
}

void ClassicalLayout::Typography(float width, float height, IplImage * sMap)
{
	InitTemplate(width, height);
	MinimizeEnergy(sMap);
}

void ClassicalLayout::InitTemplate(float w, float h)
{
	demoTemp->InitTemplate(w, h);

	vector<string> textList = material->GetTextList();
	for (int i = 0; i < textList.size(); i++)
	{
		// Note that we use to_string(i) here to get the text weight
		float weight = material->GetTextWeight(to_string(i) + "." + textList.at(i));
		if (weight >= 3.0f)
		{
			demoTemp->PutMasthead(textList.at(i));
		}
		else if (weight >= 2.5f)
		{
			demoTemp->PutLeftCoverlines(textList.at(i));
		}
		else if (weight >= 2.0f)
		{
			demoTemp->PutRightCoverlines(textList.at(i));
		}
		else
		{
			demoTemp->PutByline(textList.at(i));
		}
	}
}

float ClassicalLayout::MinimizeEnergy(IplImage * sMap)
{
	float energy = 1024.0f;
	float freeSpace = 0.0f;

	uchar* saliency = (uchar*)(sMap->imageData);
	for (int i = 0; i < sMap->imageSize; i++)
	{
		if (saliency[i] == 0)
		{
			freeSpace += 1.0f;
		}
	}

	energy += MinimizeEnergy_Masthead(demoTemp->GetMastheadRegion(), sMap, freeSpace);
	energy += MinimizeEnergy_Coverlines(demoTemp->GetLeftCoverlinesRegion(), sMap, freeSpace);
	energy += MinimizeEnergy_Coverlines(demoTemp->GetRightCoverlinesRegion(), sMap, freeSpace);
	energy += MinimizeEnergy_Byline(demoTemp->GetBylineRegion(), sMap, freeSpace);

	return energy;
}

float ClassicalLayout::MinimizeEnergy_Masthead(TemplateRegion * region, IplImage * sMap, float freeSpace)
{
	float energy = 1024.0f;
	int tempSize = region->GetMinSize();

	float tX, tY, tWidth, tHeight;
	tX = region->GetPosition().x;
	tY = region->GetPosition().y;
	tWidth = region->GetWidth();
	tHeight = region->GetHeight();

	TextContour tContour;
	tContour.PutText(region->GetOriSentence(0));
	tContour.SetAlign(region->GetAlign());
	tContour.SetBound(tX, tY, tX + tWidth, tY + tHeight);
	tContour.SetFont(region->GetFontStash());
	while (true)
	{
		ofRectangle rect = (region->GetFontStash())->getBBox(region->GetOriSentence(0), tempSize, tX, tY + tHeight, region->GetAlign());
		tContour.SetCoord(tX, tY + (tHeight + rect.getHeight()) * 0.5);
		//		std::cout << (rect.getMaxY() - tHeight - tY) << std::endl;
		tContour.SetFontSize(tempSize);

		if (!tContour.ComputeContour())
		{
			std::cout << tempSize << std::endl;
			break;
		}

		region->ClearText();
		region->ClearContourMap();
		region->PutText(tContour.GetText());
		region->PutContour(region->GetText(0), tContour);
		//		std::cout << region->GetText(0) << std::endl;

		float tEnergy = ((0.10f * ComputeInstrutionEnergy(region, sMap)) - (ComputeUtilizationEnergy(region, sMap) / freeSpace));
		if (tEnergy < energy)
		{
			energy = tEnergy;
			region->UpdateBest(tempSize);
		}

		tempSize++;
	}

	return energy;
}

float ClassicalLayout::MinimizeEnergy_Coverlines(TemplateRegion * region, IplImage * sMap, float freeSpace)
{
	int randomNum1 = 3000;
	int randomNum2 = 1000;
	float energy = 1024.0f;

	float tX, tY, tWidth, tHeight;
	tX = region->GetPosition().x;
	tY = region->GetPosition().y;
	tWidth = region->GetWidth();
	tHeight = region->GetHeight();

	map<string, vector<string>> wordMap;
	int oriTextSize = region->GetOriSentenceList().size();
	for (int i = 0; i < oriTextSize; i++)
	{
		Split(region->GetOriSentence(i), wordMap[region->GetOriSentence(i)], " ");
	}

	map<string, vector<vector<string>>> paragraphMap;
	for (int i = 0; i < oriTextSize; i++)
	{
		string oriStr = region->GetOriSentence(i);
		vector<string> word = wordMap[oriStr];
		int count = 0;

		vector<string> tempPara;
		tempPara.push_back(oriStr);
		paragraphMap[oriStr].push_back(tempPara);

		for (int pos1 = 0; pos1 < word.size() - 1; pos1++)
		{
			for (int pos2 = pos1; pos2 < word.size() - 1; pos2++)
			{
				for (int pos3 = pos2; pos3 < word.size() - 1; pos3++)
				{
					if (!tempPara.empty())
					{
						tempPara.clear();
					}

					string tempStr;
					if (pos3 == pos2 && pos2 == pos1)
					{
						tempStr = WordMerge(word, 0, pos1);
						tempPara.push_back(tempStr);

						tempStr = WordMerge(word, pos1 + 1, word.size() - 1);
						tempPara.push_back(tempStr);

						paragraphMap[oriStr].push_back(tempPara);
					}
					else if(pos3 != pos2 && pos2 == pos1)
					{
						tempStr = WordMerge(word, 0, pos1);
						tempPara.push_back(tempStr);

						tempStr = WordMerge(word, pos1 + 1, pos3);
						tempPara.push_back(tempStr);

						tempStr = WordMerge(word, pos3 + 1, word.size() - 1);
						tempPara.push_back(tempStr);

						paragraphMap[oriStr].push_back(tempPara);
					}
					else if(pos3 != pos2 && pos2 != pos1)
					{
						tempStr = WordMerge(word, 0, pos1);
						tempPara.push_back(tempStr);

						tempStr = WordMerge(word, pos1 + 1, pos2);
						tempPara.push_back(tempStr);

						tempStr = WordMerge(word, pos2 + 1, pos3);
						tempPara.push_back(tempStr);

						tempStr = WordMerge(word, pos3 + 1, word.size() - 1);
						tempPara.push_back(tempStr);

						paragraphMap[oriStr].push_back(tempPara);
					}
				}
			}
		}
	}

	map<string, int> hash;
	for (int i = 0; i < randomNum1; i++)
	{
		srand((unsigned)time(0));
		region->ClearText();
		region->ClearContourMap();

		vector<int> randList;
		string tempKey;
		for (int j = 0; j < oriTextSize; j++)
		{
			int size = paragraphMap[region->GetOriSentence(j)].size();
			int random = rand() % size;
			randList.push_back(random);
			tempKey = tempKey + to_string(random) + " ";
		}

		if (1 == hash[tempKey])
		{
			continue;
		}

		hash[tempKey] = 1;

		vector<TextContour> contourList;
		for (int j = 0; j < oriTextSize; j++)
		{
			TextContour tContour;
			tContour.SetAlign(region->GetAlign());
			tContour.SetBound(tX, tY, tX + tWidth, tY + tHeight);
			tContour.SetFont(region->GetFontStash());

			vector<string> tempStrList = paragraphMap[region->GetOriSentence(j)].at(randList.at(j));
			for (int z = 0; z < tempStrList.size(); z++)
			{
				tContour.PutText(tempStrList.at(z));
			}

			region->PutText(tContour.GetText());
			contourList.push_back(tContour);
		}

		for (int j = 0; j < randomNum2; j++)
		{
			bool forFlag = false;

			float xx = tX + 2;
			if (OF_ALIGN_HORZ_LEFT == region->GetAlign())
			{
				xx = tX + 2;
			}
			else if (OF_ALIGN_HORZ_RIGHT == region->GetAlign())
			{
				xx = tX + tWidth;
			}

			//float yy = (rand() % (int)tHeight) + tY + (rand() / double(RAND_MAX));

			float yy = tY + j;

			for (int size = region->GetMinSize(); size <= region->GetMaxSize(); size++)
			{
				ofRectangle rect(xx, yy, 0, 0);
				for (int z = 0; z < oriTextSize; z++)
				{
					float yyy = 0.0f;
					if (0 == z)
					{
						yyy = rect.getMaxY() + (region->GetFontStash())->getFontHeight(size) + 2;
					}
					else
					{
						yyy = rect.getMaxY() + 1.5 * (region->GetFontStash())->getFontHeight(size);
					}

					rect = (region->GetFontStash())->getBBox(region->GetText(z), size, xx, yyy, region->GetAlign());
					contourList.at(z).SetCoord(xx, yyy);
					contourList.at(z).SetFontSize(size);

					if (!contourList.at(z).ComputeContour())
					{
						forFlag = true;
						break;
					}

					region->PutContour(region->GetText(z), contourList.at(z));
				}

				if (forFlag)
				{
					break;
				}

				float tEnergy = (0.75f * ComputeInstrutionEnergy(region, sMap)) - (ComputeUtilizationEnergy(region, sMap) / freeSpace);
				if (tEnergy < energy)
				{
					energy = tEnergy;
					region->UpdateBest(size);
				}
			}

			if (forFlag)
			{
				continue;
			}
		}

	}

	return energy;
}

float ClassicalLayout::MinimizeEnergy_Byline(TemplateRegion * region, IplImage * sMap, float freeSpace)
{
	float energy = 1024.0f;

	float tX, tY, tWidth, tHeight;
	tX = region->GetPosition().x;
	tY = region->GetPosition().y;
	tWidth = region->GetWidth();
	tHeight = region->GetHeight();
	int oriTextSize = region->GetOriSentenceList().size();

	vector<TextContour> contourList;
	for (int j = 0; j < oriTextSize; j++)
	{
		TextContour tContour;
		tContour.SetAlign(region->GetAlign());
		tContour.SetBound(tX, tY, tX + tWidth, tY + tHeight);
		tContour.SetFont(region->GetFontStash());

		tContour.PutText(region->GetOriSentence(j));

		region->PutText(tContour.GetText());
		contourList.push_back(tContour);
	}

	float xx = tX + 2;
	float yy = tY + 2;
	bool flag = false;

	for (int size = region->GetMinSize(); size < region->GetMaxSize(); size++)
	{
		for (int j = 0; j < oriTextSize; j++)
		{
			contourList.at(j).SetCoord(xx, yy + (j + 1) * (region->GetFontStash())->getFontHeight(size));
			contourList.at(j).SetFontSize(size);

			if (!contourList.at(j).ComputeContour())
			{
				flag = true;
				break;
			}

			region->PutContour(region->GetText(j), contourList.at(j));
		}

		if (flag)
		{
			break;
		}

		float tEnergy = (0.15f * ComputeInstrutionEnergy(region, sMap)) - (ComputeUtilizationEnergy(region, sMap) / freeSpace);
		if (tEnergy < energy)
		{
			energy = tEnergy;
			region->UpdateBest(size);
		}
	}

	return energy;
}

float ClassicalLayout::ComputeInstrutionEnergy(TemplateRegion * region, IplImage * sMap)
{
	float energy = 0.0f;
	float mol = 0.0f;
	float den = 0.0f;

	uchar* saliency = (uchar*)(sMap->imageData);
	int widthStep = sMap->widthStep;

	vector<string> textList = region->GetTextList();
	for (int i = 0; i < textList.size(); i++)
	{
		TextContour tempContour = region->GetContour(textList.at(i));

		vector<ofRectangle> contourList = tempContour.GetContour();
		for (int j = 0; j < contourList.size(); j++)
		{
			ofRectangle rect = contourList.at(j);
			for (int row = rect.getTopLeft().y; row < rect.getBottomLeft().y; row++)
			{
				for (int col = rect.getTopLeft().x; col < rect.getTopRight().x; col++)
				{
					mol = mol + saliency[col + row * widthStep];
					den += 255;
				}
			}
		}
	}

	energy = mol / den;

	return energy;
}

float ClassicalLayout::ComputeUtilizationEnergy(TemplateRegion * region, IplImage * sMap)
{
	float energy = 0.0f;
	uchar* saliency = (uchar*)(sMap->imageData);
	int widthStep = sMap->widthStep;

	vector<string> textList = region->GetTextList();
	for (int i = 0; i < textList.size(); i++)
	{
		TextContour tempContour = region->GetContour(textList.at(i));

		vector<ofRectangle> contourList = tempContour.GetContour();
		for (int j = 0; j < contourList.size(); j++)
		{
			ofRectangle rect = contourList.at(j);
			for (int row = rect.getTopLeft().y; row < rect.getBottomLeft().y; row++)
			{
				for (int col = rect.getTopLeft().x; col < rect.getTopRight().x; col++)
				{
					if (saliency[col + row * widthStep] < 255)
					{
						energy += 1.0f;
					}
				}
			}
		}
	}

	return energy;
}

void ClassicalLayout::ColorDesign(IplImage * oriImage)
{
	ColorDesignByRegion(oriImage);
}

void ClassicalLayout::ColorDesignByImg(IplImage * oriImage)
{
	ofColor mainColor = GetMainColorOcTree(oriImage);
	float h, s, b;
	h = mainColor.getHueAngle();
	s = mainColor.getSaturation();
	b = mainColor.getBrightness();

	ofColor first, second, third;
	float x, y, z;

	if (s < 127.f) x = y = z = 255.f - s; else { x = s; y = z = s * 0.618; }
	first.setSaturation(x);
	second.setSaturation(y);
	third.setSaturation(z);
	
	if (b < 127.f) x = y = z = 255.f - b; else { x = b; y = z = b * 0.618; }
	first.setBrightness(x);
	second.setBrightness(y);
	third.setBrightness(z);
	
	x = h + 180.f; if (x > 360) x -= 360;
	y = h + 195.f; if (y > 360) y -= 360;
	z = h + 210.f; if (z > 360) z -= 360;
	first.setHueAngle(x);
	second.setHueAngle(y);
	third.setHueAngle(z);

	vector<ofColor> colorList1;
	vector<ofColor> colorList2;
	colorList1.push_back(first);
	colorList2.push_back(second);
	colorList2.push_back(third);
	
	demoTemp->SetFirstColor(colorList1);
	demoTemp->SetSecondColor(colorList2);
}

ofColor ClassicalLayout::GetMainColorAvg(IplImage * oriImage)
{
	cv::Mat image(oriImage);
	int hisSize[1];
	float range[2];
	const float *ranges = &range[0];
	cv::Mat channelsRGB[3];
	cv::MatND outputRGB[3];

	hisSize[0] = 256;
	range[0] = 0.0;
	range[1] = 255.0;

	cv::split(image, channelsRGB);

	calcHist(&channelsRGB[0], 1, 0, cv::Mat(), outputRGB[0], 1, hisSize, &ranges);
	calcHist(&channelsRGB[1], 1, 0, cv::Mat(), outputRGB[1], 1, hisSize, &ranges);
	calcHist(&channelsRGB[2], 1, 0, cv::Mat(), outputRGB[2], 1, hisSize, &ranges);

	float resultsB = 0.0f;
	float resultsG = 0.0f;
	float resultsR = 0.0f;

	float den = 0.0f;

	for (int i = 0; i < hisSize[0]; i++)
	{
		den += (float)i;
	}

	for (int i = 0; i < hisSize[0]; i++)
	{
		resultsB += (outputRGB[0].at<float>(i) * (float)i);
		resultsG += (outputRGB[1].at<float>(i) * (float)i);
		resultsR += (outputRGB[2].at<float>(i) * (float)i);
	}

	resultsB /= den;
	resultsG /= den;
	resultsR /= den;

	return ofColor(resultsR, resultsG, resultsB);
}

ofColor ClassicalLayout::GetMainColorOcTree(IplImage * oriImage)
{
	ofColor result;

	int pixCount = oriImage->imageSize / 3;
	uchar* uImage = (uchar*)oriImage->imageData;

	thmclrx::RGB** pRGBs = new thmclrx::RGB*[pixCount];
	for (int i = 0; i < pixCount; i++)
	{
		thmclrx::RGB* tmp = g_PoolRGB.Create();
		tmp->red = uImage[i * 3 + 2];
		tmp->green = uImage[i * 3 + 1];
		tmp->blue = uImage[i * 3];

		pRGBs[i] = tmp;
	}

	Octree oTree;
	oTree.buildTree(pRGBs, pixCount, MAX_COLOR);

	vector<ColorCount*> colorCount;
	oTree.colorStats(oTree.getRoot(), &colorCount);

	result.r = colorCount.at(0)->red;
	result.g = colorCount.at(0)->green;
	result.b = colorCount.at(0)->blue;

	Octree::recycleColorCount(&colorCount);
	thmclrx::RGB::recycleArray(pRGBs, pixCount);
	delete []pRGBs;

	return result;
}

void ClassicalLayout::ColorDesignByRegion(IplImage * oriImage)
{
	TemplateRegion* region = demoTemp->GetMastheadRegion();
	int tX, tY, width, height;
	tX = (int)region->GetTX();
	tY = (int)region->GetTY();
	width = (int)region->GetWidth();
	height = (int)region->GetHeight();

	int pixCount = width * height * 3;
	uchar* uImage = new uchar[pixCount];
	int count = 0;
	for (int i = tY; i < tY + height; i++)
	{
		for (int j = tX; j < tX + width; j++)
		{
			uImage[count++] = (uchar)oriImage->imageData[j * 3 + i * oriImage->widthStep];
			uImage[count++] = (uchar)oriImage->imageData[j * 3 + i * oriImage->widthStep + 1];
			uImage[count++] = (uchar)oriImage->imageData[j * 3 + i * oriImage->widthStep + 2];
		}
	}

//	ofColor firstColor = GetMainColorOcTree(pixCount / 3, uImage);
	ofColor firstColor = GetMainColorOcTree(oriImage);
	
	delete []uImage;
	uImage = NULL;

	pixCount = 0;
	count = 0;
	region = demoTemp->GetLeftCoverlinesRegion();
	if (region->GetOriSentenceList().size() > 0)
	{
		pixCount = pixCount + ((int)region->GetHeight()) * ((int)region->GetWidth()) * 3;
	}
	region = demoTemp->GetRightCoverlinesRegion();
	if (region->GetOriSentenceList().size() > 0)
	{
		pixCount = pixCount + ((int)region->GetHeight()) * ((int)region->GetWidth()) * 3;
	}
	region = demoTemp->GetBylineRegion();
	if (region->GetOriSentenceList().size() > 0)
	{
		pixCount = pixCount + ((int)region->GetHeight()) * ((int)region->GetWidth()) * 3;
	}

	uImage = new uchar[pixCount];

	region = demoTemp->GetLeftCoverlinesRegion();
	if (region->GetOriSentenceList().size() > 0)
	{
		tX = (int)region->GetTX();
		tY = (int)region->GetTY();
		width = (int)region->GetWidth();
		height = (int)region->GetHeight();
		for (int i = tY; i < tY + height; i++)
		{
			for (int j = tX; j < tX + width; j++)
			{
				uImage[count++] = (uchar)oriImage->imageData[j * 3 + i * oriImage->widthStep];
				uImage[count++] = (uchar)oriImage->imageData[j * 3 + i * oriImage->widthStep + 1];
				uImage[count++] = (uchar)oriImage->imageData[j * 3 + i * oriImage->widthStep + 2];
			}
		}
	}

	region = demoTemp->GetRightCoverlinesRegion();
	if (region->GetOriSentenceList().size() > 0)
	{
		tX = (int)region->GetTX();
		tY = (int)region->GetTY();
		width = (int)region->GetWidth();
		height = (int)region->GetHeight();
		for (int i = tY; i < tY + height; i++)
		{
			for (int j = tX; j < tX + width; j++)
			{
				uImage[count++] = (uchar)oriImage->imageData[j * 3 + i * oriImage->widthStep];
				uImage[count++] = (uchar)oriImage->imageData[j * 3 + i * oriImage->widthStep + 1];
				uImage[count++] = (uchar)oriImage->imageData[j * 3 + i * oriImage->widthStep + 2];
			}
		}
	}

	region = demoTemp->GetBylineRegion();
	if (region->GetOriSentenceList().size() > 0)
	{
		tX = (int)region->GetTX();
		tY = (int)region->GetTY();
		width = (int)region->GetWidth();
		height = (int)region->GetHeight();
		for (int i = tY; i < tY + height; i++)
		{
			for (int j = tX; j < tX + width; j++)
			{
				uImage[count++] = (uchar)oriImage->imageData[j * 3 + i * oriImage->widthStep];
				uImage[count++] = (uchar)oriImage->imageData[j * 3 + i * oriImage->widthStep + 1];
				uImage[count++] = (uchar)oriImage->imageData[j * 3 + i * oriImage->widthStep + 2];
			}
		}
	}
	
	ofColor secondColor = GetMainColorOcTree(pixCount / 3, uImage);

	delete []uImage;
	uImage = NULL;

	float h, s, b;
	h = firstColor.getHueAngle();
	s = firstColor.getSaturation();
	b = firstColor.getBrightness();

	ofColor first, second, third;

	if (s < 127.f) s = 255.f - s;
	first.setSaturation(s);
	if (b < 127.f) b = 255.f - b;
	first.setBrightness(b);
	h += 180.f; if (h > 360.f) h -= 360.f;
	first.setHueAngle(h);

	h = secondColor.getHueAngle();
	s = secondColor.getSaturation();
	b = secondColor.getBrightness();

	if (s < 127.f) s = 255.f - s; 
	second.setSaturation(s);
	third.setSaturation(s);
	if (b < 127.f) b = 255.f - b; 
	second.setBrightness(b);
	third.setSaturation(b);
	h += 190.f; if (h > 360.f) h -= 360.f;
	third.setHueAngle(h);
	h += 10.f; if (h > 360.f) h -= 360.f;
	second.setHueAngle(h);

	vector<ofColor> colorList1;
	vector<ofColor> colorList2;
	colorList1.push_back(first);
	colorList2.push_back(second);
	colorList2.push_back(third);

	demoTemp->SetFirstColor(colorList1);
	demoTemp->SetSecondColor(colorList2);
}

ofColor ClassicalLayout::GetMainColorOcTree(int pixCount, uchar * uImage, int index)
{
	ofColor result;

	if (0 == pixCount)
	{
		return ofColor();
	}

	thmclrx::RGB** pRGBs = new thmclrx::RGB*[pixCount];
	for (int i = 0; i < pixCount; i++)
	{
		thmclrx::RGB* tmp = g_PoolRGB.Create();
		tmp->red = uImage[i * 3 + 2];
		tmp->green = uImage[i * 3 + 1];
		tmp->blue = uImage[i * 3];

		pRGBs[i] = tmp;
	}

	Octree oTree;
	oTree.buildTree(pRGBs, pixCount, MAX_COLOR);

	vector<ColorCount*> colorCount;
	oTree.colorStats(oTree.getRoot(), &colorCount);

	result.r = colorCount.at(index)->red;
	result.g = colorCount.at(index)->green;
	result.b = colorCount.at(index)->blue;

	Octree::recycleColorCount(&colorCount);
	thmclrx::RGB::recycleArray(pRGBs, pixCount);
	delete[]pRGBs;

	return result;
}

void ClassicalLayout::Split(const string & input, vector<string>& output, const string & c)
{
	string::size_type pos1, pos2;
	pos2 = input.find(c);
	pos1 = 0;

	while (string::npos != pos2)
	{
		output.push_back(input.substr(pos1, pos2 - pos1));

		pos1 = pos2 + c.size();
		pos2 = input.find(c, pos1);
	}

	if (pos1 != input.length())
	{
		output.push_back(input.substr(pos1));
	}
}

string ClassicalLayout::WordMerge(const vector<string> word, int start, int end)
{
	string tempStr;
	for (int i = start; i <= end; i++)
	{
		tempStr = tempStr + word[i];
		if (i < end)
		{
			tempStr = tempStr + " ";
		}
	}

	return tempStr;
}
