#pragma once
#include<memory>
#ifndef __ESPSTRING__
#define __ESPSTRING__
#endif
class EspString
{
public:
	static char CharToUpper(const char C) { return (C >= 'a' && C <= 'a') ? (C - 'a' + 'A') : C; }
	static char CharToLower(const char C) { return (C >= 'A' && C <= 'Z') ? (C - 'A' + 'a') : C; }

	static unsigned int GetLength(const char* lpszStr);
	static unsigned int Find(const char* lpszStr, const char lpszChar, unsigned int nStartPos = 0);
	static unsigned int Find(const char* lpszStr, const char* lpszSub, unsigned int nStartPos = 0);
	static unsigned int ReverseFind(const char* lpszStr, const char lpszChar, unsigned int nEndPos = 0);
	static unsigned int ReverseFind(const char* lpszStr, const char* lpszSub, unsigned int nEndPos = 0);
	static bool Compare(const char* lpszStr1, const char* lpszStr2);
	static bool CompareNoCase(const char* lpszStr1, const char* lpszStr2);
	static void Reverse(char* lpszStr);

private:
	char* Buffer = nullptr;
	unsigned int StrLen = 0;
	unsigned int BufSize = 0;
public:
	EspString();
	EspString(unsigned int BufferSize, bool Doubled = false);
	EspString(const char* lpszNewStr, bool DoubledBuf = false);
	EspString(const EspString& lpszNewStr, bool DoubleBuf = false);
	~EspString();

	const char* GetAnsiStr()const;
	operator const char* ()const;
	char* GetBuffer()const;
	char* GetBuffer(unsigned int NewBufSize);
	EspString& RefreshLength() { StrLen = EspString::GetLength(Buffer); return *this; };
	char* GetBufferSetLength(unsigned int NewStrLen, bool Doubled = false);
	const unsigned int GetLength()const;
	const unsigned int GetUpperIndex()const;
	const unsigned int GetBufSize()const;
	bool IsEmpty()const;
	bool IsEmptyOrNull()const;
	bool IsFull()const;
	char& GetCharAt(unsigned int nIndex)const;
	char& operator[](unsigned int nIndex)const;

	unsigned int Find(const char lpszChar, unsigned int nStartPos = 0)const
	{
		return EspString::Find(Buffer, lpszChar, nStartPos);
	}
	unsigned int Find(const char* lpszStr, unsigned int nStartPos = 0)const;
	unsigned int Find(const EspString& lpszStr, unsigned int nStartPos = 0)const;
	unsigned int ReverseFind(const char lpszChar, unsigned int nStartPos = 0)const
	{
		return EspString::ReverseFind(Buffer, lpszChar, nStartPos);
	}
	unsigned int ReverseFind(const char* lpszStr, unsigned int nStartPos = 0)const;
	unsigned int ReverseFind(const EspString& lpszStr, unsigned int nStartPos = 0)const;
	bool Compare(const char* lpszStr)const;
	bool Compare(const EspString& lpszStr)const;
	bool CompareNoCase(const char* lpszStr)const;
	bool CompareNoCase(const EspString& lpszStr)const;

	EspString& Append(const char& lpszChar);
	EspString& Append(const char* lpszNewStr);
	EspString& Append(const EspString& lpszNewStr);

	EspString& operator+=(const char& lpszChar);
	EspString& operator+=(const char* lpszNewStr);
	EspString& operator+=(const EspString& lpszNewStr);

	EspString& Assign(const char* lpszNewStr);
	EspString& Assign(const EspString& lpszNewStr);

	EspString& operator=(const char* lpszNewStr);
	EspString& operator=(const EspString& lpszNewStr);

	EspString& Insert(unsigned int nIndex, const char& lpszChar, unsigned int nCount = 1);
	EspString& Insert(unsigned int nIndex, const char* lpszNewStr);
	EspString& Insert(unsigned int nIndex, const EspString& lpszNewStr);

	EspString& Remove(unsigned int nIndex, unsigned int nCount = 1);
	EspString& Remove(const char* lpszStr);
	EspString& Remove(const EspString& lpszStr);

	EspString& Replace(unsigned int nIndex, unsigned int nLength, const char* lpszNewStr);
	EspString& Replace(const char* lpszOldStr, const char* lpszNewStr);

	EspString& Reverse();

