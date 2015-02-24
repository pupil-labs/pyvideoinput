#pragma once


typedef unsigned int UINT32;
typedef unsigned long ULONG;


template <class T>
class CComMassivPtr
{
public:

	CComMassivPtr(void)
		:size(0), elements(0)
	{
	}

	virtual ~CComMassivPtr(void)
	{
		release();
	}

	T** getMassivPtr()
	{
		return elements;
	}

	T*** getPtrMassivPtr()
	{
		return &elements;
	}

	UINT32 *setSizeMassivPtr()
	{
		return &size;
	}

	UINT32 getSizeMassivPtr()
	{
		return size;
	}
	
	void release()
	{
		ULONG e = -1;

		for(UINT32 index = 0; index < size; index++)
		{
		//	while(e)
				e = elements[index]->Release();
								
			//if (e != 0)
			//	throw std::exception("Error!! Massive is not empty!!");
		}
		
		CoTaskMemFree(elements);
	}

	T *operator [] (UINT32 index) const
	{
		if (index >= size)
			throw std::exception("Error!! Index out of size!!");

		return elements[index];
	}
	
private:
	
	UINT32 size;
	T** elements;
	
	CComMassivPtr(const CComMassivPtr&);
	
	CComMassivPtr& operator = (const CComMassivPtr&);	

};

