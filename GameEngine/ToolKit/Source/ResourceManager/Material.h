#pragma once
#include <string>
#include <DirectXMath.h>

#include "Resource.h"

namespace Res
{
	struct Material
	{
		std::string name;
		DirectX::XMFLOAT3 Kd;
		DirectX::XMFLOAT3 Ka;
		DirectX::XMFLOAT3 Ks;
		float Ns;
		float d;
		float illum;
		Resource map_Kd;

		Material(){}
	};
}