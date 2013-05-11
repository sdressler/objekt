#include <LLVMGraph/KDV/TypeHandlers/ITypeHandler.h>

#include <llvm/Support/raw_ostream.h>

void ITypeHandler::deployHandlerData(
    BaseTypeHandler *_BTH,
    Module *_M,
    TG *_g,
    TG_edgeIterator &_e
) {

    BTH = _BTH;
    M = _M;
    g = _g;
    e = _e;

    //v = boost::target(*_e, _g);
    //v = g->target(e);
    v = g->target(e);
    //arg = v->second;
    //arg = e->second;

    //llvm::errs() << "Argument: "; arg.value->dump();

    preparePointerType();

}

Function* ITypeHandler::createFunctionDecl() {

    llvm::errs() << "FunDecl: "; t->dump(); llvm::errs() << " *** " << t << " *** ";
    llvm::errs() << getSizeFunctionName() << "\n";

    return cast<Function>(
        M->getOrInsertFunction(
            getSizeFunctionName(),
            IntegerType::get(M->getContext(), 64),
            t,
            NULL
        )
    );

}

//Function* ITypeHandler::getSizeFunction() {
void ITypeHandler::CreateSizeFunctionFromPrototype(Function *proto) {

    //errs() << "New function name: " << getSizeFunctionName() << "\n";

    //f = createFunctionDecl();
    f = proto;

    if (f->empty() && !functionExistsInLibrary()) {
    
        f->setDoesNotThrow();
        f->needsUnwindTableEntry();

        // Name first argument as "this"
        f_arg = f->arg_begin();
        f_arg->setName("this");

        // Define the function body
        BB = BasicBlock::Create(M->getContext(), "entry", f);

        // Instantiate builder
        builder = new IRBuilder<>(BB);

        // Call the size creation function
        createSizeFunction();

    }

    //return f;

}
    
void ITypeHandler::preparePointerType() {
    
    // Argument type
    Type *t_tmp = NULL;
    //if (arg.value == NULL) {
    if (e->second.value == NULL) {
        t_tmp = const_cast<Type*>(e->second.type);
    } else {
        //t_tmp = arg.value->getType();
        t_tmp = e->second.value->getType();

        //llvm::errs() << "PrepPtr Type: "; t_tmp->dump(); llvm::errs() << "\n";
        //arg.value->dump();
    }

    assert((t_tmp != NULL) && "t_tmp is NULL!");

    // t must be a pointer type, if not generate one
    if (!t_tmp->isPointerTy()) {
       t_tmp = PointerType::get(t_tmp, 0);
    }

    t = cast<PointerType>(t_tmp);

}
