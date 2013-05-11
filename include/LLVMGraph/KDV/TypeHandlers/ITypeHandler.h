#ifndef I_TYPE_HANDLER_H
#define I_TYPE_HANDLER_H

#include <llvm/Function.h>
#include <llvm/Module.h>
#include <llvm/Type.h>
#include <llvm/Argument.h>
#include <llvm/DerivedTypes.h>
#include <llvm/BasicBlock.h>
#include <llvm/IRBuilder.h>

#include <string>
#include <sstream>
#include <map>
#include <memory>

//#include "../Graph.h"
#include <LLVMGraph/TypeGraph.h>

using namespace llvm;

class BaseTypeHandler;

class ITypeHandler {

protected:
    Module *M;
    TG *g;
    //out_edge_iterator e;
    TG_edgeIterator e;

    TG_vertexIterator v;
    //vertex v;
    //ArgType arg;
    //GraphVertexProperties arg;
    
    Function *f;
    Argument *f_arg;

    PointerType *t;

    IRBuilder<> *builder;

    BasicBlock *BB;

    BaseTypeHandler *BTH;

    void preparePointerType();

    virtual bool        functionExistsInLibrary() { return false; }
    
    virtual void        createSizeFunction() = 0;

public:
    virtual Function*     createFunctionDecl();
    virtual ITypeHandler* create() = 0;

    llvm::PointerType* getType() { return t; }

public:
    virtual std::string getSizeFunctionName() {

        std::stringstream ss;
        //ss << arg.name << "_" << arg.bitwidth;
        //ss << v->second.name << "_" << v->second.bitwidth << "_" << e->second.ptrDepth << e->second.type;
        ss << v->second.name << "_" << v->second.bitwidth << "_" << e->second.ptrDepth << "_" << v->second.type;

        return ss.str();

    }

public:
    ITypeHandler() { f = NULL; }

    ITypeHandler(ITypeHandler *obj) {
        BTH = obj->BTH; M = obj->M; g = obj->g; e = obj->e;

        v = obj->v; //arg = obj->arg;

        t = obj->t;

        f = NULL;
    }

    void deployHandlerData(
        BaseTypeHandler *_BTH,
        Module *_M,
        TG *_g,
        TG_edgeIterator &e
    );

    //Function* getSizeFunction();
    //Function* Create
    void CreateSizeFunctionFromPrototype(Function *proto);

    Function* getSizeFunction() {
        if (f == NULL) {
            f = createFunctionDecl();
        }

        return f;

    }
};

#endif /* I_TYPE_HANDLER_H */
