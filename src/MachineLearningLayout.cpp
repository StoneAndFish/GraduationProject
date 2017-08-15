#include "MachineLearningLayout.h"

MachineLearningLayout::MachineLearningLayout()
{
}

MachineLearningLayout::~MachineLearningLayout()
{
}

void MachineLearningLayout::BeginLayout(float width, float height, BaseMaterial *m, Results *r)
{
	Results::Instance()->SetResult("layoutMethod : MachineLearningLayout");
}
