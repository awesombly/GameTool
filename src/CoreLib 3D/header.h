#pragma once
#pragma warning(disable : 4005)			// 매크로 재정의 경고 무시
#pragma warning(disable : 4201)			// 
#pragma warning(disable : 4099)			//

#ifndef _WINSOCKAPI_
#define _WINSOCKAPI_
#endif

#include <Windows.h>
#include <d3d11.h>						// DX11
#include <d3dx11.h>						// 확장 버전
#include <d3dcompiler.h>				// 쉐이더 컴파일 함수
#include <D3DX10math.h>

#include <algorithm>
#include <ctime>
#include <cmath>
#include <tchar.h>						// _swprintf_s(TCHAR).. 정의
#include <future>
#include <random>
#include <iostream>

#include <sstream>						// ostringstream
#include <string>
#include <string_view>
#include <vector>
#include <array>
#include <list>
#include <forward_list>
#include <map>
#include <stack>
#include <queue>
//#include <numeric>					// 수치 관련 알고리즘
//#include <functional>					// 범용비교함수, 함수포인터..

// 라이브러리 링크를 전처리 해준다
#ifdef _DEBUG
	#pragma comment( lib, "_CoreLib D3D_d.lib")
#else
	#pragma comment( lib, "_CoreLib D3D.lib")
#endif
//#pragma comment( lib, "ServerLib.lib")
#pragma comment( lib, "d2d1.lib")
#pragma comment( lib, "dwrite.lib")
#pragma comment( lib, "d3d11.lib")		// DX11
#pragma comment( lib, "d3dx10d.lib")	// 벡터, 행렬..
#pragma comment( lib, "d3dx11.lib")		// DX11 Expension
#pragma comment( lib, "dxgi.lib")
#pragma comment( lib, "dxguid.lib" )
#pragma comment( lib, "winmm.lib")		// +timeGetTime()
#pragma comment( lib, "msimg32.lib")	// +TransparentBlt()	

using namespace std;

constexpr static float PI = 3.14159265358979323846f;
constexpr static float EPSILON = std::numeric_limits<float>::epsilon();
constexpr static float GravityPower = 9.8f * 10.0f;

namespace Vector3 {
	const static D3DXVECTOR3 One	  =	{ 1.0f,  1.0f,  1.0f };			// 1, 1, 1
	const static D3DXVECTOR3 Zero	  =	{ 0.0f,  0.0f,  0.0f };			// 0, 0 ,0
	const static D3DXVECTOR3 Left	  =	{ -1.0f, 0.0f,  0.0f };			// -1, 0 ,0
	const static D3DXVECTOR3 Right	  =	{ 1.0f,  0.0f,  0.0f };			// 1, 0 ,0
	const static D3DXVECTOR3 Up		  =	{ 0.0f,  1.0f,  0.0f };			// 0, 1 ,0
	const static D3DXVECTOR3 Down	  =	{ 0.0f, -1.0f,  0.0f };			// 0, -1 ,0
	const static D3DXVECTOR3 Forward  =	{ 0.0f,  0.0f,  1.0f };			// 0, 0 ,1
	const static D3DXVECTOR3 Backward = { 0.0f,  0.0f, -1.0f };			// 0, 0 ,-1
};

namespace Vector4 {
	const static D3DXVECTOR4 One	  = { 1.0f,  1.0f,  1.0f, 1.0f };	// 1, 1, 1, 1
	const static D3DXVECTOR4 Zero	  = { 0.0f,  0.0f,  0.0f, 0.0f };	// 0, 0 ,0, 0
	const static D3DXVECTOR4 Left	  = { -1.0f, 0.0f,  0.0f, 0.0f };	// -1, 0 ,0, 0
	const static D3DXVECTOR4 Right	  = { 1.0f,  0.0f,  0.0f, 0.0f };	// 1, 0 ,0, 0
	const static D3DXVECTOR4 Up		  = { 0.0f,  1.0f,  0.0f, 0.0f };	// 0, 1 ,0, 0
	const static D3DXVECTOR4 Down	  = { 0.0f, -1.0f,  0.0f, 0.0f };	// 0, -1 ,0, 0
	const static D3DXVECTOR4 Forward  = { 0.0f,  0.0f,  1.0f, 0.0f };	// 0, 0 ,1, 0
	const static D3DXVECTOR4 Backward = { 0.0f,  0.0f, -1.0f, 0.0f };	// 0, 0 ,-1, 0
};

