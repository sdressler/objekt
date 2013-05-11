#ifndef BASE_TYPE_HANDLER_H
#define BASE_TYPE_HANDLER_H


#include <LLVMGraph/KDV/TypeHandlers/ITypeHandler.h>
#include <LLVMGraph/ValueGraph.h>

#include <llvm/Support/raw_ostream.h>

class TypeHandlerFactory {

public:
//    typedef std::map<ArgType::Types, std::shared_ptr<ITypeHandler>> handler_map_type;
    //typedef std::map<ArgType::Types, ITypeHandler*> handler_map_type;
    typedef std::map<Types, ITypeHandler*> handler_map_type;

    std::map<Type*,  Function*> TSF_Map;

private: 
    static handler_map_type *handlerMap;

protected:
    static handler_map_type* getHandlerMap() {
/*        
        if (!handlerMap) {
            handlerMap = new handler_map_type;
        }
*/
        return handlerMap;
    }

public:

    static ITypeHandler* createInstance(Types type) {
        handler_map_type::iterator it = getHandlerMap()->find(type);
        
        if (it == getHandlerMap()->end()) {
            return NULL;
        }

        return it->second->create();

    }
};

template<class T>
class RegisterTypeHandler : public TypeHandlerFactory {
public:
    RegisterTypeHandler(Types type) {

        errs() << "Register type handler called. Type: " << type << "\n";

        getHandlerMap()->insert(std::make_pair(type, new T));

    }
};

class BaseTypeHandler : public TypeHandlerFactory {

private:
    llvm::Module *M;
    TG *tg;
    VG *vg;

    /**
     * This function processes pointer memory allocations made with e.g. "new"
     * or "malloc". It may rely on loop informations.
     *
     */
    std::vector<llvm::GlobalValue*> ProcessPointerAllocations(TG_edgeIterator &e);

public:
    void GenerateAllocGraph(llvm::Function *F, llvm::Value *val);
    BaseTypeHandler(llvm::Module *_M, TG *_tg) : M(_M), tg(_tg), vg(NULL) { }

    llvm::Function* runOnGraphEdge(TG_edgeIterator &e, VG *vg = NULL);

    void createVG()  { vg = new VG(); }
    void destroyVG() { delete vg;     }

};

template<class Base>
class HandlerDerivate : public Base {
private:
    std::string baseName;
    std::string suffixName;

protected:    
    virtual std::string getSizeFunctionName() { return baseName + "::" + suffixName; }

public:
    HandlerDerivate(Base *h, std::string s) : Base(h) {
        baseName = h->getSizeFunctionName();
        suffixName = s;
    }

    virtual ~HandlerDerivate() { }

};

#endif /* BASE_TYPE_HANDLER_H */