	void Empty()
	{
		if (Buffer != NULL)
		{
			::memset(Buffer, 0, BufSize * sizeof(char));
			StrLen = 0;
		}
	}

public:
	EspString Left(unsigned int nIndex)const;
	EspString Left(const EspString& lpszEndStr, unsigned int nStartPos = 0)const;
	EspString Right(unsigned int nIndex)const;
	EspString Right(const EspString& lpszStartStr, unsigned int nStartPos = 0)const;
	EspString Middle(unsigned int nIndex, unsigned int nCount)const;
	EspString Middle(const EspString& lpszStartStr, const EspString& lpszEndStr, unsigned int nStartPos = 0)const;

	static EspString ToString(const double Value, const unsigned int MaxLength = 10)
	{
		double Abs;
		EspString Result;
		if (Value < 0)
			Abs = -Value;
		else
			Abs = Value;
		while ((int)Abs != 0)
		{
			Result.Append((char)(((int)Abs % 10) + 48));
			Abs /= 10.0;
		}
		if (Result.IsEmpty())
			Result.Append('0');
		else
			Result.Reverse();
		double DecValue = Value - (int)Value;
		if (Value < 0)
			DecValue = -DecValue;
		else
			DecValue = DecValue;
		if (DecValue != 0.0)
		{
			Result.Append('.');
			while (Result.GetLength() < MaxLength)
			{
				DecValue *= 10.0;
				Result.Append((char)(((int)(DecValue) % 10) + 48));
			}
			unsigned int NonZeroPos = Result.GetLength() - 1;
			while (Result.GetCharAt(NonZeroPos) == '0')
				NonZeroPos--;
			if (NonZeroPos != Result.GetLength() - 1)
				Result.Remove(NonZeroPos + 1, Result.GetLength() - NonZeroPos - 1);
		}
		if (Value < 0)
			Result.Insert(0, '-');
		return Result;
	}
};

unsigned int EspString::GetLength(const char* lpszStr)
{
	const char* lpszInit = lpszStr;
	while (*lpszStr++);
	return (lpszStr - lpszInit - 1);
}
unsigned int EspString::Find(const char* lpszStr, const char lpszChar, unsigned int nStartPos)
{
	const char* lpszStrPos = lpszStr + nStartPos;
	while (*lpszStrPos)
	{
		if (*lpszStrPos == lpszChar)
			return lpszStrPos - lpszStr;
		lpszStrPos++;
	}
	return -1;
}
unsigned int EspString::Find(const char* lpszStr, const char* lpszSub, unsigned int nStartPos)
{
	if ((lpszStr + nStartPos) == NULL || lpszSub == NULL)
		return -1;
	const char* lpszStrPos = lpszStr + nStartPos;
	const char* lpszSubPos = lpszSub;
	const char* lpszTargetPos = NULL;
	while (*lpszStrPos)
	{
		lpszTargetPos = lpszStrPos;
		while (*lpszStrPos && *lpszSubPos && *lpszStrPos == *lpszSubPos)
			lpszStrPos++, lpszSubPos++;
		if (*lpszSubPos == 0)
			return lpszTargetPos - lpszStr;
		lpszSubPos = lpszSub;
		lpszStrPos++;
	}
	return -1;
}
unsigned int EspString::ReverseFind(const char* lpszStr, const char lpszChar, unsigned int nEndPos)
{
	const char* lpszStrPos = lpszStr + nEndPos;
	const char* lpszLastPos = NULL;
	while (*lpszStrPos)
	{
		if (*lpszStrPos == lpszChar)
			lpszLastPos = lpszStrPos;
		lpszStrPos++;
	}
	return lpszLastPos != NULL ? lpszLastPos - lpszStr : -1;
}
unsigned int EspString::ReverseFind(const char* lpszStr, const char* lpszSub, unsigned int nEndPos)
{
	if ((lpszStr + nEndPos) == NULL || lpszSub == NULL)
		return -1;
	const char* lpszStrPos = lpszStr + nEndPos;
	const char* lpszSubPos = lpszSub;
	const char* lpszPos = NULL;
	const char* lpszLastPos = NULL;
	while (*lpszStrPos)
	{
		lpszPos = lpszStrPos;
		while (*lpszStrPos && *lpszSubPos && *lpszStrPos == *lpszSubPos)
			lpszStrPos++, lpszSubPos++;
		if (*lpszSubPos == 0)
			lpszLastPos = lpszPos;
		lpszSubPos = lpszSub;
		lpszStrPos++;
	}
	return lpszLastPos != NULL ? lpszLastPos - lpszStr : -1;
}
bool EspString::Compare(const char* lpszStr1, const char* lpszStr2)
{
	if (lpszStr1 == NULL || lpszStr2 == NULL)
		return false;
	while (*lpszStr1 && (*lpszStr1 == *lpszStr2))lpszStr1++, lpszStr2++;
	return (*lpszStr1 - *lpszStr2) == 0;
}
bool EspString::CompareNoCase(const char* lpszStr1, const char* lpszStr2)
{
	if (lpszStr1 == NULL || lpszStr2 == NULL)
		return false;
	while (*lpszStr1 && (EspString::CharToLower(*lpszStr1) == EspString::CharToLower(*lpszStr2)))lpszStr1++, lpszStr2++;
	return (EspString::CharToLower(*lpszStr1) - EspString::CharToLower(*lpszStr2)) == 0;
}
void EspString::Reverse(char* lpszStr)
{
	char* lpszStart, * lpszEnd;
	if (!lpszStr || !*lpszStr)return;
	for (lpszStart = lpszStr, lpszEnd = lpszStr + EspString::GetLength(lpszStr) - 1; lpszEnd > lpszStart; ++lpszStart, --lpszEnd)
	{
		*lpszStart ^= *lpszEnd;
		*lpszEnd ^= *lpszStart;
		*lpszStart ^= *lpszEnd;
	}
}

