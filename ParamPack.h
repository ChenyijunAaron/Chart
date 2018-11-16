#pragma once
#include "basetypes.h"
#include <qlineseries.h>
//#include
struct ParamPack
{
	ParamPack() ;
	VectorVec2d origSignal;
	VectorVec2d filterSignal;
	VectorVec2d processedSignal;

	//std::shared_ptr<qlines> origSeries;

};