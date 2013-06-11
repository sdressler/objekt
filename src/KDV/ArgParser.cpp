//#include <KernelVolume/ArgParser.h>
#include <LLVMGraph/KDV/ArgParser.h>

#include <llvm/Support/CallSite.h>
#include <llvm/Support/raw_ostream.h>
#include <iostream>

void ArgParser::performArgumentAnalysis(llvm::Instruction *I, const std::string &kernelName) {

    // We use "CallSite" here to induce an abstract level
    CallSite CS(I);

    // Insert the root vertex
    //vertexIterator v = boost::add_vertex(*g);
    //auto v = g->addVertex(F->getFunctionType()).first;
    auto v = g->addVertex(CS.getCalledFunction()->getFunctionType()).first;

    // Assign its name
    //(*g)[v].name = kernelName;
    //(*g)[v].t = Types::FUNCTION;
    v->second.name = kernelName;
    v->second.t = Types::FUNCTION;

    unsigned int idx = 0;

    //for (auto arg = F->arg_begin(); arg != F->arg_end(); ++arg) {
    //for (unsigned int )
    for (auto arg = CS.arg_begin(); arg != CS.arg_end(); ++arg) {
        
        errs() << "Processing: "; (*arg)->dump();

        llvm::Value *argument;
        if (llvm::GetElementPtrInst::classof(*arg)) {
            argument = llvm::dyn_cast<llvm::GetElementPtrInst>(*arg)->getPointerOperand();
        } else {
            argument = *arg;
        }

        preAnalyzeArgument(argument, NULL, v, idx);
        idx++;

    }

}

void ArgParser::preAnalyzeArgument(Value *arg, Type *type, TG_vertexIterator root, unsigned int idx) {
  
    errs() << "Argument pre-analysis (APA).\n";

    // Retrieve the type analysis result
    auto at = analyzeArgument(arg, type);

    if (at.t == Types::FUNCTION) {
        return;
    }

    // Extract label
    auto label = at.name;
    
    // Save old pointer state for later
    bool tmp_isPointer = at.isPointer;
    llvm::Type *tmp_type = at.type;
    unsigned int tmp_ptrDepth = at.ptrDepth;
    Types tmp_t = at.t;

    // Remove pointer flag, if needed
    if (at.isPointer) {

        llvm::Type *no_ptr = at.type;

        while (at.ptrDepth > 0) {
            no_ptr = no_ptr->getPointerElementType();
            at.ptrDepth--;
        }

        at.type = no_ptr;
        at.isPointer = false;

    }

    errs() << "APA: Insert vertexIterator " << label << "\n";
    // Insert the vertex
    auto v = g->addVertex(at.type);

    errs() << "APA: Insert Edge.\n";
    // Add edge to root vertexIterator
    auto e = g->addEdge(root, v.first);

    // Store the properties
    e->second.index = idx;
    e->second.isPointer = tmp_isPointer;
    e->second.ptrDepth = tmp_ptrDepth;
    e->second.type = tmp_type;
    e->second.bitwidth = at.bitwidth;
    e->second.t = tmp_t;

    //llvm::errs() << "isPtr: " << e->second.isPointer << "\n";

    // Add a value, if present
    if (arg != NULL) {
        e->second.name = v.first->second.name;
        e->second.value = arg;
    }

    // Already analyzed?!
    if (!v.second) {
        return;
    }

    // Store parameters
    v.first->second = at;

    if (at.t == Types::ARRAY) {

    	llvm::errs() << "APA: LLVM ARRAY must be analyzed.\n";

    	//llvm::ArrayType array;
    	//if (at.isPointer) {
    	//	preAnalyzeArgument(NULL, at.type->getContainedType(0)->getArrayElementType(), v.first, 0);
    	//} else {
    	preAnalyzeArgument(NULL, at.type->getArrayElementType(), v.first, 0);
    	//}

    }

    // If the inserted vertexIterator is a custom class, we must analyze it
    if (at.t == Types::STRUCT) {

        errs() << "APA: Class must be analyzed.\n";

        //errs() << "Test: " << (*g)[e].isPointer << "\n";

        //if (at.isPointer) {
        //    classAnalysis(cast<StructType>(at.type->getContainedType(0)), v.first);
        //} else {
            classAnalysis(cast<StructType>(at.type), v.first);
        //}

    }

    // If the inserted vertexIterator is a STL container, analyze the contained argument
    if (
            at.t >= Types::STL_CONTAINER_BEGIN &&
            at.t <= Types::STL_CONTAINER_END
       ) {

        errs() << "APA: Container allocated type must be analyzed.\n";
        //if (at.isPointer) {
        //    analyzeContainerTemplateType(at.type->getContainedType(0), v.first);
        //} else {
            analyzeContainerTemplateType(at.type, v.first);
        //}

    }

}
