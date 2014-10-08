#pragma once
#include <string>
#include "XMFloatUtil.h"

struct MeshInstance
{
	std::string meshName;
	Vector3 position;
	Vector3 rotation;
	Vector3 scale;

	mutable bool m_IsCalculated;
	mutable DirectX::XMFLOAT4X4 world;

	const DirectX::XMFLOAT4X4& getWorldMatrix() const
	{
		if (!m_IsCalculated)
		{
			calculateWorldMatrix();
		}
		return world;
	}	

	void MeshInstance::setPosition(const Vector3& p_Position)
	{
		position = p_Position;
		m_IsCalculated = false;
	}

	void MeshInstance::setRotation(const Vector3& p_Rotation)
	{
		rotation = p_Rotation;
		m_IsCalculated = false;
	}

	void MeshInstance::setScale(const Vector3& p_Scale)
	{
		scale = p_Scale;
		m_IsCalculated = false;
	}
private:
	void calculateWorldMatrix(void) const
	{
		using namespace DirectX;
		XMMATRIX rotationMat = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&rotation));
		XMMATRIX translationMat = XMMatrixTranslationFromVector(XMLoadFloat3(&position));
		XMMATRIX scaleMat = XMMatrixScalingFromVector(XMLoadFloat3(&scale));
		XMStoreFloat4x4(&world, XMMatrixTranspose(scaleMat * rotationMat * translationMat));
		m_IsCalculated = true;
	}
};