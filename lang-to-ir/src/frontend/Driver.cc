#include "frontend/Driver.hh"
#include "frontend/ASTBuilder.hh"
#include "frontend/DotGenerator.hh"
#include "frontend/Lexer.hh"
#include "frontend/Types.hh"
#include "frontend/SemanticAnalyzer.hh"
#include "frontend/LibraryLoader.hh"
#include "backend/IRGen.hh"

namespace paracl {

Driver::Driver(std::string_view moduleName)
    : moduleName_(moduleName),
      astBuilder_(std::make_unique<ASTBuilder>()),
      lexer_(std::make_unique<Lexer>(*this)),
      errorReporter_(std::make_unique<ErrorReporter>()),
      typeCreator_(std::make_unique<TypeCreator>()), stmts_({}) {
        lexer_->setFilepath(moduleName);
        auto root = astBuilder_->createNode<TranslationUnit>();
        astBuilder_->setRoot(root);
        auto voidType = getVoidType();
        auto mainType = getFunctionType(voidType, {});
        auto main = astBuilder_->createNode<FunctionDecl>(location{}, "main");
        main->setType(mainType);
        root->setMain(main);
        auto body = astBuilder_->createNode<CompoundStmt>(location{});
        main->setBody(body);
        LibraryLoader libraryLoader{*this};
        libraryLoader.loadLibrary();
      }

TranslationUnit *Driver::getRoot() const { return astBuilder_->getRoot(); }

void Driver::generateDot(std::string_view filepath) const {
  ::paracl::generateDot(filepath, getRoot());
}

Parser::symbol_type Driver::getNextToken() { return lexer_->getNextToken(); }

void Driver::parse() {
  Parser parser{*this};
  parser.parse();
}

bool Driver::hasErrors() const { return errorReporter_->hasErrors(); }

void Driver::reportAllErrors(std::ostream &os) const {
  errorReporter_->reportAllErrors(os);
}

IntType *Driver::getIntType() { return typeCreator_->getType<IntType>(); }

VoidType *Driver::getVoidType() { return typeCreator_->getType<VoidType>(); }

ArrayType *Driver::getArrayType(Type *elemType, size_t elemCount) {
  return typeCreator_->getType<ArrayType>(elemType, elemCount);
}

PointerType *Driver::getPointerType(Type *elemType) {
  return typeCreator_->getType<PointerType>(elemType);
}

FunctionType *Driver::getFunctionType(Type *retType, std::vector<Type *> &&paramTypes) {
  return typeCreator_->getType<FunctionType>(retType, std::move(paramTypes));
}

void Driver::analyze() {
  ::paracl::semanticAnalyze(*this);
}

void Driver::execute() {
  ::paracl::execute(moduleName_, getRoot());
}


Driver::~Driver() = default;

} // namespace paracl
