#ifndef LLVM_GRAPH_H
#define LLVM_GRAPH_H

#include <llvm/Pass.h>
#include <llvm/Module.h>
#include <llvm/Type.h>
#include <llvm/Constants.h>

#include <llvm/Analysis/LoopInfo.h>
#include <llvm/Analysis/AliasAnalysis.h>

using namespace llvm;

class LLVMGraph: public ModulePass {
public:
	static char ID;
	LLVMGraph() : ModulePass(ID) { }

	bool runOnModule(Module &M);

	void getAnalysisUsage(AnalysisUsage &AU) const {
	    AU.addRequired<llvm::LoopInfo>();
	    AU.addRequired<llvm::AliasAnalysis>();
	}

};

#endif /* LLVM_GRAPH_H */
