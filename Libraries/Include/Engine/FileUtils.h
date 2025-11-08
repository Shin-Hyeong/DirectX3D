#pragma once

// 파일을 읽거나 쓰거나

enum FileMode : uint8
{
	Write,
	Read
};


class FileUtils
{
public:
	FileUtils();
	~FileUtils();

	// 입출력용 핸들 생성
	void Open(wstring filePath, FileMode mode);

public:
	///////////
	// Wirte //
	///////////
	
	// Handle를 통해 전달한 데이터의 메모리를 파일에 쓰기
	// string와 같은 class로 이루어져 동적인 크기를 가진 데이터는 사용할 수없음
	// @param data : 파일에 쓸 데이터
	template<typename T>
	void Write(const T& data)
	{
		DWORD numOfBytes = 0;
		assert(::WriteFile(_handle, &data, sizeof(T), (LPDWORD)&numOfBytes, nullptr));
	}
	// 실수로 template에 string을 넣었을때 
	template<>
	void Write<string>(const string& data)
	{
		return Write(data);
	}
	
	// @param data : 파일에 쓸 데이터
	// @param dataSize: 데이터 크기, sizeof()
	void Write(void* data, uint32 dataSize);
	// String 전용
	void Write(const string& data);

	// 가변 데이터 쓰기
	// 1. 파일의 4바이트(uint32)에 문자열 길이를 데이터로 넣음. (Hello -> 5)
	// 2. 4바이트 다음 버퍼에 문자열 길이만큼 기록(저장)함.

public:
	//////////
	// Read //
	//////////



	// 데이터를 저장해서 반환하는 방식
	// @param data : 읽은 데이터를 저장할 매개변수
	template<typename T>
	void Read(OUT T& data)
	{
		DWORD numOfBytes = 0;
		assert(::ReadFile(_handle, &data, sizeof(T), (LPDWORD)&numOfBytes, nullptr));
	}

	// 데이터를 복사해서 반환하는 방식
	template<typename T>
	T Read()
	{
		T data;
		Read(data);
		return data;
	}

	// 데이터를 읽기
	// @param data : 읽은 데이터를 저장함
	void Read(void** data, uint32 dataSize);
	// String 타입 데이터 읽기
	void Read(OUT string& data);

	// 가변 길이 데이터(String) 읽는 과정
	// 1. 파일을 읽을때 처음 4바이트(uint32)만큼 읽은 후 값(저장된 문자열의 길이)을 확인한다.
	// 2. 값 만큼 버퍼를 생성해서 확인한 문자열 길이만큼 읽는다.


private:
	HANDLE _handle = INVALID_HANDLE_VALUE;
};

