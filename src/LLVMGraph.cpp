#include <LLVMGraph/LLVMGraph.h>
#include <LLVMGraph/TypeGraph.h>
#include <LLVMGraph/BlockProcessor.h>
#include <LLVMGraph/KDV.h>

#include <llvm/Support/raw_ostream.h>

bool LLVMGraph::runOnModule(Module &M) {

    TG g;

    KernelVolume KDV(&M, &g, NULL);
    KDV.run();

    llvm::errs() << "Done!\n";

    return true;

}

char LLVMGraph::ID = 0;
static RegisterPass<LLVMGraph> X(
    "kdv",
    "Determine Kernel Data Volume",
    false,
    false
);
