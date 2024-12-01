#include "frontend/LibraryLoader.hh"

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

void LibraryLoader::loadLibrary() {
	auto root = driver_.getRoot();
	auto scan = getScan(driver_);
	root->push_back(scan);
	auto print = getPrint(driver_);
	root->push_back(print);
}

} // namespace paracl
