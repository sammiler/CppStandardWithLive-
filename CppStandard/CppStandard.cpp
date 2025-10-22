#include <iostream>
#include <vector>
#include <functional>
#include <thread>
#include <chrono>

#include "stl.h"

// --- Live++ Integration ---
#ifdef _DEBUG
#define LPP_ENABLE
#endif

#ifdef LPP_ENABLE
#include "LivePP/API/x64/LPP_API_x64_CPP.h"
#endif

int main()
{
#ifdef LPP_ENABLE
	// 启动 Live++ Agent
	lpp::LppDefaultAgent lppAgent = lpp::LppCreateDefaultAgent(
		nullptr, L"C:/Users/sammi/source/repos/CppStandard/ThirdParty/LivePP");
	if (lpp::LppIsValidDefaultAgent(&lppAgent))
	{
		lppAgent.EnableModule(lpp::LppGetCurrentModulePath(), lpp::LPP_MODULES_OPTION_ALL_IMPORT_MODULES, nullptr,
		                      nullptr);
	}
#endif

	// 只需要在程序启动时初始化一次！


	// 简化的主循环
	while (true)
	{
		initFuncList();
		// 直接执行列表中的函数
		for (const auto& func : funcList)
		{
			func();
		}

		std::cout << "-------------------------------------------\n";
		std::cout << "Press Enter to run again, or Q to quit \n";

		char c = getchar();
		if (c == 'Q' || c == 'q')
		{
			break;
		}
		funcList.clear();
	}

#ifdef LPP_ENABLE
	if (lpp::LppIsValidDefaultAgent(&lppAgent))
	{
		lpp::LppDestroyDefaultAgent(&lppAgent);
	}
#endif

	return 0;
}