namespace Quaternion {
	const static D3DXQUATERNION Base	 = { 0.0f,  0.0f,  0.0f, 1.0f };	// 0, 0, 0, 1
	const static D3DXQUATERNION One		 = { 1.0f,  1.0f,  1.0f, 1.0f };	// 1, 1, 1, 1
	const static D3DXQUATERNION Zero	 = { 0.0f,  0.0f,  0.0f, 0.0f };	// 0, 0 ,0,0
	const static D3DXQUATERNION Left	 = { 0.0f, -1.0f,  0.0f, 0.0f };	// 0, -1 ,0, 0
	const static D3DXQUATERNION Right	 = { 0.0f,  1.0f,  0.0f, 0.0f };	// 0, 1 ,0, 0
	const static D3DXQUATERNION Up		 = { 1.0f,  0.0f,  0.0f, 0.0f };	// 1, 0 ,0,0
	const static D3DXQUATERNION Down	 = { -1.0f,  0.0f, 0.0f, 0.0f };	// -1, 0 ,0, 0
	const static D3DXQUATERNION Forward  = { 0.0f,  0.0f,  1.0f, 0.0f };	// 0, 0 ,1, 0
	const static D3DXQUATERNION Backward = { 0.0f,  0.0f, -1.0f, 0.0f };	// 0, 0 ,-1 0
};

namespace Color {
	const static D3DXVECTOR4 White  = { 1.0f,  1.0f,  1.0f, 1.0f };		// 1, 1, 1, 1
	const static D3DXVECTOR4 Black  = { 0.0f,  0.0f,  0.0f, 1.0f };		// 0, 0, 0, 1
	const static D3DXVECTOR4 Red    = { 1.0f,  0.0f,  0.0f, 1.0f };		// 1, 0, 0, 1
	const static D3DXVECTOR4 Green  = { 0.0f,  1.0f,  0.0f, 1.0f };		// 0, 1, 0, 1
	const static D3DXVECTOR4 Blue   = { 0.0f,  0.0f,  1.0f, 1.0f };		// 0, 0, 1, 1
	const static D3DXVECTOR4 Yellow = { 1.0f,  1.0f,  0.0f, 1.0f };		// 1, 1, 0, 1
	const static D3DXVECTOR4 Purple = { 1.0f,  0.0f,  1.0f, 1.0f };		// 1, 0, 1, 1
	const static D3DXVECTOR4 Cyan   = { 0.0f,  1.0f,  1.0f, 1.0f };		// 0, 1, 1, 1
};

namespace Matrix {
	const static D3DXMATRIX Identity = { 1, 0, 0, 0,
										 0, 1, 0, 0,
										 0, 0, 1, 0,
										 0, 0, 0, 1 };
	const static D3DXMATRIX Zero = { 0, 0, 0, 0,
									 0, 0, 0, 0,
									 0, 0, 0, 0,
									 0, 0, 0, 0 };
};

namespace DataInfo {
	constexpr static UINT MaxUINT = (UINT)-1;
	constexpr static UCHAR MaxUCHAR = (UCHAR)-1;
}


