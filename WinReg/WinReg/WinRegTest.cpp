//////////////////////////////////////////////////////////////////////////
//
// WinRegTest.cpp -- by Giovanni Dicanio
// 
// Test some of the code in WinReg.hpp
// 
// NOTE --- Test Preparation ---
// In the folder containing this source file, there should be also a file 
// "GioTest.reg". This REG file contains some initial data to load into 
// the registry for this test.
// 
////////////////////////////////////////////////////////////////////////// 

#include "WinReg.hpp"   // Module to test
#include <exception>
#include <iostream>
#include <vector>
using namespace std;
using namespace winreg;

#ifdef UNICODE
	#define REG_COUT wcout
#else
	#define REG_COUT cout
#endif // UNICODE



int main()
{
    constexpr int kExitOk = 0;
    constexpr int kExitError = 1;

    try 
    {
        REG_COUT << _T("=========================================\n");
        REG_COUT << _T("*** Testing Giovanni Dicanio's WinReg ***\n");
        REG_COUT << _T("=========================================\n\n");

        //
        // Test subkey and value enumeration
        // 

        const tstring testSubKey = _T("SOFTWARE\\GioTest");
        RegKey key{ HKEY_CURRENT_USER, testSubKey };    
        
        vector<tstring> subKeyNames = key.EnumSubKeys();
        REG_COUT << _T("Subkeys:\n");
        for (const auto& s : subKeyNames)
        {
            REG_COUT << _T("  [") << s << _T("]\n");
        }
        REG_COUT << _T('\n');

        vector<pair<tstring, DWORD>> values = key.EnumValues();
        REG_COUT << _T("Values:\n");
        for (const auto& v : values)
        {
            REG_COUT << _T("  [") << v.first << _T("](") << RegKey::RegTypeToString(v.second) << _T(")\n");
        }
        REG_COUT << _T('\n');

        key.Close();


        //
        // Test SetXxxValue and GetXxxValue methods
        // 
        
        key.Open(HKEY_CURRENT_USER, testSubKey);
        
        const DWORD testDw = 0x1234ABCD;
        const ULONGLONG testQw = 0xAABBCCDD11223344;
        const tstring testSz = _T("CiaoTestSz");
        const tstring testExpandSz = _T("%PATH%");
        const vector<BYTE> testBinary{0xAA, 0xBB, 0xCC, 0x11, 0x22, 0x33};
        const vector<tstring> testMultiSz{ _T("Hi"), _T("Hello"), _T("Ciao") };

        key.SetDwordValue(_T("TestValueDword"), testDw);
        key.SetQwordValue(_T("TestValueQword"), testQw);
        key.SetStringValue(_T("TestValueString"), testSz);
        key.SetExpandStringValue(_T("TestValueExpandString"), testExpandSz);
        key.SetMultiStringValue(_T("TestValueMultiString"), testMultiSz);
        key.SetBinaryValue(_T("TestValueBinary"), testBinary);

        DWORD testDw1 = key.GetDwordValue(_T("TestValueDword"));
        if (testDw1 != testDw) 
        {
            REG_COUT << _T("RegKey::GetDwordValue failed.\n");
        }

        DWORD typeId = key.QueryValueType(_T("TestValueDword"));
        if (typeId != REG_DWORD)
        {
            REG_COUT << _T("RegKey::QueryValueType failed for REG_DWORD.\n");
        }

        ULONGLONG testQw1 = key.GetQwordValue(_T("TestValueQword"));
        if (testQw1 != testQw)
        {
            REG_COUT << _T("RegKey::GetQwordValue failed.\n");
        }

        typeId = key.QueryValueType(_T("TestValueQword"));
        if (typeId != REG_QWORD)
        {
            REG_COUT << _T("RegKey::QueryValueType failed for REG_QWORD.\n");
        }

        tstring testSz1 = key.GetStringValue(_T("TestValueString"));
        if (testSz1 != testSz) 
        {
            REG_COUT << _T("RegKey::GetStringValue failed.\n");
        }

        typeId = key.QueryValueType(_T("TestValueString"));
        if (typeId != REG_SZ)
        {
            REG_COUT << _T("RegKey::QueryValueType failed for REG_SZ.\n");
        }

        tstring testExpandSz1 = key.GetExpandStringValue(_T("TestValueExpandString"));
        if (testExpandSz1 != testExpandSz)
        {
            REG_COUT << _T("RegKey::GetExpandStringValue failed.\n");
        }

        typeId = key.QueryValueType(_T("TestValueExpandString"));
        if (typeId != REG_EXPAND_SZ)
        {
            REG_COUT << _T("RegKey::QueryValueType failed for REG_EXPAND_SZ.\n");
        }

        vector<tstring> testMultiSz1 = key.GetMultiStringValue(_T("TestValueMultiString"));
        if (testMultiSz1 != testMultiSz)
        {
            REG_COUT << _T("RegKey::GetMultiStringValue failed.\n");
        }

        typeId = key.QueryValueType(_T("TestValueMultiString"));
        if (typeId != REG_MULTI_SZ)
        {
            REG_COUT << _T("RegKey::QueryValueType failed for REG_MULTI_SZ.\n");
        }

        vector<BYTE> testBinary1 = key.GetBinaryValue(_T("TestValueBinary"));
        if (testBinary1 != testBinary)
        {
            REG_COUT << _T("RegKey::GetBinaryValue failed.\n");
        }

        typeId = key.QueryValueType(_T("TestValueBinary"));
        if (typeId != REG_BINARY)
        {
            REG_COUT << _T("RegKey::QueryValueType failed for REG_BINARY.\n");
        }


        //
        // Remove some test values
        //
        
        key.DeleteValue(_T("TestValueDword"));
        key.DeleteValue(_T("TestValueQword"));
        key.DeleteValue(_T("TestValueString"));
        key.DeleteValue(_T("TestValueExpandString"));
        key.DeleteValue(_T("TestValueMultiString"));
        key.DeleteValue(_T("TestValueBinary"));

        REG_COUT << _T("All right!! :)\n\n");
    }
    catch (const RegException& e)
    {
        cout << "\n*** Registry Exception: " << e.what();
        cout << "\n*** [Windows API error code = " << e.ErrorCode() << "\n\n";
        return kExitError;
    }
    catch (const exception& e)
    {
        cout << "\n*** ERROR: " << e.what() << '\n';
        return kExitError;
    }

    return kExitOk;
}
