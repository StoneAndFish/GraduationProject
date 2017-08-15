#include "DemoLayout.h"

DemoLayout::DemoLayout()
{
	demoTemp = new DemoTemplate();
}

DemoLayout::~DemoLayout()
{

}

void DemoLayout::BeginLayout(float width, float height, BaseMaterial *m, Results *r)
{
	material = m;
	results = r;

	results->SetResult("layoutMethod : DemoLayout");

	ofxCvGrayscaleImage tempSMap(material->GetSaliencyMap());
	tempSMap.resize(width, height);

	Typography(width, height, tempSMap.getCvImage());

	results->SetTemplate(demoTemp);
}

void DemoLayout::Typography(float width, float height, IplImage* sMap)
{
	InitTemplate(width, height);
	MinimizeEnergy(sMap);
}

void DemoLayout::InitTemplate(float w, float h)
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

float DemoLayout::MinimizeEnergy(IplImage* sMap)
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

float DemoLayout::MinimizeEnergy_Masthead(TemplateRegion * region, IplImage * sMap, float freeSpace)
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

float DemoLayout::MinimizeEnergy_Coverlines(TemplateRegion * region, IplImage * sMap, float freeSpace)
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
				if (!tempPara.empty())
				{
					tempPara.clear();
				}

				string tempStr;
				if (pos2 == pos1)
				{
					tempStr = WordMerge(word, 0, pos1);
					tempPara.push_back(tempStr);

					tempStr = WordMerge(word, pos1 + 1, word.size() - 1);
					tempPara.push_back(tempStr);

					paragraphMap[oriStr].push_back(tempPara);
				}
				else
				{
					tempStr = WordMerge(word, 0, pos1);
					tempPara.push_back(tempStr);

					tempStr = WordMerge(word, pos1 + 1, pos2);
					tempPara.push_back(tempStr);

					tempStr = WordMerge(word, pos2 + 1, word.size() - 1);
					tempPara.push_back(tempStr);

					paragraphMap[oriStr].push_back(tempPara);
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

float DemoLayout::MinimizeEnergy_Byline(TemplateRegion * region, IplImage * sMap, float freeSpace)
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

float DemoLayout::ComputeInstrutionEnergy(TemplateRegion * region, IplImage* sMap)
{
	float energy = 0.0f;
	float mol = 0.0f;
	float den = 0.0f;

	uchar* saliency = (uchar*) (sMap->imageData);
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

float DemoLayout::ComputeUtilizationEnergy(TemplateRegion * region, IplImage * sMap)
{
	float energy = 0.0f;

	vector<string> textList = region->GetTextList();
	for (int i = 0; i < textList.size(); i++)
	{
		TextContour tempContour = region->GetContour(textList.at(i));
		
		vector<ofRectangle> contourList = tempContour.GetContour();
		for (int j = 0; j < contourList.size(); j++)
		{
			energy = energy + contourList.at(j).getArea();
		}
	}

	return energy;
}

void DemoLayout::Split(const string & input, vector<string>& output, const string & c)
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

string DemoLayout::WordMerge(const vector<string> word, int start, int end)
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