EspString::EspString() {}
EspString::EspString(unsigned int BufferSize, bool Doubled)
{
	unsigned int NewBufSize = BufferSize;
	if (Doubled)
		NewBufSize *= 2;
	Buffer = (char*)::malloc(NewBufSize * sizeof(char));
	if (Buffer == NULL)
		throw("Allocate Buffer Unsuccessfully");
	::memset(Buffer, 0, NewBufSize * sizeof(char));
	StrLen = 0;
	BufSize = NewBufSize;
}
EspString::EspString(const char* lpszNewStr, bool DoubledBuf)
{
	if (lpszNewStr == NULL)
	{
		Buffer = NULL;
		BufSize = StrLen = 0;
		return;
	}
	unsigned int NewStrLen = EspString::GetLength(lpszNewStr);
	unsigned int NewBufSize = NewStrLen + 1;
	if (DoubledBuf)
		NewBufSize *= 2;
	Buffer = (char*)::malloc(NewBufSize * sizeof(char));
	if (Buffer == NULL)
		throw("Allocate Buffer Unsuccessfully");
	::memcpy(Buffer, lpszNewStr, NewStrLen * sizeof(char));
	::memset(Buffer + NewStrLen, 0, (NewBufSize - NewStrLen) * sizeof(char));
	StrLen = NewStrLen;
	BufSize = NewBufSize;
}
EspString::EspString(const EspString& lpszNewStr, bool DoubleBuf)
{
	if (lpszNewStr.Buffer == NULL)
	{
		Buffer = NULL;
		BufSize = StrLen = 0;
		return;
	}
	unsigned int NewStrLen = lpszNewStr.StrLen;
	unsigned int NewBufSize = NewStrLen + 1;
	if (DoubleBuf)
		NewBufSize *= 2;
	Buffer = (char*)::malloc(NewBufSize * sizeof(char));
	if (Buffer == NULL)
		throw("Allocate Buffer Unsuccessfully");
	::memcpy(Buffer, lpszNewStr.Buffer, NewStrLen * sizeof(char));
	::memset(Buffer + NewStrLen, 0, (NewBufSize - NewStrLen) * sizeof(char));
	StrLen = NewStrLen;
	BufSize = NewBufSize;
}
EspString::~EspString()
{
	if (Buffer != NULL)
		::free(Buffer);
	Buffer = NULL;
}

