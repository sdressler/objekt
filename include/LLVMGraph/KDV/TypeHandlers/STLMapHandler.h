#include "BaseTypeHandler.h"

class STLMapHandler : public ITypeHandler {

public:
    STLMapHandler() { }
    
    virtual STLMapHandler* create() { return new STLMapHandler(); }

protected:
    STLMapHandler(STLMapHandler *handler) : ITypeHandler(handler) { }

private:
    virtual void createSizeFunction();

    static RegisterTypeHandler<STLMapHandler> reg;
};

class STLMapHandler_MapSize : public HandlerDerivate<STLMapHandler> {
public:
    STLMapHandler_MapSize(
        STLMapHandler *h
    ) : HandlerDerivate<STLMapHandler>(h, "size") { }

private:
    virtual void createSizeFunction();
};


class STLMapHandler_ElementIterator : public HandlerDerivate<STLMapHandler> {
public:
    STLMapHandler_ElementIterator(
        STLMapHandler *h
    ) : HandlerDerivate<STLMapHandler>(h, "elem_iterator") {

        // Replace the original pointer type
        StructType *Rb_tree_it_type = M->getTypeByName("struct.std::_Rb_tree_iterator");        
        t = PointerType::get(Rb_tree_it_type, 0);
    
    }

private:
    virtual void createSizeFunction();
    virtual Function* createFunctionDecl() {

        return cast<Function>(M->getOrInsertFunction(
            getSizeFunctionName(),
            Type::getVoidTy(M->getContext()),
            t,   // Function Argument
            NULL
        ));

    }

};

class STLMapHandler_ElementIteratorInc : public HandlerDerivate<STLMapHandler> {
public:
    STLMapHandler_ElementIteratorInc(
        STLMapHandler *h, PointerType *it_ty
    ) : HandlerDerivate<STLMapHandler>(h, "elem_iterator_inc") {

        // Replace the original pointer type
        t = it_ty;
    
    }

private:
    virtual void createSizeFunction();
    virtual Function* createFunctionDecl() {

        return cast<Function>(M->getOrInsertFunction(
            getSizeFunctionName(),
            t,
            t,
            NULL
        ));

    }

};

class STLMapHandler_CreateRbTreeIterator : public HandlerDerivate<STLMapHandler> {
private:
    PointerType *it_ty;
    PointerType *node_ty;

public:
    STLMapHandler_CreateRbTreeIterator(
        STLMapHandler *h, PointerType *_it_ty, PointerType *_node_ty
    ) : HandlerDerivate<STLMapHandler>(h, "rb_tree_it_ctor"),
        it_ty(_it_ty),
        node_ty(_node_ty) { }

private:
    virtual void createSizeFunction();
    virtual Function* createFunctionDecl() {

        return cast<Function>(M->getOrInsertFunction(
            getSizeFunctionName(),
            Type::getVoidTy(M->getContext()),
            it_ty,
            node_ty,   // Function Argument
            NULL
        ));

    }

};

class STLMapHandler_TreeIteratorCmp : public HandlerDerivate<STLMapHandler> {
public:
    STLMapHandler_TreeIteratorCmp(
        STLMapHandler *h, PointerType *_it_ty
    ) : HandlerDerivate<STLMapHandler>(h, "it_cmp") {

        t = _it_ty;

    }

private:
    virtual void createSizeFunction();
    virtual Function* createFunctionDecl() {

        return cast<Function>(M->getOrInsertFunction(
            getSizeFunctionName(),
            IntegerType::get(M->getContext(), 1),
            t,
            t,
            NULL
        ));

    }

};

class STLMapHandler_TreeIteratorAccess : public HandlerDerivate<STLMapHandler> {
private:
    StructType *node_ty;
    PointerType *it_ty;

public:
    STLMapHandler_TreeIteratorAccess(
        STLMapHandler *h, StructType *_node_ty, PointerType *_it_ty
    ) : HandlerDerivate<STLMapHandler>(h, "it_access"), node_ty(_node_ty), it_ty(_it_ty) { }

private:
    virtual void createSizeFunction();
    virtual Function* createFunctionDecl() {

        PointerType *pair_ty = PointerType::get(node_ty->getContainedType(1), 0);

        return cast<Function>(M->getOrInsertFunction(
            getSizeFunctionName(),
            pair_ty,
            it_ty,
            NULL
        ));

    }

};

class STLMapHandler_TreeIteratorBeginOrEnd : public HandlerDerivate<STLMapHandler> {
private:
    bool begin;

    static std::string getName(bool _begin) {
        if (_begin) {
            return "begin";
        } else {
            return "end";
        }
    }

public:
    STLMapHandler_TreeIteratorBeginOrEnd(
        STLMapHandler *h, bool _begin
    ) : HandlerDerivate<STLMapHandler>(
            h,
            STLMapHandler_TreeIteratorBeginOrEnd::getName(_begin)
        ), begin(_begin) { }

private:
    virtual void createSizeFunction();
    virtual Function* createFunctionDecl() {
        
        StructType *Rb_tree_node_base_type = M->getTypeByName("struct.std::_Rb_tree_node_base");
        if (Rb_tree_node_base_type == NULL) {
            errs() << "Rb_tree_node_base_type not found!\n";
            assert(0);
        }

        PointerType *Rb_tree_node_base_ptr_type = PointerType::get(Rb_tree_node_base_type, 0);

        return cast<Function>(M->getOrInsertFunction(
            getSizeFunctionName(),
            Rb_tree_node_base_ptr_type,
            t,   // Function Argument
            NULL
        ));
    
    }

};

class STLMapHandler_TreeIteratorBeginOrEndL2 : public HandlerDerivate<STLMapHandler> {
private:
    bool begin;
    PointerType *ret_ty;

public:
    STLMapHandler_TreeIteratorBeginOrEndL2(
        STLMapHandler *h, bool _begin, PointerType *_ret_ty, PointerType *_arg_ty
    ) : HandlerDerivate<STLMapHandler>(h, "L2"),
        begin(_begin), ret_ty(_ret_ty) {
    
            t = _arg_ty;

    }

private:
    virtual void createSizeFunction();
    virtual Function* createFunctionDecl() {

        return cast<Function>(M->getOrInsertFunction(
            getSizeFunctionName(),  // Function name
            ret_ty,
            t,   // Function Argument
            NULL
        ));
    
    }

};
