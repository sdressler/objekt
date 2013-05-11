/*
 * AnalyzeBB.cpp
 *
 *  Created on: Nov 11, 2012
 *      Author: bzcdress
 */

#include <LLVMGraph/BlockProcessor.h>

#include <llvm/Support/raw_ostream.h>

void BlockProcessor::analyzeBB(llvm::BasicBlock *BB) {

    for (auto &it : *BB) {
        analyzeInstruction(&it);
    }

    // Connect first instruction with block
    auto it = BB->begin();
    while(llvm::isa<llvm::AllocaInst>(it) ||
          llvm::isa<llvm::StoreInst>(it)  ||
          llvm::isa<llvm::LoadInst>(it)) {

        ++it;

    }

    g->addEdge((*g)[BB], (*g)[it]);

}