const char* EspString::GetAnsiStr()const { return Buffer; }
EspString::operator const char* ()const { return Buffer; }
char* EspString::GetBuffer()const { return Buffer; }
char* EspString::GetBuffer(unsigned int NewBufSize)
{
	if (Buffer == NULL)
	{
		Buffer = (char*)::malloc(NewBufSize * sizeof(char));
		if (Buffer == NULL)
			throw("Allocate Buffer Unsuccessfully");
		::memset(Buffer, 0, NewBufSize * sizeof(char));
		BufSize = NewBufSize;
	}
	else if (NewBufSize >= BufSize)
	{
		char* NewBuffer = (char*)::malloc(NewBufSize * sizeof(char));
		if (NewBuffer == NULL)
			throw("Allocate Buffer Unsuccessfully");
		::memcpy(NewBuffer, Buffer, StrLen * sizeof(char));
		::memset(NewBuffer + StrLen, 0, (NewBufSize - StrLen) * sizeof(char));
		::free(Buffer);
		Buffer = NewBuffer;
		BufSize = NewBufSize;
	}
	return Buffer;
}
char* EspString::GetBufferSetLength(unsigned int NewStrLen, bool Doubled)
{
	StrLen = NewStrLen++;
	NewStrLen = Doubled ? NewStrLen * 2 : NewStrLen;
	return GetBuffer(NewStrLen);
}
const unsigned int EspString::GetLength()const { return StrLen; }
const unsigned int EspString::GetUpperIndex()const { return StrLen - 1; }
const unsigned int EspString::GetBufSize()const { return BufSize; }
bool EspString::IsEmpty()const { return StrLen == 0; }
bool EspString::IsEmptyOrNull()const { return (StrLen == 0 || Buffer == NULL); }
bool EspString::IsFull()const { return StrLen + 1 == BufSize; }
char& EspString::GetCharAt(unsigned int nIndex)const
{
	if (nIndex >= 0 && nIndex < GetLength())
		return Buffer[nIndex];
}
char& EspString::operator[](unsigned int nIndex)const { return GetCharAt(nIndex); }

unsigned int EspString::Find(const char* lpszStr, unsigned int nStartPos)const { return EspString::Find(Buffer, lpszStr, nStartPos); }
unsigned int EspString::Find(const EspString& lpszStr, unsigned int nStartPos)const { return EspString::Find(Buffer, lpszStr.Buffer, nStartPos); }
unsigned int EspString::ReverseFind(const char* lpszStr, unsigned int nStartPos)const { return EspString::ReverseFind(Buffer, lpszStr, nStartPos); }
unsigned int EspString::ReverseFind(const EspString& lpszStr, unsigned int nStartPos)const { return EspString::ReverseFind(Buffer, lpszStr.Buffer, nStartPos); }
bool EspString::Compare(const char* lpszStr)const { return EspString::Compare(Buffer, lpszStr); }
bool EspString::Compare(const EspString& lpszStr)const { return EspString::Compare(Buffer, lpszStr.Buffer); }
bool EspString::CompareNoCase(const char* lpszStr)const { return EspString::CompareNoCase(Buffer, lpszStr); }
bool EspString::CompareNoCase(const EspString& lpszStr)const { return EspString::CompareNoCase(Buffer, lpszStr.Buffer); }