void ErrorMessage(const string_view& msg, const bool& useLoop = false)	noexcept;
void ErrorMessage(const wstring_view& msg, const bool& useLoop = false) noexcept;
// WELLRNG512 난수 생성
unsigned long Random(void) noexcept;
float RandomNormal(void) noexcept;
// 난수 시드 설정
void InitWELLState() noexcept;
///
char * WCharToChar(const wchar_t* str)	noexcept;
wchar_t* CharToWChar(const char* str)		noexcept;
///
static constexpr float DegreeToRadian(const float& degree) noexcept
{
	return degree * (PI / 180);
}
static constexpr float RadianToDegree(const float& radian) noexcept
{
	return radian * (180 / PI);
}
const float			 Lerp(const float& start, const float& end, const float& time)					 noexcept;
const D3DXVECTOR3	 Lerp(const D3DXVECTOR3& start, const D3DXVECTOR3& end, const float& time)		 noexcept;
const D3DXQUATERNION Lerp(const D3DXQUATERNION& start, const D3DXQUATERNION& end, const float& time) noexcept;
const D3DXVECTOR3	 Product(const D3DXVECTOR3& vectorA, const D3DXVECTOR3& vectorB)				 noexcept;
const D3DXQUATERNION Product(const D3DXQUATERNION& quatA, const D3DXQUATERNION& quatB)				 noexcept;
const D3DXVECTOR3	 Divide(const D3DXVECTOR3& vectorA, const D3DXVECTOR3& vectorB)					 noexcept;
const D3DXQUATERNION Divide(const D3DXQUATERNION& quatA, const D3DXQUATERNION& quatB)				 noexcept;
const D3DXVECTOR2	 Normalize(const D3DXVECTOR2& vector2)											 noexcept;
const D3DXVECTOR3	 Normalize(const D3DXVECTOR3& vector3)											 noexcept;
//const D3DXVECTOR2	 VectorCross(const D3DXVECTOR2& vectorA, const D3DXVECTOR2& vectorB)			 noexcept;
const D3DXVECTOR3	 VectorCross(const D3DXVECTOR3& vectorA, const D3DXVECTOR3& vectorB)			 noexcept;
const float			 VectorDot(const D3DXVECTOR2& vectorA, const D3DXVECTOR2& vectorB)				 noexcept;
const float			 VectorDot(const D3DXVECTOR3& vectorA, const D3DXVECTOR3& vectorB)				 noexcept;
const float			 VectorLength(const D3DXVECTOR2& vector2)										 noexcept;
const float			 VectorLength(const D3DXVECTOR3& vector3)										 noexcept;
const float			 VectorLengthSq(const D3DXVECTOR2& vector2)										 noexcept;
const float			 VectorLengthSq(const D3DXVECTOR3& vector3)										 noexcept;
const D3DXMATRIX	 MatrixTranspose(const D3DXMATRIX& matrix)										 noexcept;
const D3DXVECTOR3	 QuatToRotation(const D3DXQUATERNION& quat)										 noexcept;


























namespace myStd {
	// 32비트용 sqrt
	//double inline __declspec (naked) __fastcall sqrt14(double n)
	//{
	//	_asm fld qword ptr[esp + 4]
	//	_asm fsqrt
	//	_asm ret 8
	//}

	// 약간 빠른 sqrt
	//float sqrtQuick(const float& value) noexcept
	//{
	//	union
	//	{
	//		int i;
	//		float x;
	//	} u;
	//	u.x = value;
	//	u.i = (1 << 29) + (u.i >> 1) - (1 << 22);
	//	// Two Babylonian Steps (simplified from:)
	//	// u.x = 0.5f * (u.x + x/u.x);
	//	// u.x = 0.5f * (u.x + x/u.x);
	//	u.x = u.x + value / u.x;
	//	u.x = 0.25f * u.x + value / u.x;
	//	return u.x;
	//}

	//  템플릿 abs
	template <typename Type>
	constexpr Type AbsT(const Type& value) noexcept
	{
		return value >= 0 ? value : -value;
	}
	// 템플릿 pow
	template <typename Type, typename Type2>
	constexpr Type PowT(const Type& target, const Type& result, const Type2& value) noexcept
	{
		return value <= 1 ? result : PowT(target, result * target, value - 1);
	}
	template <typename Type, typename Type2>
	constexpr Type PowT(const Type& target, const Type2& value) noexcept
	{
		return value <= 1 ? target : PowT(target, target * target, value - 1);
	}
	// 기본 2제곱
	template <typename Type>
	constexpr Type PowT(const Type& target) noexcept
	{
		return target * target;
	}
	/*template <typename Type>
	struct PowT
	{
	enum { value = target * target }
	};*/

	// 컴파일타임 sin
	constexpr float SinSeries(const float& radian, const int& cnt, const int& maxTerms) noexcept
	{
		return cnt > maxTerms ? 1.0f :
			1.0f - (radian * radian / (2.0f * cnt + 2.0f) / (2.0f * cnt + 3.0f) * SinSeries(radian, cnt + 1, maxTerms));
	}
	constexpr float Sin(const float& radian) noexcept
	{
		constexpr int maxTerms = 10;
		return radian * SinSeries(radian, 0, maxTerms);
	}

	// con, tan, 
}