#include "header.h"
#include <fstream>

#ifdef _DEBUG
static std::wofstream debugStream;// (L"../../debugOut.txt", std::ios::app);
#endif

void ErrorMessage(const string_view& msg, const bool& useLoop) noexcept
{
#ifdef _DEBUG
	// 디버그 출력
	static stringstream ostr;
	ostr.str("");
	ostr << endl << msg << "\t";
	OutputDebugStringA(ostr.str().c_str());

	debugStream.open(L"../../debugOut.txt", std::ios::app);
	if (debugStream.is_open())
	{
		debugStream << ostr.str().c_str();
		debugStream.close();
	}
	while (useLoop)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
		OutputDebugStringA(ostr.str().c_str());
	}
#endif
}

void ErrorMessage(const wstring_view& msg, const bool& useLoop) noexcept
{
#ifdef _DEBUG
	// 디버그 출력
	static wstringstream ostr;
	ostr.str(L"");
	ostr << endl << msg << L"\t";
	OutputDebugStringW(ostr.str().c_str());

	debugStream.open(L"../../debugOut.txt", std::ios::app);
	if (debugStream.is_open())
	{
		debugStream << ostr.str().c_str();
		debugStream.close();
	}
	while (useLoop)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
		OutputDebugStringW(ostr.str().c_str());
	}
#endif
}


// + Random 비트 초기화
static unsigned long wellState[16];
static unsigned int wellIndex = 0;
const static unsigned long MaxValue = (unsigned long)-1;
// WELLRNG512 난수 생성기
unsigned long Random(void) noexcept
{
	static unsigned long a, b, c, d;
	a = wellState[wellIndex];
	c = wellState[(wellIndex + 13) & 15];
	b = a ^ c ^ (a << 16) ^ (c << 15);
	c = wellState[(wellIndex + 9) & 15];
	c ^= (c >> 11);
	a = wellState[wellIndex] = b ^ c;
	d = a ^ ((a << 5) & 0xDA442D20UL);
	wellIndex = (wellIndex + 15) & 15;
	a = wellState[wellIndex];
	wellState[wellIndex] = a ^ b ^ d ^ (a << 2) ^ (b << 18) ^ (c << 28);
	return wellState[wellIndex];
}

float RandomNormal(void) noexcept
{
	return (float)Random() / (float)MaxValue;
}

void InitWELLState() noexcept
{
	for (auto& iter : wellState)
	{
		iter = rand();
	}
}


char * WCharToChar(const wchar_t* str) noexcept
{
	//반환할 char* 변수 선언
	char* pStr;
	//입력받은 wchar_t 변수의 길이를 구함
	int strSize = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
	//char* 메모리 할당
	pStr = new char[strSize];
	//형 변환 
	WideCharToMultiByte(CP_ACP, 0, str, -1, pStr, strSize, 0, 0);
	return pStr;
}

wchar_t* CharToWChar(const char* str) noexcept
{
	//wchar_t형 변수 선언
	wchar_t* pStr;
	//멀티 바이트 크기 계산 길이 반환
	int strSize = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, NULL);
	//wchar_t 메모리 할당
	pStr = new WCHAR[strSize];
	//형 변환
	MultiByteToWideChar(CP_ACP, 0, str, (int)strlen(str) + 1, pStr, strSize);
	return pStr;
}



const float Lerp(const float& start, const float& end, const float& time) noexcept
{
	return (1 - min(time, 1.0f)) * start + min(time, 1.0f) * end;
}

const D3DXVECTOR3 Lerp(const D3DXVECTOR3& start, const D3DXVECTOR3& end, const float& time) noexcept
{
	return (1 - min(time, 1.0f)) * start + min(time, 1.0f) * end;
}

const D3DXQUATERNION Lerp(const D3DXQUATERNION& start, const D3DXQUATERNION& end, const float& time) noexcept
{
	return (1 - min(time, 1.0f)) * start + min(time, 1.0f) * end;
}

const D3DXVECTOR3 Product(const D3DXVECTOR3& vectorA, const D3DXVECTOR3& vectorB) noexcept
{
	return { vectorA.x * vectorB.x, vectorA.y * vectorB.y, vectorA.z * vectorB.z };
}
const D3DXQUATERNION Product(const D3DXQUATERNION& quatA, const D3DXQUATERNION& quatB) noexcept
{
	return { quatA.x * quatB.x, quatA.y * quatB.y, quatA.z * quatB.z, quatA.w * quatB.w };
}