EspString& EspString::Append(const char& lpszChar)
{
	if (Buffer == NULL)
	{
		Buffer = (char*)::malloc(10);
		if (Buffer == NULL)
			throw("Allocate Buffer Unsuccessfully");
		::memcpy(Buffer, &lpszChar, sizeof(char));
		::memset(Buffer + 1, 0, 9 * sizeof(char));
		StrLen = 1;
		BufSize = 10;
	}
	else
	{
		unsigned int NewStrLen = StrLen + 1;
		if (NewStrLen < BufSize)
		{
			::memcpy(Buffer + StrLen, &lpszChar, sizeof(char));
			StrLen++;
		}
		else
		{
			unsigned int NewBufLen = NewStrLen * 2;
			char* NewStrData = (char*)::malloc(NewBufLen * sizeof(char));
			if (NewStrData == NULL)
				throw("Allocate Buffer Unsuccessfully");
			::memcpy_s(NewStrData, NewBufLen * sizeof(char), Buffer, StrLen * sizeof(char));
			::memcpy(NewStrData + StrLen, &lpszChar, sizeof(char));
			::memset(NewStrData + NewStrLen, 0, (NewBufLen - NewStrLen) * sizeof(char));
			StrLen = NewStrLen;
			BufSize = NewBufLen;
			::free(Buffer);
			Buffer = NewStrData;
		}
	}
	return *this;
}
EspString& EspString::Append(const char* lpszNewStr)
{
	if (lpszNewStr != NULL)
	{
		unsigned int NewStrLen = EspString::GetLength(lpszNewStr);
		if (Buffer == NULL)
		{
			unsigned int NewBufSize = NewStrLen + 1;
			Buffer = (char*)::malloc(NewBufSize * sizeof(char));
			if (Buffer == NULL)
				throw("Allocate Buffer Unsuccessfully");
			::memcpy(Buffer, lpszNewStr, NewStrLen * sizeof(char));
			Buffer[NewStrLen] = '\0';
			StrLen = NewStrLen;
			BufSize = NewBufSize;
		}
		else
		{
			unsigned int TotalStrLen = StrLen + NewStrLen;
			if (TotalStrLen < BufSize)
			{
				::memcpy(Buffer + StrLen, lpszNewStr, NewStrLen * sizeof(char));
				StrLen = TotalStrLen;
			}
			else
			{
				unsigned int NewBufSize = TotalStrLen * 2;
				char* NewBuffer = (char*)::malloc(NewBufSize * sizeof(char));
				if (NewBuffer == NULL)
					throw("Allocate Buffer Unsuccessfully");
				::memcpy(NewBuffer, Buffer, StrLen * sizeof(char));
				::memcpy(NewBuffer + StrLen, lpszNewStr, NewStrLen * sizeof(char));
				::memset(NewBuffer + TotalStrLen, 0, (NewBufSize - TotalStrLen) * sizeof(char));
				::free(Buffer);
				Buffer = NewBuffer;
				StrLen = TotalStrLen;
				BufSize = NewBufSize;
			}
		}
	}
	return *this;
}
EspString& EspString::Append(const EspString& lpszNewStr)
{
	if (lpszNewStr != NULL)
	{
		unsigned int NewStrLen = lpszNewStr.StrLen;
		if (Buffer == NULL)
		{
			unsigned int NewBufSize = NewStrLen + 1;
			Buffer = (char*)::malloc(NewBufSize * sizeof(char));
			if (Buffer == NULL)
				throw("Allocate Buffer Unsuccessfully");
			::memcpy(Buffer, lpszNewStr.Buffer, NewStrLen * sizeof(char));
			Buffer[NewStrLen] = '\0';
			StrLen = NewStrLen;
			BufSize = NewBufSize;
		}
		else
		{
			unsigned int TotalStrLen = StrLen + NewStrLen;
			if (TotalStrLen < BufSize)
			{
				::memcpy(Buffer + StrLen, lpszNewStr.Buffer, NewStrLen * sizeof(char));
				StrLen = TotalStrLen;
			}
			else
			{
				unsigned int NewBufSize = TotalStrLen * 2;
				char* NewBuffer = (char*)::malloc(NewBufSize * sizeof(char));
				if (NewBuffer == NULL)
					throw("Allocate Buffer Unsuccessfully");
				::memcpy(NewBuffer, Buffer, StrLen * sizeof(char));
				::memcpy(NewBuffer + StrLen, lpszNewStr.Buffer, NewStrLen * sizeof(char));
				::memset(NewBuffer + TotalStrLen, 0, (NewBufSize - TotalStrLen) * sizeof(char));
				::free(Buffer);
				Buffer = NewBuffer;
				StrLen = TotalStrLen;
				BufSize = NewBufSize;
			}
		}
	}
	return *this;
}

EspString& EspString::operator+=(const char& lpszChar) { return Append(lpszChar); }
EspString& EspString::operator+=(const char* lpszNewStr) { return Append(lpszNewStr); }
EspString& EspString::operator+=(const EspString& lpszNewStr) { return Append(lpszNewStr); }

