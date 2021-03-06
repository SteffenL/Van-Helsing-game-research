
//         Copyright Eóin O'Callaghan 2006 - 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#define HAL_INF							40203

#include <atlddx.h>
#include <string>

#define DDX_EX_FLOAT_POSITIVE(nID, var) \
		if(nCtlID == (UINT)-1 || nCtlID == nID) \
		{ \
			if(!DDX_Numeric_Inf<float>(nID, var, bSaveAndValidate)) \
				return FALSE; \
		}

#define DDX_EX_INT_FLOAT_LIMIT(nID, var, lower, include) \
		if(nCtlID == (UINT)-1 || nCtlID == nID) \
		{ \
			if(!DDX_Numeric_Inf<float>(nID, var, bSaveAndValidate, lower, include)) \
				return FALSE; \
		}

#define DDX_EX_INT_POSITIVE_LIMIT(nID, var, lower, include) \
		if(nCtlID == (UINT)-1 || nCtlID == nID) \
		{ \
			if(!DDX_Numeric_Inf<int>(nID, var, bSaveAndValidate, lower, include)) \
				return FALSE; \
		}

#define DDX_EX_INT_POSITIVE(nID, var) \
		if(nCtlID == (UINT)-1 || nCtlID == nID) \
		{ \
			if(!DDX_Numeric_Inf<int>(nID, var, bSaveAndValidate)) \
				return FALSE; \
		}

#define DDX_EX_STDWSTRING(nID, str) \
		if(nCtlID == (UINT)-1 || nCtlID == nID) \
			if(!DDX_StdWString(nID, str, bSaveAndValidate)) \
				return FALSE; \

#define DDX_WTLx_WSTRING(controlObj, str) \
		if(nCtlID == (UINT)-1 || nCtlID == controlObj.GetDlgCtrlID()) \
			if(!DDX_WTLx_WString(controlObj, str, bSaveAndValidate)) \
				return FALSE; \

namespace WTLx
{

template <class T>
class WinDataExchangeEx : public WTL::CWinDataExchange<T>
{
public:	

	template<class C>
	bool DDX_WTLx_WString(C& control, std::wstring& str, BOOL save)
	{	
		if (save)
			str = control;
		else
			control = str;

		return true;
	}

	BOOL DDX_StdWString(UINT id, std::wstring& str, BOOL save, const int max_len=MAX_PATH)
	{
		T* pT = static_cast<T*>(this);
		BOOL success = TRUE;
		
		if (save)
		{
			std::vector<wchar_t> buffer(max_len);
			size_t len = pT->GetDlgItemText(id, &buffer[0], static_cast<int>(buffer.size()));
			str.assign(buffer.begin(), buffer.begin()+len);
		}
		else
		{		
			pT->SetDlgItemText(id, str.c_str());
		}
		
		return success;
	}
	
	template <typename N>
	BOOL DDX_Numeric_Inf(UINT nID, N& nVal, BOOL bSave, N lower_limit = 0, bool include_limit = true, BOOL bValidate = FALSE)
	{
		T* pT = static_cast<T*>(this);
		bool bSuccess = true;
		const int cchBuff = 128;
		wchar_t szBuff[cchBuff] = { 0 };
		
		if(bSave)
		{
			pT->GetDlgItemText(nID, szBuff, cchBuff);
			try
			{
			
			nVal = lexical_cast<N>(szBuff);
					
			if (include_limit)
			{
				if (nVal < lower_limit) 
					nVal = -1;
			}
			else
			{
				if (nVal <= lower_limit) 
					nVal = -1;
			}
			
			}        
			catch(boost::bad_lexical_cast &)
			{
				hal::event_log().post(shared_ptr<hal::EventDetail>(
					new hal::EventMsg(L"Save bad_lexical_cast")));
					
				nVal = -1;
			}
			
			if (nVal == -1)
				pT->SetDlgItemText(nID, hal::app().res_wstr(HAL_INF).c_str());
		}
		else
		{
			wstring number = hal::app().res_wstr(HAL_INF);
			
			try
			{
			
			if (include_limit)
			{
				if (nVal >= lower_limit) 
					number = lexical_cast<wstring>(nVal);
			}
			else
			{
				if (nVal > lower_limit) 
					number = lexical_cast<wstring>(nVal);
			}
				
			}
			catch(boost::bad_lexical_cast &)
			{
				hal::event_log().post(shared_ptr<hal::EventDetail>(
					new hal::EventMsg(L"Load bad_lexical_cast")));
					
				wstring number = hal::app().res_wstr(HAL_INF);
			}			
				
			pT->SetDlgItemText(nID, number.c_str());
		}
		
		return bSuccess;
	} 
};

};