const D3DXVECTOR3 Divide(const D3DXVECTOR3& vectorA, const D3DXVECTOR3& vectorB) noexcept
{
	return { vectorA.x / vectorB.x, vectorA.y / vectorB.y, vectorA.z / vectorB.z };
}
const D3DXQUATERNION Divide(const D3DXQUATERNION& quatA, const D3DXQUATERNION& quatB) noexcept
{
	return { quatA.x / quatB.x, quatA.y / quatB.y, quatA.z / quatB.z, quatA.w / quatB.w };
}

const D3DXVECTOR2 Normalize(const D3DXVECTOR2& vector2) noexcept
{
	float length = sqrt(vector2.x * vector2.x + vector2.y * vector2.y);
	return { vector2.x / length, vector2.y / length };
}
const D3DXVECTOR3 Normalize(const D3DXVECTOR3& vector3) noexcept
{
	float length = sqrt(vector3.x * vector3.x + vector3.y * vector3.y + vector3.z * vector3.z);
	return { vector3.x / length, vector3.y / length, vector3.z / length };
}

//const D3DXVECTOR2 VectorCross(const D3DXVECTOR2& vectorA, const D3DXVECTOR2& vectorB) noexcept
//{
//	
//}
const D3DXVECTOR3 VectorCross(const D3DXVECTOR3& vectorA, const D3DXVECTOR3& vectorB) noexcept 
{
	return { (vectorA.y * vectorB.z) - (vectorB.y * vectorA.z),
			 (vectorB.x * vectorA.z) - (vectorA.x * vectorB.z),
			 (vectorA.x * vectorB.y) - (vectorB.x * vectorA.y) };
}

const float VectorDot(const D3DXVECTOR2& vectorA, const D3DXVECTOR2& vectorB) noexcept 
{
	return vectorA.x * vectorB.x + vectorA.y * vectorB.y;
}
const float VectorDot(const D3DXVECTOR3& vectorA, const D3DXVECTOR3& vectorB) noexcept
{
	return vectorA.x * vectorB.x + vectorA.y * vectorB.y + vectorA.z * vectorB.z;
}

const float VectorLength(const D3DXVECTOR2& vector2) noexcept
{
	return std::sqrtf(std::powf(vector2.x, 2) + std::powf(vector2.y, 2));
}
const float VectorLength(const D3DXVECTOR3& vector3) noexcept
{
	return std::sqrtf(std::powf(vector3.x, 2) + std::powf(vector3.y, 2) + std::powf(vector3.z, 2));
}

const float VectorLengthSq(const D3DXVECTOR2& vector2) noexcept
{
	return std::powf(vector2.x, 2) + std::powf(vector2.y, 2);
}
const float VectorLengthSq(const D3DXVECTOR3& vector3) noexcept
{
	return std::powf(vector3.x, 2) + std::powf(vector3.y, 2) + std::powf(vector3.z, 2);
}

const D3DXMATRIX MatrixTranspose(const D3DXMATRIX& matrix) noexcept
{
	return { matrix._11, matrix._21, matrix._31, matrix._41,
			 matrix._12, matrix._22, matrix._32, matrix._42,
			 matrix._13, matrix._23, matrix._33, matrix._43,
			 matrix._14, matrix._24, matrix._34, matrix._44	};
}

const D3DXVECTOR3 QuatToRotation(const D3DXQUATERNION& quat) noexcept
{
	D3DXVECTOR3 rotation;
	rotation.x = atan2f(2.0f * (quat.y*quat.z + quat.w*quat.x), quat.w*quat.w - quat.x*quat.x - quat.y*quat.y + quat.z*quat.z);
	rotation.y = asinf(-2.0f * (quat.x*quat.z - quat.w*quat.y));
	rotation.z = atan2f(2.0f * (quat.x*quat.y + quat.w*quat.z), quat.w*quat.w + quat.x*quat.x - quat.y*quat.y - quat.z*quat.z);
	return rotation;
}