EspString& EspString::Assign(const char* lpszNewStr)
{
	if (lpszNewStr != NULL)
	{
		unsigned int NewStrLen = EspString::GetLength(lpszNewStr);
		if (Buffer == NULL)
		{
			unsigned int NewBufSize = NewStrLen + 1;
			Buffer = (char*)::malloc(NewBufSize * sizeof(char));
			if (Buffer == NULL)
				throw("Allocate Buffer Unsuccessfully");
			::memcpy(Buffer, lpszNewStr, NewStrLen * sizeof(char));
			::memset(Buffer + NewStrLen, 0, (NewBufSize - NewStrLen) * sizeof(char));
			StrLen = NewStrLen;
			BufSize = NewBufSize;
		}
		else
		{
			if (NewStrLen < BufSize)
			{
				::memset(Buffer, 0, StrLen * sizeof(char));
				::memcpy(Buffer, lpszNewStr, NewStrLen * sizeof(char));
				StrLen = NewStrLen;
			}
			else
			{
				unsigned int NewBufSize = NewStrLen * 2;
				::free(Buffer);
				Buffer = (char*)::malloc(NewBufSize * sizeof(char));
				if (Buffer == NULL)
					throw("Allocate Buffer Unsuccessfully");
				::memcpy(Buffer, lpszNewStr, NewStrLen * sizeof(char));
				::memset(Buffer + NewStrLen, 0, (NewBufSize - NewStrLen) * sizeof(char));
				StrLen = NewStrLen;
				BufSize = NewBufSize;
			}
		}
	}
	return *this;
}
EspString& EspString::Assign(const EspString& lpszNewStr)
{
	if (lpszNewStr != NULL)
	{
		unsigned int NewStrLen = lpszNewStr.StrLen;
		if (Buffer == NULL)
		{
			unsigned int NewBufSize = NewStrLen + 1;
			Buffer = (char*)::malloc(NewBufSize * sizeof(char));
			if (Buffer == NULL)
				throw("Allocate Buffer Unsuccessfully");
			::memcpy(Buffer, lpszNewStr.Buffer, NewStrLen * sizeof(char));
			::memset(Buffer + NewStrLen, 0, (NewBufSize - NewStrLen) * sizeof(char));
			StrLen = NewStrLen;
			BufSize = NewBufSize;
		}
		else
		{
			if (NewStrLen < BufSize)
			{
				::memset(Buffer, 0, StrLen * sizeof(char));
				::memcpy(Buffer, lpszNewStr.Buffer, NewStrLen * sizeof(char));
				StrLen = NewStrLen;
			}
			else
			{
				unsigned int NewBufSize = NewStrLen * 2;
				::free(Buffer);
				Buffer = (char*)::malloc(NewBufSize * sizeof(char));
				if (Buffer == NULL)
					throw("Allocate Buffer Unsuccessfully");
				::memcpy(Buffer, lpszNewStr.Buffer, NewStrLen * sizeof(char));
				::memset(Buffer + NewStrLen, 0, (NewBufSize - NewStrLen) * sizeof(char));
				StrLen = NewStrLen;
				BufSize = NewBufSize;
			}
		}
	}
	return *this;
}

EspString& EspString::operator=(const char* lpszNewStr) { return Assign(lpszNewStr); }
EspString& EspString::operator=(const EspString& lpszNewStr) { return Assign(lpszNewStr); }

