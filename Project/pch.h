//
// pch.h
// Header for standard system include files.
//

#pragma once

#include <WinSDKVer.h>
#define _WIN32_WINNT 0x0600
#include <SDKDDKVer.h>

// Use the C++ standard templated min/max
#define NOMINMAX

// DirectX apps don't need GDI
#define NODRAWTEXT
#define NOGDI
#define NOBITMAP

// Include <mcx.h> if you need this
#define NOMCX

// Include <winsvc.h> if you need this
#define NOSERVICE

// WinHelp is deprecated
#define NOHELP

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <wrl/client.h>

#include <d3d11_1.h>
#include <DirectXMath.h>
#include <DirectXColors.h>

#include <algorithm>
#include <exception>
#include <memory>
#include <stdexcept>

#include <stdio.h>

#include "CommonStates.h"
#include "DDSTextureLoader.h"
#include "DirectXHelpers.h"
#include "Effects.h"
#include "GamePad.h"
#include "GeometricPrimitive.h"
#include "GraphicsMemory.h"
#include "Keyboard.h"
#include "Model.h"
#include "Mouse.h"
#include "PostProcess.h"
#include "PrimitiveBatch.h"
#include "ScreenGrab.h"
#include "SimpleMath.h"
#include "SpriteBatch.h"
#include "SpriteFont.h"
#include "VertexTypes.h"
#include "WICTextureLoader.h"

#include "btBulletDynamicsCommon.h"

namespace DX
{
    // Helper class for COM exceptions
    class com_exception : public std::exception
    {
    public:
        com_exception(HRESULT hr) : result(hr) {}

        virtual const char* what() const override
        {
            static char s_str[64] = {};
            sprintf_s(s_str, "Failure with HRESULT of %08X", static_cast<unsigned int>(result));
            return s_str;
        }

    private:
        HRESULT result;
    };

    // Helper utility converts D3D API failures into exceptions.
    inline void ThrowIfFailed(HRESULT hr)
    {
        if (FAILED(hr))
        {
            throw com_exception(hr);
        }
    }
}

namespace SimpleMath_LinearMath
{
	inline btQuaternion smQ_btQ(const DirectX::SimpleMath::Quaternion& quaternion)
	{
		return btQuaternion(quaternion.x, quaternion.y, quaternion.z, quaternion.w);
	}

	inline btVector3 smV3_btV3(const DirectX::SimpleMath::Vector3& vector3)
	{
		return btVector3(vector3.x, vector3.y, vector3.z);
	}

	inline DirectX::SimpleMath::Quaternion btQ_smQ(const btQuaternion& quaternion)
	{
		return DirectX::SimpleMath::Quaternion(quaternion.getX(), quaternion.getY(), quaternion.getZ(), quaternion.getW());
	}

	inline DirectX::SimpleMath::Vector3 btV3_smV3(const btVector3& vector3)
	{
		return DirectX::SimpleMath::Vector3(vector3.getX(), vector3.getY(), vector3.getZ());
	}

	inline DirectX::SimpleMath::Color btV3_smC(const btVector3& color)
	{
		return DirectX::SimpleMath::Color(color.getX(), color.getY(), color.getZ());
	}
}

std::string wc_s(const wchar_t* wcp);

inline std::string to_string(DirectX::SimpleMath::Vector3& vector3)
{
	return "(" + std::to_string(vector3.x) + "," +
		std::to_string(vector3.y) + "," +
		std::to_string(vector3.z) + ")";
}

inline std::string to_string(DirectX::SimpleMath::Quaternion& quaternion)
{
	return "(" + std::to_string(quaternion.x) + "," +
		std::to_string(quaternion.y) + "," +
		std::to_string(quaternion.z) + "," +
		std::to_string(quaternion.w) + ")";
}

inline std::string to_string(DirectX::SimpleMath::Vector2& vector2)
{
	return "(" + std::to_string(vector2.x) + "," +
		std::to_string(vector2.y) + ")";
}


inline std::string to_string(DirectX::SimpleMath::Color& color)
{
	return "(" + std::to_string(color.R()) + "," +
		std::to_string(color.G()) + "," +
		std::to_string(color.B()) + "," +
		std::to_string(color.A()) + ")";
}

void GetTokens(const std::string&, const char&, std::vector<std::string>&);

void GetTokens(std::ifstream&, const char&, std::vector<std::string>&);

inline DirectX::SimpleMath::Vector3 stov3(std::string str)
{
	std::vector<std::string> tokens;
	GetTokens(str.substr(1, str.size() - 1), ',', tokens);
	return DirectX::SimpleMath::Vector3(std::stof(tokens[0]), std::stof(tokens[1]), std::stof(tokens[2]));
}

inline DirectX::SimpleMath::Quaternion stoq(std::string str)
{
	std::vector<std::string> tokens;
	GetTokens(str.substr(1, str.size() - 1), ',', tokens);
	return DirectX::SimpleMath::Quaternion(std::stof(tokens[0]), std::stof(tokens[1]), std::stof(tokens[2]), std::stof(tokens[3]));
}