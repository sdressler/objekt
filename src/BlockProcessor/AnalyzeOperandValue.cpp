/*
 * AnalyzeOperandValue.cpp
 *
 *  Created on: Nov 15, 2012
 *      Author: bzcdress
 */

#include <LLVMGraph/BlockProcessor.h>
#include <llvm/Support/raw_ostream.h>

#include <sstream>

vertexIterator BlockProcessor::analyzeOperandValue(llvm::Value *OP) {

    assert(!(OP == NULL) && "OP == NULL!");

    if ((*g)[OP] != g->vertex_end()) { return (*g)[OP]; }

    std::stringstream ss;
    auto v = g->addVertex(OP);

    if (llvm::isa<llvm::ConstantFP>(OP)) {

        auto CFP = llvm::dyn_cast<llvm::ConstantFP>(OP);
        ss << CFP->getValueAPF().convertToDouble() << "f";

    } else if (llvm::isa<llvm::ConstantInt>(OP)) {

        auto CI = llvm::dyn_cast<llvm::ConstantInt>(OP);
        ss << CI->getSExtValue() << "i";

    } else if (llvm::isa<llvm::Argument>(OP)) {

        auto CI = llvm::dyn_cast<llvm::Argument>(OP);
        ss << CI->getName().str();

    } else {
        OP->dump();
        llvm::errs() << OP->getValueID() << "\n";
        assert(!"Operand type not resolvable.");
    }

    v.first->second.name = ss.str();

    return v.first;

}