EspString& EspString::Insert(unsigned int nIndex, const char& lpszChar, unsigned int nCount)
{
	if (nIndex > StrLen)
		nIndex = StrLen;
	unsigned int TotalStrLen = StrLen + nCount;
	if (TotalStrLen < BufSize)
	{
		::memcpy(Buffer + nIndex + nCount, Buffer + nIndex, (TotalStrLen - nCount - nIndex) * sizeof(char));
		for (unsigned int TimeNum = 0; TimeNum < nCount; TimeNum++)
			Buffer[nIndex + TimeNum] = lpszChar;
		StrLen += nCount;
	}
	else
	{
		unsigned int NewBufSize = TotalStrLen * 2;
		char* NewBuffer = (char*)::malloc(NewBufSize * sizeof(char));
		if (NewBuffer == NULL)
			throw("Allocate Buffer Unsuccessfully");
		::memcpy(NewBuffer, Buffer, nIndex * sizeof(char));
		for (unsigned int TimeNum = 0; TimeNum < nCount; TimeNum++)
			NewBuffer[nIndex + TimeNum] = lpszChar;
		::memcpy(NewBuffer + nIndex + nCount, Buffer + nIndex, (TotalStrLen - nCount - nIndex) * sizeof(char));
		::memset(NewBuffer + TotalStrLen, 0, (NewBufSize - TotalStrLen) * sizeof(char));
		::free(Buffer);
		Buffer = NewBuffer;
		StrLen = TotalStrLen;
		BufSize = NewBufSize;
	}
	return *this;
}
EspString& EspString::Insert(unsigned int nIndex, const char* lpszNewStr)
{
	if (lpszNewStr != NULL)
	{
		if (nIndex > StrLen)
			nIndex = StrLen;
		unsigned int NewStrLen = EspString::GetLength(lpszNewStr);
		unsigned int TotalStrLen = StrLen + NewStrLen;
		if (TotalStrLen < BufSize)
		{
			::memcpy(Buffer + nIndex + NewStrLen, Buffer + nIndex, (TotalStrLen - nIndex - NewStrLen) * sizeof(char));
			::memcpy(Buffer + nIndex, lpszNewStr, NewStrLen * sizeof(char));
			StrLen = TotalStrLen;
		}
		else
		{
			unsigned int NewBufSize = TotalStrLen * 2;
			char* NewBuffer = (char*)::malloc(NewBufSize * sizeof(char));
			if (NewBuffer == NULL)
				throw("Allocate Buffer Unsuccessfully");
			::memcpy(NewBuffer, Buffer, nIndex * sizeof(char));
			::memcpy(NewBuffer + nIndex + NewStrLen, Buffer + nIndex, (TotalStrLen - NewStrLen - nIndex) * sizeof(char));
			::memcpy(NewBuffer + nIndex, lpszNewStr, NewStrLen * sizeof(char));
			::memset(NewBuffer + TotalStrLen, 0, (NewBufSize - TotalStrLen) * sizeof(char));
			::free(Buffer);
			Buffer = NewBuffer;
			StrLen = TotalStrLen;
			BufSize = NewBufSize;
		}
	}
	return *this;
}
EspString& EspString::Insert(unsigned int nIndex, const EspString& lpszNewStr)
{
	if (!lpszNewStr.IsEmpty())
	{
		if (nIndex > StrLen)
			nIndex = StrLen;
		unsigned int NewStrLen = lpszNewStr.StrLen;
		unsigned int TotalStrLen = StrLen + NewStrLen;
		if (TotalStrLen < BufSize)
		{
			::memcpy(Buffer + nIndex + NewStrLen, Buffer + nIndex, (TotalStrLen - nIndex - NewStrLen) * sizeof(char));
			::memcpy(Buffer + nIndex, lpszNewStr.Buffer, NewStrLen * sizeof(char));
			StrLen = TotalStrLen;
		}
		else
		{
			unsigned int NewBufSize = TotalStrLen * 2;
			char* NewBuffer = (char*)::malloc(NewBufSize * sizeof(char));
			if (NewBuffer == NULL)
				throw("Allocate Buffer Unsuccessfully");
			::memcpy(NewBuffer, Buffer, nIndex * sizeof(char));
			::memcpy(NewBuffer + nIndex + NewStrLen, Buffer + nIndex, (TotalStrLen - NewStrLen - nIndex) * sizeof(char));
			::memcpy(NewBuffer + nIndex, lpszNewStr.Buffer, NewStrLen * sizeof(char));
			::memset(NewBuffer + TotalStrLen, 0, (NewBufSize - TotalStrLen) * sizeof(char));
			::free(Buffer);
			Buffer = NewBuffer;
			StrLen = TotalStrLen;
			BufSize = NewBufSize;
		}
	}
	return *this;
}

EspString& EspString::Remove(unsigned int nIndex, unsigned int nCount)
{
	if (nIndex > StrLen - 1)
	{
		nIndex = StrLen - 1;
		nCount = 1;
	}
	unsigned int NewStrLen = StrLen - nCount;
	if (nCount > 0)
	{
		::memcpy(Buffer + nIndex, Buffer + nIndex + nCount, ::abs((signed int)(NewStrLen - (nIndex + nCount) + 1)) * sizeof(char));
		::memset(Buffer + NewStrLen, 0, (BufSize - NewStrLen) * sizeof(char));
		StrLen = NewStrLen;
	}
	return *this;
}

