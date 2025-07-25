#include "gpu_monitor.hpp"

#define NOMINMAX
#include <windows.h>
#include <dxgi.h>
#include <dxgi1_2.h>
#include <comdef.h>
#include <wbemidl.h>
#include <vector>
#include <string>
#include <map>
#include <algorithm>

#ifdef _DEBUG
#include <iostream>
#endif

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "wbemuuid.lib")
#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "oleaut32.lib")

class WindowsGpuMonitor : public IGpuMonitor {
private:
    struct Adapter {
        std::string name;
        LUID        luid{};
        uint64_t    luidKey{0};
    };

    std::vector<Adapter> adapters_;
    IWbemServices* wmiServices_ = nullptr;
    bool initialized_ = false;

    static uint64_t luidToKey(const LUID& luid) {
        return (static_cast<uint64_t>(luid.HighPart) << 32) | luid.LowPart;
    }

    bool initializeWmi() {
        HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
        if (FAILED(hr)) {
            return false;
        }

        hr = CoInitializeSecurity(nullptr, -1, nullptr, nullptr,
                                   RPC_C_AUTHN_LEVEL_DEFAULT,
                                   RPC_C_IMP_LEVEL_IMPERSONATE,
                                   nullptr, EOAC_NONE, nullptr);
        if (FAILED(hr) && hr != RPC_E_TOO_LATE) {
            CoUninitialize();
            return false;
        }

        IWbemLocator* wmiLocator = nullptr;
        hr = CoCreateInstance(CLSID_WbemLocator, nullptr, CLSCTX_INPROC_SERVER,
                              IID_IWbemLocator, (LPVOID*)&wmiLocator);
        if (FAILED(hr)) {
            CoUninitialize();
            return false;
        }

        hr = wmiLocator->ConnectServer(_bstr_t(L"ROOT\\CIMV2"), nullptr, nullptr, 0, 0, 0, 0, &wmiServices_);
        wmiLocator->Release();
        
        if (FAILED(hr)) {
            CoUninitialize();
            return false;
        }

        hr = CoSetProxyBlanket(wmiServices_, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, nullptr,
                               RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE,
                               nullptr, EOAC_NONE);
        if (FAILED(hr)) {
            wmiServices_->Release();
            CoUninitialize();
            return false;
        }

        return true;
    }

    bool enumerateAdapters() {
        IDXGIFactory1* factory = nullptr;
        if (FAILED(CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)&factory))) {
            return false;
        }

        for (UINT i = 0; ; ++i) {
            IDXGIAdapter1* adapter = nullptr;
            if (factory->EnumAdapters1(i, &adapter) == DXGI_ERROR_NOT_FOUND) {
                break;
            }

            DXGI_ADAPTER_DESC1 desc{};
            if (FAILED(adapter->GetDesc1(&desc))) {
                adapter->Release();
                continue;
            }

            if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) {
                adapter->Release();
                continue; // Skip Microsoft Basic Render Driver
            }

            Adapter a;
            a.luid = desc.AdapterLuid;
            a.luidKey = luidToKey(desc.AdapterLuid);

            int len = WideCharToMultiByte(CP_UTF8, 0, desc.Description, -1, nullptr, 0, nullptr, nullptr);
            if (len > 0) {
                a.name.resize(len - 1);
                WideCharToMultiByte(CP_UTF8, 0, desc.Description, -1, &a.name[0], len, nullptr, nullptr);
                adapters_.push_back(std::move(a));
            }
            
            adapter->Release();
        }
        
        factory->Release();
        return !adapters_.empty();
    }

