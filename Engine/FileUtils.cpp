#include "pch.h"
#include "FileUtils.h"

FileUtils::FileUtils()
{

}

FileUtils::~FileUtils()
{
	// 핸들 정리
	if (_handle != INVALID_HANDLE_VALUE)
	{
		::CloseHandle(_handle);
		_handle = INVALID_HANDLE_VALUE;	
	}
}

void FileUtils::Open(wstring filePath, FileMode mode)
{
	// 쓰기 모드
	if (mode == FileMode::Write)
	{
		_handle = ::CreateFile(
			filePath.c_str(),
			GENERIC_WRITE,	// 파일을 열거나 생성함
			0,
			nullptr,
			CREATE_ALWAYS,	// 지정한 경로에 파일이 있으면 지우고 새로 만듦. 없으면 새로 생성
			FILE_ATTRIBUTE_NORMAL,
			nullptr
		);
	}

	// 읽기 모드
	else
	{
		_handle = ::CreateFile
		(
			filePath.c_str(),
			GENERIC_READ,
			FILE_SHARE_READ,
			nullptr,
			OPEN_EXISTING,	// 파일이 없으면 열지 않음따로 만들지 않음
			FILE_ATTRIBUTE_NORMAL,
			nullptr
		);
	}

	assert(_handle != INVALID_HANDLE_VALUE);
}

void FileUtils::Write(void* data, uint32 dataSize)
{
	uint32 numOfBytes = 0;
	// @param _handle : 파일에 대한 정보
	// @param data : 파일에 기록할 데이터
	// @param sizeof(T) : 파일에 쓸 데이터 크기
	// @param numOfBytes : 실제로 기록된 바이트 수
	assert(::WriteFile(_handle, data, dataSize, reinterpret_cast<LPDWORD>(&numOfBytes), nullptr));
}

void FileUtils::Write(const string& data)
{
	// 문자열 길이(문자 갯수)을 알아냄
	uint32 size = (uint32)data.size();
	Write(size); // uint32(4byte)를 통해 문자열의 길이 값을 읽음.

	if (data.size() == 0)
		return;
	
	// uint32(4byte)를 통해 얻은 값(문자 갯수) 만큼 string을 읽음
	Write((void*)data.data(), size); 
}

void FileUtils::Read(void** data, uint32 dataSize)
{
	uint32 numOfBytes = 0;
	assert(::ReadFile(_handle, *data, dataSize, reinterpret_cast<LPDWORD>(&numOfBytes), nullptr));
}

void FileUtils::Read(OUT string& data)
{
	// 사이즈
	uint32 size = Read<uint32>();

	if (size == 0)
		return;

	
	char* temp = new char[size + 1];	// 사이즈 만큼 동적 할당
	temp[size] = 0;				// 문자열 마지막은 NULL
	Read((void**)&temp, size);	// 문자열 읽기
	data = temp;				// 읽은 데이터를 string에 넣기
	delete[] temp;
}