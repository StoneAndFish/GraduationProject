#include "TextContour.h"

TextContour::TextContour()
{}

TextContour::~TextContour()
{}

void TextContour::SetFont(ofxFontStash *fontStash)
{
	font = fontStash;
}

void TextContour::SetAlign(ofAlignHorz aln)
{
	align = aln;
}

void TextContour::SetFontSize(int size)
{
	fontSize = size;
}

void TextContour::SetCoord(float xx, float yy)
{
	x = xx;
	y = yy;
}

void TextContour::SetBound(float minx, float miny, float maxx, float maxy)
{
	minX = minx;
	minY = miny;
	maxX = maxx;
	maxY = maxy;
}

void TextContour::PutText(string text)
{
	textVector.push_back(text);
}

string TextContour::GetText()
{
	string temp = "";
	for (int i = 0; i < textVector.size(); i++)
	{
		temp = temp + textVector.at(i) + "\n";
	}

	return temp;
}

void TextContour::ClearText()
{
	if (!textVector.empty())
	{
		textVector.clear();
	}
}

bool TextContour::ComputeContour()
{
	float tempX, tempY;
	tempX = x;
	tempY = y;

	if (!contour.empty())
	{
		contour.clear();
	}

	for (int i = 0; i < textVector.size(); i++)
	{
		ofRectangle tempRect = font->getBBox(textVector.at(i), fontSize, tempX, tempY);
		if (OF_ALIGN_HORZ_LEFT == align)
		{
			if (!JudgeBound(tempRect))
			{
				return false;
			}
		}
		else if (OF_ALIGN_HORZ_RIGHT == align)
		{
			tempRect.setPosition((tempX - tempRect.getWidth()), tempRect.getY());
			if (!JudgeBound(tempRect))
			{
				return false;
			}
		}
		else if (OF_ALIGN_HORZ_CENTER == align)
		{

			tempRect.setPosition(minX + (maxX - minX - tempRect.getWidth()) * 0.5, tempRect.getY());
//			std::cout << tempRect.getX() << " " << tempRect.getY() << std::endl;
			if (!JudgeBound(tempRect))
			{
				return false;
			}
		}

		contour.push_back(tempRect);

		tempY = tempY + font->getFontHeight(fontSize);
	}

	return true;
}

vector<ofRectangle> TextContour::GetContour()
{
	return contour;
}

ofPoint TextContour::GetPosition()
{
	float tx = 0.0f, ty = y;
	if (OF_ALIGN_HORZ_RIGHT == align)
	{
		for (int i = 0; i < contour.size(); i++)
		{
			if (0 == i)
			{
				tx = contour.at(i).getBottomLeft().x;
			}

			float tempX = contour.at(i).getBottomLeft().x;
			if (tempX < tx)
			{
				tx = tempX;
			}
		}
	}
	else
	{
		tx = contour.at(0).getBottomLeft().x;
		ty = contour.at(0).getBottomLeft().y;
	}


	return ofPoint(tx, ty);
}

bool TextContour::JudgeBound(ofRectangle rect)
{
	if ((rect.getMaxX() > maxX) || (rect.getMinX() < minX)
		|| (rect.getMaxY() > maxY) || (rect.getMinY() < minY))
	{
//		std::cout << "maxX:" << maxX << ",maxY" << maxY << ",minX:" << minX << ",minY:" << minY << std::endl;
//		std::cout << rect.getMaxX() << " " << rect.getMaxY() << " " << rect.getMinX() << " " << rect.getMinY() << std::endl;
		return false;
	}

	return true;
}