public:
    WindowsGpuMonitor() {
        if (initializeWmi() && enumerateAdapters()) {
            initialized_ = true;
        }
    }

    ~WindowsGpuMonitor() override {
        if (wmiServices_) {
            wmiServices_->Release();
        }
        if (initialized_) {
            CoUninitialize();
        }
    }

    bool isSupported() const override {
        return initialized_;
    }

    size_t getGpuCount() const override {
        return adapters_.size();
    }

    GpuUsage getGpuUsage() override {
        GpuUsage usage;
        if (!initialized_ || !wmiServices_) {
            return usage;
        }

        std::map<uint64_t, double> maxUtilByLuid;

        IEnumWbemClassObject* wmiEnumerator = nullptr;
        HRESULT hr = wmiServices_->ExecQuery(
            bstr_t("WQL"),
            bstr_t("SELECT Name, UtilizationPercentage FROM Win32_PerfFormattedData_GPUPerformanceCounters_GPUEngine"),
            WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
            nullptr, &wmiEnumerator);

#ifdef _DEBUG
        std::cout << "[DEBUG] WMI Query HR: 0x" << std::hex << hr << std::dec << std::endl;
#endif

        if (SUCCEEDED(hr)) {
            IWbemClassObject* wmiObject = nullptr;
            ULONG ret = 0;
#ifdef _DEBUG
            int objectCount = 0;
#endif
            while (wmiEnumerator->Next(WBEM_INFINITE, 1, &wmiObject, &ret) == S_OK) {
#ifdef _DEBUG
                objectCount++;
#endif
                VARIANT vName, vUtil;
                VariantInit(&vName);
                VariantInit(&vUtil);
                
                wmiObject->Get(L"Name", 0, &vName, nullptr, nullptr);
                wmiObject->Get(L"UtilizationPercentage", 0, &vUtil, nullptr, nullptr);

#ifdef _DEBUG
                std::wcout << L"[DEBUG] WMI Object " << objectCount << L": Name=" 
                          << (vName.vt == VT_BSTR ? vName.bstrVal : L"NULL") 
                          << L", UtilType=" << vUtil.vt << L", Value=";
                
                if (vUtil.vt == VT_R8) std::wcout << vUtil.dblVal;
                else if (vUtil.vt == VT_UI8) std::wcout << vUtil.ullVal;
                else if (vUtil.vt == VT_UI4) std::wcout << vUtil.ulVal;
                else if (vUtil.vt == VT_UI1) std::wcout << (int)vUtil.bVal;
                else if (vUtil.vt == VT_I4) std::wcout << vUtil.lVal;
                else if (vUtil.vt == VT_BSTR) {
                    std::wcout << L"\"" << (vUtil.bstrVal ? vUtil.bstrVal : L"NULL") << L"\"";
                }
                else std::wcout << L"UNKNOWN (type=" << vUtil.vt << L")";
                std::wcout << std::endl;
#endif

                if (vName.vt == VT_BSTR && vUtil.vt != VT_NULL) {
                    std::wstring inst(vName.bstrVal, SysStringLen(vName.bstrVal));
#ifdef _DEBUG
                    std::wcout << L"[DEBUG]   Checking instance: " << inst << std::endl;
#endif
                    size_t pos = inst.find(L"luid_");
                    if (pos != std::wstring::npos) {
#ifdef _DEBUG
                        std::wcout << L"[DEBUG]   Found luid_ at position " << pos << std::endl;
#endif
                        pos += 5;
                        size_t us = inst.find(L"_", pos);
                        size_t us2 = inst.find(L"_", us + 1);
#ifdef _DEBUG
                        std::wcout << L"[DEBUG]   Underscore positions: " << us << L", " << us2 << std::endl;
#endif
                        if (us != std::wstring::npos && us2 != std::wstring::npos) {
                            try {
                                std::wstring hiStr = inst.substr(pos, us - pos);
                                std::wstring loStr = inst.substr(us + 1, us2 - us - 1);
#ifdef _DEBUG
                                std::wcout << L"[DEBUG]   LUID parts: hi=" << hiStr << L", lo=" << loStr << std::endl;
#endif
                                
                                uint32_t hi = std::stoul(hiStr, nullptr, 16);
                                uint32_t lo = std::stoul(loStr, nullptr, 16);
                                uint64_t key = (static_cast<uint64_t>(hi) << 32) | lo;
                                
                                double utilization = 0.0;
                                if (vUtil.vt == VT_R8) {
                                    utilization = vUtil.dblVal;
                                } else if (vUtil.vt == VT_UI8) {
                                    utilization = static_cast<double>(vUtil.ullVal);
                                } else if (vUtil.vt == VT_UI4) {
                                    utilization = static_cast<double>(vUtil.ulVal);
                                } else if (vUtil.vt == VT_UI1) {
                                    utilization = static_cast<double>(vUtil.bVal);
                                } else if (vUtil.vt == VT_I4) {
                                    utilization = static_cast<double>(vUtil.lVal);
                                } else if (vUtil.vt == VT_BSTR && vUtil.bstrVal) {
                                    // Parse utilization from string
                                    try {
                                        std::wstring utilStr(vUtil.bstrVal);
                                        utilization = std::stod(utilStr);
                                    } catch (...) {
                                        utilization = 0.0;
                                    }
                                }
                                
#ifdef _DEBUG
                                std::cout << "[DEBUG]   Parsed LUID: " << std::hex << key << std::dec 
                                         << ", Util: " << utilization << std::endl;
#endif
                                
                                if (maxUtilByLuid.find(key) == maxUtilByLuid.end()) {
                                    maxUtilByLuid[key] = 0.0;
                                }
                                maxUtilByLuid[key] = std::max(maxUtilByLuid[key], utilization);

                            } catch (...) {
#ifdef _DEBUG
                                std::wcout << L"[DEBUG]   LUID parsing error" << std::endl;
#endif
                            }
                        }
#ifdef _DEBUG
                        else {
                            std::wcout << L"[DEBUG]   Invalid underscore positions" << std::endl;
                        }
#endif
                    }
#ifdef _DEBUG
                    else {
                        std::wcout << L"[DEBUG]   No luid_ found in instance name" << std::endl;
                    }
#endif
                }
                
                VariantClear(&vName);
                VariantClear(&vUtil);
                wmiObject->Release();
            }
#ifdef _DEBUG
            std::cout << "[DEBUG] Total WMI objects found: " << objectCount << std::endl;
#endif
            wmiEnumerator->Release();
        }
#ifdef _DEBUG
        else {
            std::cout << "[DEBUG] WMI Query failed!" << std::endl;
        }
#endif

#ifdef _DEBUG
        std::cout << "[DEBUG] Detected adapters: " << adapters_.size() << std::endl;
        for (const auto& ad : adapters_) {
            std::cout << "[DEBUG] Adapter LUID: " << std::hex << ad.luidKey << std::dec 
                     << ", Name: " << ad.name << std::endl;
        }

        std::cout << "[DEBUG] WMI LUID keys found:" << std::endl;
        for (const auto& pair : maxUtilByLuid) {
            std::cout << "[DEBUG]   LUID: " << std::hex << pair.first << std::dec 
                     << " -> Util: " << pair.second << "%" << std::endl;
        }
#endif

        double totalUtil = 0.0;
        for (auto& ad : adapters_) {
            GpuInfo gi;
            gi.name = ad.name;
            gi.utilizationPercent = 0.0;
            
            auto it = maxUtilByLuid.find(ad.luidKey);
            if (it != maxUtilByLuid.end()) {
                gi.utilizationPercent = it->second;
#ifdef _DEBUG
                std::cout << "[DEBUG] Found utilization for " << ad.name 
                         << ": " << it->second << "%" << std::endl;
#endif
            }
#ifdef _DEBUG
            else {
                std::cout << "[DEBUG] No utilization data found for " << ad.name << std::endl;
            }
#endif
            
            gi.utilizationPercent = std::min(100.0, std::max(0.0, gi.utilizationPercent));
            usage.gpus.push_back(gi);
            totalUtil += gi.utilizationPercent;
        }

        if (!usage.gpus.empty()) {
            usage.averageUtilization = totalUtil / usage.gpus.size();
        }

        return usage;
    }
};

// Factory function for use elsewhere
IGpuMonitor* createGpuMonitor() {
    return new WindowsGpuMonitor();
}