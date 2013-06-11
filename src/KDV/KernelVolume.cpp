#include <LLVMGraph/KDV.h>
#include <LLVMGraph/KDV/ArgParser.h>

//bool KernelVolume::runOnModule(Module &M) {
void KernelVolume::run() {
    
    //Initialize Argument Parser    
    parser = new ArgParser(g, M);

    getKernelsFromGlobalAnnotations(M->getNamedGlobal("llvm.global.annotations"));
    errs() << "Found " << KernelFunctions.size() << " Kernels\n";

    llvm::Function *Fsize = cast<llvm::Function>(
        M->getOrInsertFunction(
            "_Z21printKernelVolumeSizeyy",
            Type::getVoidTy(M->getContext()),
            IntegerType::getInt64Ty(M->getContext()),
            IntegerType::getInt64Ty(M->getContext()),
            NULL
        )
    );

    Fsize->dump();

    for (auto it : KernelFunctions) {
        it->dump();
        callProcessor(M, it, Fsize);
    }
    
    errs() << "Done!\n";

}

