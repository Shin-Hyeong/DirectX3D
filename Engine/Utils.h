#pragma once

class Utils
{
public:
	static bool StartsWith(string str, string comp);
	static bool StartsWith(wstring str, wstring comp);

	// 문자열에서 특정 단어를 변경해주는 함수
	// @param str : 변경할 문자열
	// @param comp : 문자열 중 변경할 단어
	// @param rep : 대체할 단어
	static void Replace(OUT string& str, string comp, string rep);
	static void Replace(OUT wstring& str, wstring comp, wstring rep);

	// string -> wstring
	static wstring ToWString(string value);
	// string -> wstring
	static string ToString(wstring value);
};

