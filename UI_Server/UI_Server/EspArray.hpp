#pragma once
#include<assert.h>
#include<memory>
#ifndef __ESPARRAY__
#define __ESPARRAY__
#endif
template<class EspType>
class EspArray
{
private:
	EspType* ArrayData;
	unsigned int ArraySize;
	unsigned int AllocSize;

public:
	EspArray()
	{
		ArrayData = NULL;
		ArraySize = AllocSize = 0;
	}
	~EspArray()
	{
		if (ArrayData != NULL)
		{
			for (unsigned int TimeNum = 0; TimeNum < ArraySize; TimeNum++)
				(ArrayData + TimeNum)->~EspType();
			::free(ArrayData);
		}
		ArrayData = NULL;
		ArraySize = AllocSize = 0;
	}

	void AddElement(const EspType& NewElement)
	{
		if (ArrayData == NULL)
		{
			ArrayData = (EspType*)::malloc(2 * sizeof(EspType));
			::new(ArrayData)EspType;
			//::memcpy(ArrayData, &NewElement, sizeof(EspType));
			ArrayData[0] = NewElement;
			::memset(ArrayData + 1, 0, sizeof(EspType));
			ArraySize = 1;
			AllocSize = 2;
		}
		else
		{
			unsigned int NewArraySize = ArraySize + 1;
			if (NewArraySize <= AllocSize)
			{
				::new(ArrayData + ArraySize)EspType;
				ArrayData[ArraySize] = NewElement;
				ArraySize = NewArraySize;
			}
			else
			{
				unsigned int NewAllocSize = NewArraySize * 2;
				EspType* NewArrayData = (EspType*)::malloc(NewAllocSize * sizeof(EspType));
				if (NewArrayData == NULL)
					throw("Allocate Buffer Unsuccessfully!");
				::memcpy(NewArrayData, ArrayData, ArraySize * sizeof(EspType));
				::new(NewArrayData + ArraySize)EspType;
				NewArrayData[ArraySize] = NewElement;
				::memset(NewArrayData + NewArraySize, 0, (NewAllocSize - NewArraySize) * sizeof(EspType));
				::free(ArrayData);
				ArrayData = NewArrayData;
				ArraySize = NewArraySize;
				AllocSize = NewAllocSize;
			}
		}
	}
	void AddArray(const EspArray<EspType>& NewArray)
	{
		if (ArrayData == NULL)
		{
			ArrayData = (EspType*)::malloc(NewArray.GetCount() * sizeof(EspType));
			for (unsigned int TimeNum = 0; TimeNum < NewArray.GetCount(); TimeNum++)
				::new(ArrayData + TimeNum)EspType;
			//::memcpy(ArrayData, &NewElement, sizeof(EspType));
			for (unsigned int TimeNum = 0; TimeNum < NewArray.GetCount(); TimeNum++)
				ArrayData[TimeNum] = NewArray.GetElementAt(TimeNum);
			ArraySize = NewArray.GetCount();
			AllocSize = NewArray.GetCount();
		}
		else
		{
			unsigned int NewArraySize = ArraySize + NewArray.GetCount();
			if (NewArraySize <= AllocSize)
			{
				for (unsigned int TimeNum = 0; TimeNum < NewArray.GetCount(); TimeNum++)
				{
					::new(ArrayData + ArraySize + TimeNum)EspType;
					ArrayData[ArraySize + TimeNum] = NewArray.GetElementAt(TimeNum);
				}
				ArraySize = NewArraySize;
			}
			else
			{
				unsigned int NewAllocSize = NewArraySize * 2;
				EspType* NewArrayData = (EspType*)::malloc(NewAllocSize * sizeof(EspType));
				if (NewArrayData == NULL)
					throw("Allocate Buffer Unsuccessfully!");
				::memcpy(NewArrayData, ArrayData, ArraySize * sizeof(EspType));
				for (unsigned int TimeNum = 0; TimeNum < NewArray.GetCount(); TimeNum++)
				{
					::new(NewArrayData + ArraySize + TimeNum)EspType;
					NewArrayData[ArraySize + TimeNum] = NewArray.GetElementAt(TimeNum);
				}
				::memset(NewArrayData + NewArraySize, 0, (NewAllocSize - NewArraySize) * sizeof(EspType));
				::free(ArrayData);
				ArrayData = NewArrayData;
				ArraySize = NewArraySize;
				AllocSize = NewAllocSize;
			}
		}
	}

	void DeleteElement(unsigned int Index, unsigned int Count = 1)
	{
		if (ArrayData != NULL)
		{
			assert(Index < ArraySize);
			unsigned int NewArraySize = ArraySize - Count;
			for (unsigned int TimeNum = 0; TimeNum < Count; TimeNum++)
				(ArrayData + Index + TimeNum)->~EspType();
			::memcpy(ArrayData + Index, ArrayData + Index + Count, (ArraySize - Index - Count) * sizeof(EspType));
			ArraySize = NewArraySize;
			::memset(ArrayData + NewArraySize, 0, sizeof(EspType));
		}
	}

