#pragma once
#include"EspString.hpp"

typedef signed short Int16;
typedef unsigned short UInt16;
typedef signed int Int32;
typedef unsigned int UInt32;
typedef signed long long Int64;
typedef unsigned long long UInt64;

class EspConvert
{
public:
	static void ToString(int IntValue, EspString& StringValue)
	{
		StringValue.Empty();
		if (IntValue == 0)
		{
			StringValue.Append('0');
			return;
		}
		bool IsNeg = IntValue < 0;
		if (IsNeg)
		{
			IntValue = -IntValue;
		}
		while (IntValue != 0)
		{
			StringValue.Append((IntValue % 10) + 48);
			IntValue /= 10;
		}
		if (IsNeg)StringValue.Append('-');
		StringValue.Reverse();
	}
	static int ParseInt(EspString StringValue)
	{
		if (StringValue.IsEmptyOrNull())
			return 0;
		const char* Pos = StringValue.GetAnsiStr();
		bool IsNeg = false;
		if (*Pos == '+')Pos++;
		else if (*Pos == '-')
		{
			Pos++;
			IsNeg = true;
		}
		int Result = 0;
		while (*Pos >= '0' && *Pos <= '9')
			Result = Result * 10 + (*Pos - 48), Pos++;
		if (IsNeg)Result = -Result;
		return Result;
	}
	static void ToString(unsigned int UIntValue, EspString& StringValue)
	{
		StringValue.Empty();
		if (UIntValue == 0)
		{
			StringValue.Append('0');
			return;
		}
		while (UIntValue != 0)
		{
			StringValue.Append((UIntValue % 10) + 48);
			UIntValue /= 10;
		}
		StringValue.Reverse();
	}
	static unsigned int ParseUInt(EspString StringValue)
	{
		if (StringValue.IsEmptyOrNull())
			return 0;
		const char* Pos = StringValue.GetAnsiStr();
		if (*Pos == '+' || *Pos == '-')Pos++;
		unsigned int Result = 0;
		while (*Pos >= '0' && *Pos <= '9')
			Result = Result * 10 + (*Pos - 48), Pos++;
		return Result;
	}
	static void ToString(long LongValue, EspString& StringValue)
	{
		StringValue.Empty();
		if (LongValue == 0)
		{
			StringValue.Append('0');
			return;
		}
		bool IsNeg = LongValue < 0;
		if (IsNeg)
			LongValue = -LongValue;
		while (LongValue != 0)
		{
			StringValue.Append((LongValue % 10) + 48);
			LongValue /= 10;
		}
		if (IsNeg)StringValue.Append('-');
		StringValue.Reverse();
	}
	static long ParseLong(EspString StringValue)
	{
		if (StringValue.IsEmptyOrNull())
			return 0;
		const char* Pos = StringValue.GetAnsiStr();
		bool IsNeg = false;
		if (*Pos == '+')Pos++;
		else if (*Pos == '-')
		{
			Pos++;
			IsNeg = true;
		}
		long Result = 0;
		while (*Pos >= '0' && *Pos <= '9')
			Result = Result * 10 + (*Pos - 48), Pos++;
		if (IsNeg)Result = -Result;
		return Result;
	}
	static void ToString(unsigned long ULongValue, EspString& StringValue)
	{
		StringValue.Empty();
		if (ULongValue == 0)
		{
			StringValue.Append('0');
			return;
		}
		while (ULongValue != 0)
		{
			StringValue.Append((ULongValue % 10) + 48);
			ULongValue /= 10;
		}
		StringValue.Reverse();
	}
	static unsigned long ParseULong(EspString StringValue)
	{
		if (StringValue.IsEmptyOrNull())
			return 0;
		const char* Pos = StringValue.GetAnsiStr();
		if (*Pos == '+' || *Pos == '-')Pos++;
		unsigned long Result = 0;
		while (*Pos >= '0' && *Pos <= '9')
			Result = Result * 10 + (*Pos - 48), Pos++;
		return Result;
	}
	static void ToString(long long LongLongValue, EspString& StringValue)
	{
		StringValue.Empty();
		if (LongLongValue == 0)
		{
			StringValue.Append('0');
			return;
		}
		bool IsNeg = LongLongValue < 0;
		if (IsNeg)
			LongLongValue = -LongLongValue;
		while (LongLongValue != 0)
		{
			StringValue.Append((LongLongValue % 10) + 48);
			LongLongValue /= 10;
		}
		if (IsNeg)StringValue.Append('-');
		StringValue.Reverse();
	}
	static long long ParseLongLong(EspString StringValue)
	{
		if (StringValue.IsEmptyOrNull())
			return 0;
		const char* Pos = StringValue.GetAnsiStr();
		bool IsNeg = false;
		if (*Pos == '+')Pos++;
		else if (*Pos == '-')
		{
			Pos++;
			IsNeg = true;
		}
		long long Result = 0;
		while (*Pos >= '0' && *Pos <= '9')
			Result = Result * 10 + (*Pos - 48), Pos++;
		if (IsNeg)Result = -Result;
		return Result;
	}
	static void ToString(unsigned long long ULongLongValue, EspString& StringValue)
	{
		StringValue.Empty();
		if (ULongLongValue == 0)
		{
			StringValue.Append('0');
			return;
		}
		while (ULongLongValue != 0)
		{
			StringValue.Append((ULongLongValue % 10) + 48);
			ULongLongValue /= 10;
		}
		StringValue.Reverse();
	}
	static unsigned long long ParseULongLong(EspString StringValue)
	{
		if (StringValue.IsEmptyOrNull())
			return 0;
		const char* Pos = StringValue.GetAnsiStr();
		if (*Pos == '+' || *Pos == '-')Pos++;
		unsigned long long Result = 0;
		while (*Pos >= '0' && *Pos <= '9')
			Result = Result * 10 + (*Pos - 48), Pos++;
		return Result;
	}

	static void ToString(float FloatValue, EspString& StringValue,unsigned short DecPartMaxLen=16)
	{
		StringValue.Empty();
		if (FloatValue == 0.0)
		{
			StringValue.Append('0');
			return;
		}
		bool IsNeg = FloatValue < 0;
		if (IsNeg)FloatValue = -FloatValue;
		while (FloatValue >= 1)
		{
			StringValue.Append(((int)FloatValue % 10) + 48);
			FloatValue /= 10;
		}
		if (IsNeg)
			StringValue.Append('-');
		StringValue.Reverse();
		if (FloatValue == 0.0)
			return;
		StringValue.Append('.');
		while (FloatValue != 0)
		{
			FloatValue *= 10;
			StringValue.Append(((int)(FloatValue) % 10) + 48);
		}
	}
	static float ParseFloat(EspString StringValue)
	{

	}
	static void ToString(double DoubleValue, EspString& StringValue, unsigned short DecPartMaxLen = 16)
	{
		StringValue.Empty();
		if (DoubleValue == 0.0)
		{
			StringValue.Append('0');
			return;
		}
		bool IsNeg = DoubleValue < 0;
		if (IsNeg)DoubleValue = -DoubleValue;
		while (DoubleValue >= 1)
		{
			StringValue.Append(((int)DoubleValue % 10) + 48);
			DoubleValue /= 10;
		}
		if (IsNeg)
			StringValue.Append('-');
		StringValue.Reverse();
		if (DoubleValue == 0.0)
			return;
		StringValue.Append('.');
		while (DecPartMaxLen--)
		{
			DoubleValue *= 10;
			StringValue.Append(((long long)(DoubleValue) % 10) + 48);
		}
	}
	static double ParseDouble(EspString StringValue)
	{

	}
};