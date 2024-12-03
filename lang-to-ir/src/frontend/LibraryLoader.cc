#include "frontend/LibraryLoader.hh"

#include "GUILib.h"

namespace paracl
{

void print(int x) {
	std::cout << x << std::endl;
}

int scan() {
	int x;
	std::cin >> x;
	return x;
}

ExternFunctionDecl* getLoadedFunction(Driver& driver, std::string_view name, FunctionType* type, void* ptr) {
	auto funcDecl = driver.createNode<ExternFunctionDecl>(location{}, name);
	assert(funcDecl);
	funcDecl->setType(type);
	funcDecl->setPtr(ptr);
	for (auto&& paramType: *type) {
		auto param = driver.createNode<ParmVarDecl>(location{}, "");
		param->setType(paramType);
		funcDecl->push_back(param);
	}
	return funcDecl;
}

ExternFunctionDecl* getScan(Driver& driver) {
	std::vector<Type*> params{};
	auto retType = driver.getIntType();
	auto funcType = driver.getFunctionType(retType, std::move(params));
	return getLoadedFunction(driver, "scan", funcType, (void*)&scan);
}

ExternFunctionDecl* getPrint(Driver& driver) {
	std::vector<Type*> params{};
	auto intType = driver.getIntType();
	params.push_back(intType);
	auto retType = driver.getVoidType();
	auto funcType = driver.getFunctionType(retType, std::move(params));
	return getLoadedFunction(driver, "print", funcType, (void*)&print);
}

ExternFunctionDecl* getGuiInit(Driver& driver) {
	std::vector<Type*> params{};
	auto retType = driver.getVoidType();
	auto funcType = driver.getFunctionType(retType, std::move(params));
	return getLoadedFunction(driver, "gui_init", funcType, (void*)&gui_init);
}

ExternFunctionDecl* getGuiExit(Driver& driver) {
	std::vector<Type*> params{};
	auto retType = driver.getVoidType();
	auto funcType = driver.getFunctionType(retType, std::move(params));
	return getLoadedFunction(driver, "gui_exit", funcType, (void*)&gui_exit);
}

ExternFunctionDecl* getGuiFlush(Driver& driver) {
	std::vector<Type*> params{};
	auto retType = driver.getVoidType();
	auto funcType = driver.getFunctionType(retType, std::move(params));
	return getLoadedFunction(driver, "gui_flush", funcType, (void*)&gui_flush);
}

ExternFunctionDecl* getGuiQuitEvent(Driver& driver) {
	std::vector<Type*> params{};
	auto retType = driver.getIntType();
	auto funcType = driver.getFunctionType(retType, std::move(params));
	return getLoadedFunction(driver, "gui_quit_event", funcType, (void*)&gui_quit_event);
}

ExternFunctionDecl* getGuiSetPixel(Driver& driver) {
	std::vector<Type*> params{};
	auto intType = driver.getIntType();
	params.push_back(intType);
	params.push_back(intType);
	params.push_back(intType);
	auto retType = driver.getVoidType();
	auto funcType = driver.getFunctionType(retType, std::move(params));
	return getLoadedFunction(driver, "gui_set_pixel", funcType, (void*)&gui_set_pixel);
}

ExternFunctionDecl* getGuiRand(Driver& driver) {
	std::vector<Type*> params{};
	auto retType = driver.getIntType();
	auto funcType = driver.getFunctionType(retType, std::move(params));
	return getLoadedFunction(driver, "gui_rand", funcType, (void*)&gui_rand);
}

void LibraryLoader::loadLibrary() {
	auto root = driver_.getRoot();
	auto scan = getScan(driver_);
	root->push_back(scan);
	auto print = getPrint(driver_);
	root->push_back(print);
	auto gui_init = getGuiInit(driver_);
	root->push_back(gui_init);
	auto gui_exit = getGuiExit(driver_);
	root->push_back(gui_exit);
	auto gui_flush = getGuiFlush(driver_);
	root->push_back(gui_flush);
	auto gui_quit_event = getGuiQuitEvent(driver_);
	root->push_back(gui_quit_event);
	auto gui_set_pixel = getGuiSetPixel(driver_);
	root->push_back(gui_set_pixel);
	auto gui_rand = getGuiRand(driver_);
	root->push_back(gui_rand);
}

} // namespace paracl
