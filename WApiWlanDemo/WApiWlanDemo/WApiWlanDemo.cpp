#include "stdafx.h"

using namespace std;

wchar_t* UTF8ToUnicode(char* utf8_str)
{
	auto wcsLen = MultiByteToWideChar(CP_UTF8, NULL, utf8_str, strlen(utf8_str), NULL, 0);
	auto wszString = new wchar_t[wcsLen + 1];
	MultiByteToWideChar(CP_UTF8, NULL, utf8_str, strlen(utf8_str), wszString, wcsLen);
	wszString[wcsLen] = '\0';
	return wszString;
}

int main()
{
	wcout.imbue(locale(""));

	DWORD wlan_version;
	HANDLE wlan_handle;
	PWLAN_INTERFACE_INFO_LIST interface_list;
	PWLAN_BSS_LIST bss_list;

	auto error = WlanOpenHandle(2, NULL, &wlan_version, &wlan_handle);
	error = WlanEnumInterfaces(wlan_handle, NULL, &interface_list);
	error = WlanGetNetworkBssList(wlan_handle, &interface_list[0].InterfaceInfo[0].InterfaceGuid,
		NULL, dot11_BSS_type_any, NULL, NULL, &bss_list);
	auto index = 0;
	while (index < bss_list->dwNumberOfItems)
	{
		auto wlan_entry = bss_list->wlanBssEntries[index];
		wcout << "SSID: " << UTF8ToUnicode(reinterpret_cast<char*>(wlan_entry.dot11Ssid.ucSSID)) << ", ";
		cout << "MAC: ";
		for (size_t mac_index = 0; mac_index < 6; mac_index++)
		{
			printf("%02x", wlan_entry.dot11Bssid[mac_index]);
			if (mac_index < 5) cout << ":";
			else cout << ", ";
		}
		cout << "RSSI: " << dec << wlan_entry.lRssi << endl;
		index++;
	}
	system("pause");
	return 0;
}
