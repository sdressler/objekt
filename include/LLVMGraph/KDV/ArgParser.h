#ifndef CLASS_ANALYZER_H
#define CLASS_ANALYZER_H

#include <llvm/Value.h>
#include <llvm/Instructions.h>
#include <llvm/DerivedTypes.h>
#include <llvm/Function.h>

#include <LLVMGraph/TypeGraph.h>

using namespace llvm;

class ArgParser {
    
    friend class KernelVolume;

private:
    
//    G *g;

    TG *g;

    StructType *_struct;
    Module *M;

public:
    ArgParser(TG *_g, Module *_m) {
        g = _g;
        M = _m;
        _struct = NULL;
    }

    //void performArgumentAnalysis(CallInst *ci, const std::string &kernelName);
    void performArgumentAnalysis(llvm::Instruction *I, const std::string &kernelName);
	void classAnalysis(const StructType *s, TG_vertexIterator root);
	void typeHandler(const StructType *s, const Type *t, std::string root);

    void analyzeContainerTemplateType(const Type *t, TG_vertexIterator root);
    void analyzeContainerTemplateType_Vector(const StructType *t, TG_vertexIterator root);
    void analyzeContainerTemplateType_Map(const StructType *t, TG_vertexIterator root);

    StructType* findMapTreeNode(StructType *TreeType);

    void preAnalyzeArgument(Value *arg, Type *type, TG_vertexIterator root, unsigned int idx);
    GraphVertexProperties analyzeArgument(Value *arg, Type *type);

};

#endif /* CLASS_ANALYZER_H */