EspString& EspString::Replace(unsigned int nIndex, unsigned int nLength, const char* lpszNewStr)
{
	if (Buffer != NULL && lpszNewStr != NULL)
	{
		unsigned int NewStrLen = EspString::GetLength(lpszNewStr);
		unsigned int TotalStrLen = StrLen - nLength + NewStrLen;
		if (TotalStrLen < BufSize)
		{
			::memcpy(Buffer + nIndex + NewStrLen, Buffer + nIndex + nLength, (StrLen - nIndex - nLength + 1) * sizeof(char));
			::memcpy(Buffer + nIndex, lpszNewStr, NewStrLen * sizeof(char));
			StrLen = TotalStrLen;
		}
		else
		{
			unsigned int NewBufSize = TotalStrLen * 2;
			char* NewBuffer = (char*)::malloc(NewBufSize * sizeof(char));
			if (NewBuffer == NULL)
				throw("Allocate Buffer Unsuccessfully");
			::memcpy(NewBuffer, Buffer, nIndex * sizeof(char));
			::memcpy(NewBuffer + nIndex, lpszNewStr, NewStrLen * sizeof(char));
			::memcpy(NewBuffer + nIndex + NewStrLen, Buffer + nIndex + nLength, (StrLen - nIndex - nLength + 1) * sizeof(char));
			::memset(NewBuffer + TotalStrLen, 0, (NewBufSize - TotalStrLen) * sizeof(char));
			::free(Buffer);
			Buffer = NewBuffer;
			StrLen = TotalStrLen;
			BufSize = NewBufSize;
		}
	}
	return *this;
}
EspString& EspString::Replace(const char* lpszOldStr, const char* lpszNewStr)
{
	unsigned int TargetPos = 0;
	unsigned int OldStrLen = EspString::GetLength(lpszOldStr);
	unsigned int NewStrLen = EspString::GetLength(lpszNewStr);
	while ((TargetPos = EspString::Find(Buffer, lpszOldStr, TargetPos)) != -1)
	{
		Replace(TargetPos, OldStrLen, lpszNewStr);
		TargetPos += NewStrLen;
	}
	return *this;
}

EspString& EspString::Reverse()
{
	EspString::Reverse(Buffer);
	return *this;
}

EspString EspString::Left(unsigned int nIndex)const
{
	EspString Result;
	::memcpy(Result.GetBufferSetLength(nIndex, false), Buffer, nIndex * sizeof(char));
	return Result;
}
EspString EspString::Left(const EspString& lpszEndStr, unsigned int nStartPos)const
{
	return Left(EspString::Find(Buffer, lpszEndStr, nStartPos));
}
EspString EspString::Right(unsigned int nIndex)const
{
	EspString Result;
	::memcpy(Result.GetBufferSetLength(nIndex, false), Buffer + StrLen - nIndex, nIndex * sizeof(char));
	return Result;
}
EspString EspString::Right(const EspString& lpszStartStr, unsigned int nStartPos)const
{
	return Right(StrLen - EspString::ReverseFind(Buffer, lpszStartStr, nStartPos));
}
EspString EspString::Middle(unsigned int nIndex, unsigned int nCount)const
{
	EspString Result;
	::memcpy(Result.GetBufferSetLength(nCount, false), Buffer + nIndex, nCount * sizeof(char));
	return Result;
}
EspString EspString::Middle(const EspString& lpszStartStr, const EspString& lpszEndStr, unsigned int nStartPos)const
{
	EspString Result;
	unsigned int Pos_Left = EspString::Find(Buffer, lpszStartStr, nStartPos) + lpszStartStr.StrLen;
	unsigned int Pos_Right = EspString::Find(Buffer, lpszEndStr, Pos_Left) - Pos_Left;
	::memcpy(Result.GetBufferSetLength(Pos_Right, false), Buffer + Pos_Left, Pos_Right * sizeof(char));
	return Result;
}
#include"EspArray.hpp"
void EspSplitString(const EspString& lpszStr, const char lpszSymbol, EspArray<EspString>& Result, unsigned int nCount = 0)
{
	unsigned int nStartPos = 0;
	unsigned int nEndPos = lpszStr.Find(lpszSymbol);
	while (true)
	{
		Result.AddElement(lpszStr.Middle(nStartPos, nEndPos - nStartPos));
		nStartPos = nEndPos + 1;
		nEndPos = lpszStr.Find(lpszSymbol, nStartPos);
		if (nEndPos == -1)
		{
			Result.AddElement(lpszStr.Middle(nStartPos,lpszStr.GetLength()-nEndPos));
			break;
		}
	}
}
