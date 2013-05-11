#include <LLVMGraph/KDV/ArgParser.h>

#include <llvm/Support/raw_ostream.h>
#include <llvm/ADT/StringRef.h>
#include <llvm/DataLayout.h>
#include <llvm/Module.h>
#include <llvm/LLVMContext.h>

void ArgParser::classAnalysis(const StructType *s, TG_vertexIterator root) {

    auto sName = s->getStructName().str();
    errs() << "#### Extended analysis for " << sName << "\n";
    
    unsigned int idx = 0;   
    for (auto it = s->element_begin(); it != s->element_end(); ++it) {
        preAnalyzeArgument(NULL, *it, root, idx);
        idx++;
    }

    errs() << "### Done analysis of " << sName << "\n";

}

void ArgParser::analyzeContainerTemplateType(const Type *t, TG_vertexIterator root) {

    // This extracts the type that is inside of the vector
    auto *s = cast<StructType>(t);

    // This is for safety
    if (s->getStructNumElements() > 1) {
        errs() << "Error: more than one struct element inside of container class.\n";
        assert(0);
    }

    //switch ((*g)[root].t) {
    switch (root->second.t) {
        case Types::STL_CONTAINER_VECTOR:
            analyzeContainerTemplateType_Vector(s, root);
            break;

        case Types::STL_CONTAINER_MAP:
            analyzeContainerTemplateType_Map(s, root);
            break;

        default:
            assert(0);
            break;
    }
}
    
void ArgParser::analyzeContainerTemplateType_Vector(const StructType *s, TG_vertexIterator root) {

    // Allocated type is on 2nd level
    StructType *allocTypeStruct = cast<StructType>(s->getStructElementType(0));
    allocTypeStruct = cast<StructType>(allocTypeStruct->getStructElementType(0));

    // This is the allocated type, but wrapped in a pointer
    PointerType *allocTypePtr = cast<PointerType>(allocTypeStruct->getStructElementType(0));

    // Run argument analysis with allocated type, but strip the pointer
    preAnalyzeArgument(NULL, allocTypePtr->getContainedType(0), root, 0);

}

