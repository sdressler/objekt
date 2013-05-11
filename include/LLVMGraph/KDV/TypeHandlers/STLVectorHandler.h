#include "BaseTypeHandler.h"

class STLVectorHandler : public ITypeHandler {
protected:
	llvm::Type *template_type;
	llvm::Type *iterator_type;

public:
    STLVectorHandler() { }

    virtual STLVectorHandler* create() { return new STLVectorHandler(); }

protected:
    STLVectorHandler(STLVectorHandler *handler) : ITypeHandler(handler) { }

private:
    virtual void createSizeFunction();

    static RegisterTypeHandler<STLVectorHandler> reg;
};

class STLVectorHandler_IteratorBegin : public HandlerDerivate<STLVectorHandler> {
public:
    STLVectorHandler_IteratorBegin(
        STLVectorHandler *h, llvm::Type *tt, llvm::Type *it
    ) : HandlerDerivate<STLVectorHandler>(h, "it_begin") {
        template_type = tt;
        iterator_type = it;
    }

private:
    virtual void createSizeFunction();

    virtual Function* createFunctionDecl() {

        return cast<llvm::Function>(M->getOrInsertFunction(
        	getSizeFunctionName(),
        	template_type,
        	t,
        	NULL
        ));

    }

};

class STLVectorHandler_IteratorEnd : public HandlerDerivate<STLVectorHandler> {
public:
    STLVectorHandler_IteratorEnd(
        STLVectorHandler *h, llvm::Type *tt, llvm::Type *it
    ) : HandlerDerivate<STLVectorHandler>(h, "it_end") {
        template_type = tt;
        iterator_type = it;
    }

private:
    virtual void createSizeFunction();

    virtual Function* createFunctionDecl() {

        return cast<llvm::Function>(M->getOrInsertFunction(
            getSizeFunctionName(),
            template_type,
            t,
            NULL
        ));

    }
};

class STLVectorHandler_IteratorL1 : public HandlerDerivate<STLVectorHandler> {
public:
    STLVectorHandler_IteratorL1(
        STLVectorHandler *h, llvm::Type *tt, llvm::Type *it
    ) : HandlerDerivate<STLVectorHandler>(h, "iterator_l1") {
        template_type = llvm::PointerType::get(tt, 0);
        iterator_type = llvm::PointerType::get(it, 0);
    }

private:
    virtual void createSizeFunction();

    virtual Function* createFunctionDecl() {

        return cast<llvm::Function>(M->getOrInsertFunction(
            getSizeFunctionName(),
            llvm::Type::getVoidTy(M->getContext()),
            iterator_type,
            template_type,
            NULL
        ));

    }

};

class STLVectorHandler_IteratorL2 : public HandlerDerivate<STLVectorHandler> {
public:
    STLVectorHandler_IteratorL2(
        STLVectorHandler *h, llvm::Type *tt, llvm::Type *it
    ) : HandlerDerivate<STLVectorHandler>(h, "iterator_l2") {
        template_type = llvm::PointerType::get(tt, 0);
        iterator_type = llvm::PointerType::get(it, 0);
    }

private:
    virtual void createSizeFunction();

    virtual Function* createFunctionDecl() {

        return cast<llvm::Function>(M->getOrInsertFunction(
            getSizeFunctionName(),
            llvm::Type::getVoidTy(M->getContext()),
            iterator_type,
            template_type,
            NULL
        ));

    }

};

class STLVectorHandler_IteratorOpNeq : public HandlerDerivate<STLVectorHandler> {
public:
    STLVectorHandler_IteratorOpNeq (
        STLVectorHandler *h, llvm::Type *tt, llvm::Type *it
    ) : HandlerDerivate<STLVectorHandler>(h, "iterator_op_neq") {
        iterator_type = llvm::PointerType::get(it, 0);
        template_type = tt;
    }

private:
    virtual void createSizeFunction();

    virtual Function* createFunctionDecl() {

        return cast<llvm::Function>(M->getOrInsertFunction(
            getSizeFunctionName(),
            llvm::Type::getInt1Ty(M->getContext()),
            iterator_type,
            iterator_type,
            NULL
        ));

    }

};

class STLVectorHandler_IteratorBase : public HandlerDerivate<STLVectorHandler> {
public:
    STLVectorHandler_IteratorBase (
        STLVectorHandler *h, llvm::Type *tt, llvm::Type *it
    ) : HandlerDerivate<STLVectorHandler>(h, "iterator_base") {
        template_type = llvm::PointerType::get(tt, 0);
        iterator_type = llvm::PointerType::get(it, 0);
    }

private:
    virtual void createSizeFunction();

    virtual Function* createFunctionDecl() {

        return cast<llvm::Function>(M->getOrInsertFunction(
            getSizeFunctionName(),
            template_type,
            iterator_type,
            NULL
        ));

    }

};

class STLVectorHandler_IteratorOpIndir : public HandlerDerivate<STLVectorHandler> {
public:
    STLVectorHandler_IteratorOpIndir (
        STLVectorHandler *h, llvm::Type *tt, llvm::Type *it
    ) : HandlerDerivate<STLVectorHandler>(h, "iterator_indir") {
        template_type = tt;
        iterator_type = llvm::PointerType::get(it, 0);
    }

private:
    virtual void createSizeFunction();

    virtual Function* createFunctionDecl() {

        return cast<llvm::Function>(M->getOrInsertFunction(
            getSizeFunctionName(),
            template_type,
            iterator_type,
            NULL
        ));

    }

};

class STLVectorHandler_IteratorOpInc : public HandlerDerivate<STLVectorHandler> {
public:
    STLVectorHandler_IteratorOpInc (
        STLVectorHandler *h, llvm::Type *it
    ) : HandlerDerivate<STLVectorHandler>(h, "iterator_inc") {
        iterator_type = llvm::PointerType::get(it, 0);
    }

private:
    virtual void createSizeFunction();

    virtual Function* createFunctionDecl() {

        return cast<llvm::Function>(M->getOrInsertFunction(
            getSizeFunctionName(),
            iterator_type,
            iterator_type,
            NULL
        ));

    }

};
