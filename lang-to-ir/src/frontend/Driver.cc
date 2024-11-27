#include "frontend/Driver.hh"
#include "frontend/ASTBuilder.hh"
#include "frontend/DotGenerator.hh"
#include "frontend/Errors.hh"
#include "frontend/Lexer.hh"
#include "frontend/Types.hh"
//#include "frontend/SemanticAnalyzer.hh"
#include "parser.tab.hh"

namespace paracl {

Driver::Driver()
    : astBuilder_(std::make_unique<ASTBuilder>()),
      lexer_(std::make_unique<Lexer>(*this)),
      parser_(std::make_unique<Parser>(*this)),
      errorReporter_(std::make_unique<ErrorReporter>()),
      typeCreator_(std::make_unique<TypeCreator>()),
      stmts_({})
      {}

void Driver::setFilepath(std::string_view filepath) {
  lexer_->setFilepath(filepath);
}

void Driver::setRoot(INode *root) const {
  astBuilder_->setRoot(root);
}
INode *Driver::getRoot() const {
  return astBuilder_->getRoot();
}

void Driver::generateDot(std::string_view filepath) const {
  DotGenerator generator{filepath};
  generator.generate(getRoot());
}

Parser::symbol_type Driver::getNextToken() {
  return lexer_->getNextToken();
}

int Driver::parse() {
  return parser_->parse();
}

bool Driver::hasErrors() const {
  return errorReporter_->hasErrors();
}
void Driver::reportAllErrors(std::ostream &os) const {
  errorReporter_->reportAllErrors(os);  
}

IntType* Driver::createIntType() {
  return typeCreator_->getType<IntType>();
}

VoidType* Driver::createVoidType() {
  return typeCreator_->getType<VoidType>();
}
ArrayType* Driver::createArrayType(Type *elemType, size_t elemCount) {
  return typeCreator_->getType<ArrayType>(elemType, elemCount);
}
TupleType* Driver::createTupleType(std::vector<Type*> &&types) {
  return typeCreator_->getType<TupleType>(std::move(types));
}

FunctionType* Driver::createFunctionType(Type *retType, TupleType *paramTypes) {
  return typeCreator_->getType<FunctionType>(retType, paramTypes);
}

//void Driver::analyze(INode *node) const

Driver::~Driver() = default;

} // namespace paracl
