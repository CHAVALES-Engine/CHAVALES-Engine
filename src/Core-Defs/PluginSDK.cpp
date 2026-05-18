#include "PluginSDK.h"
#include "checkMLNew.h"

extern "C" __declspec(dllexport)
const core::ComponentDescriptor* getPluginComponents(size_t& count)
{
	const auto& comps = PluginSDK::PluginComponentRegistry::instance().get();
	count = comps.size();
	return comps.empty() ? nullptr : comps.data();
}

extern "C" __declspec(dllexport)
const std::string* getPluginPreloadResources(size_t& count)
{
	const auto& ress = PluginSDK::PluginResourcesRegistry::instance().get();
	count = ress.size();
	return ress.empty() ? nullptr : ress.data();
}