	void InsertElementAt(unsigned int Index, const EspType& NewElement, unsigned int Count = 1)
	{
		if (ArrayData != NULL)
		{
			assert(Index < ArraySize);
			unsigned int NewArraySize = ArraySize + Count;
			if (NewArraySize <= AllocSize)
			{
				::memcpy(ArrayData + Index + Count, ArrayData + Index, (ArraySize - Index) * sizeof(EspType));
				for (unsigned int TimeNum = 0; TimeNum < Count; TimeNum++)
				{
					::new(ArrayData + Index + TimeNum)EspType;
					ArrayData[Index + TimeNum] = NewElement;
				}
				ArraySize = NewArraySize;
			}
			else
			{
				unsigned int NewAllocSize = NewArraySize * 2;
				EspType* NewArrayData = (EspType*)::malloc(NewAllocSize * sizeof(EspType));
				if (NewArrayData == NULL)
					throw("Allocate Buffer Unsuccessfully!");
				::memcpy(NewArrayData, ArrayData, (Index) * sizeof(EspType));
				for (unsigned int TimeNum = 0; TimeNum < Count; TimeNum++)
				{
					::new(NewArrayData + Index + TimeNum)EspType;
					NewArrayData[Index + TimeNum] = NewElement;
				}
				::memcpy(NewArrayData + Index + Count, ArrayData + Index, (ArraySize - Index) * sizeof(EspType));
				::memset(NewArrayData + NewArraySize, 0, (NewAllocSize - NewArraySize) * sizeof(EspType));
				::free(ArrayData);
				ArrayData = NewArrayData;
				ArraySize = NewArraySize;
				AllocSize = NewAllocSize;
			}
		}
	}
	void InsertArray(unsigned int Index, const EspArray<EspType>& NewArray)
	{
		if (ArrayData != NULL)
		{
			assert(Index < ArraySize);
			unsigned int NewArraySize = ArraySize + NewArray.GetCount();
			if (NewArraySize <= AllocSize)
			{
				::memcpy(ArrayData + Index + NewArray.GetCount(), ArrayData + Index, (ArraySize - Index) * sizeof(EspType));
				for (unsigned int TimeNum = 0; TimeNum < NewArray.GetCount(); TimeNum++)
				{
					::new(ArrayData + Index + TimeNum)EspType;
					ArrayData[Index + TimeNum] = NewArray.GetElementAt(TimeNum);
				}
				ArraySize = NewArraySize;
			}
			else
			{
				unsigned int NewAllocSize = NewArraySize * 2;
				EspType* NewArrayData = (EspType*)::malloc(NewAllocSize * sizeof(EspType));
				if (NewArrayData == NULL)
					throw("Allocate Buffer Unsuccessfully!");
				::memcpy(NewArrayData, ArrayData, Index * sizeof(EspType));
				for (unsigned int TimeNum = 0; TimeNum < NewArray.GetCount(); TimeNum++)
				{
					::new(NewArrayData + Index + TimeNum)EspType;
					NewArrayData[Index + TimeNum] = NewArray.GetElementAt(TimeNum);
				}
				::memcpy(NewArrayData + Index + NewArray.GetCount(), ArrayData + Index, (ArraySize - Index) * sizeof(EspType));
				::memset(NewArrayData + NewArraySize, 0, (NewAllocSize - NewArraySize) * sizeof(EspType));
				::free(ArrayData);
				ArrayData = NewArrayData;
				ArraySize = NewArraySize;
				AllocSize = NewAllocSize;
			}
		}
	}

	EspType& GetElementAt(unsigned int Index)const
	{
		assert(Index < ArraySize);
		return ArrayData[Index];
	}
	void SetElementAt(unsigned int Index, const EspType& NewElement)
	{
		assert(Index < ArraySize);
		ArrayData[Index] = NewElement;
	}
	unsigned int GetCount()const { return ArraySize; }
	unsigned int GetBufSize()const { return AllocSize; }
	unsigned int GetExtraSize()const { return AllocSize - ArraySize; }
	const EspType* GetBuffer()const { return ArrayData; }
	EspType* GetBuffer() { return ArrayData; }
	bool IsEmpty()const { return (ArraySize == 0); }
	bool IsEmptyOrNull()const { return(ArraySize == 0 || ArrayData == NULL); }
	bool IsFull()const { return (ArraySize == AllocSize); }

	void Empty()
	{
		if (ArrayData != NULL)
		{
			for (unsigned int TimeNum = 0; TimeNum < ArraySize; TimeNum++)
				(ArrayData + TimeNum)->~EspType();
			ArraySize = 0;
		}
	}
};
