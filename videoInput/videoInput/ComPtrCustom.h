#pragma once


typedef unsigned int UINT32;
typedef unsigned long ULONG;

template <class T>
class CComPtrCustom
{
public:

	CComPtrCustom()
		:element(nullptr)
	{
	}

	virtual ~CComPtrCustom()
	{
		Release();
	}

	void Release()
	{
		if (element == nullptr)
			return;

		ULONG e = -1;

		e = element->Release();

		//CoTaskMemFree(element);

		element = nullptr;
	}

	CComPtrCustom& operator = (T *pElement)
	{
		Release();

		ULONG e = -1;
		
		e = pElement->AddRef();

		element = pElement;

		return *this;
	}

	T* operator->()
	{
		return element;
	}

	operator T*()
	{
		return element;
	}

	T* get()
	{
		return element;
	}
	
	T** operator &()
	{
		return &element;
	}

	bool operator !()
	{
		return element != nullptr;
	}

	bool operator == (const T *pElement)
	{
		return element == pElement;
	}


	CComPtrCustom(const CComPtrCustom&) = delete;

	CComPtrCustom& operator = (const CComPtrCustom&) = delete;

private:

	UINT32 size;
	T* element;
};

