#include "BaseTypeHandler.h"

class PODHandler : public ITypeHandler {
public:
    virtual PODHandler* create() { return new PODHandler(); }

private:
    virtual void createSizeFunction();
    
    static RegisterTypeHandler<PODHandler> reg;

};

class STLStringHandler : public ITypeHandler {
public:
    virtual STLStringHandler* create() { return new STLStringHandler(); }

private:
    virtual bool functionExistsInLibrary() { return true; }

    virtual void createSizeFunction() { }
    virtual std::string getSizeFunctionName() { return "_ZNKSs4sizeEv"; }

    static RegisterTypeHandler<STLStringHandler> reg;
};

class StructHandler : public ITypeHandler {
public:
    virtual StructHandler* create() { return new StructHandler(); }

public:
    virtual void createSizeFunction();

    static RegisterTypeHandler<StructHandler> reg;

};

class ArrayHandler : public ITypeHandler {
public:
    virtual ArrayHandler* create() { return new ArrayHandler(); }

public:
    virtual void createSizeFunction();

    static RegisterTypeHandler<ArrayHandler> reg;

};
