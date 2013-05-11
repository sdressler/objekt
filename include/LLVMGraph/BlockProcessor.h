/*
 * BlockProcessor.h
 *
 *  Created on: Nov 11, 2012
 *      Author: bzcdress
 */

#ifndef BLOCK_PROCESSOR_H
#define BLOCK_PROCESSOR_H_

#include <llvm/Module.h>
#include <llvm/Function.h>
#include <llvm/Instructions.h>

#include <LLVMGraph/Graph.h>

#include <map>
#include <string>

class BlockProcessor {
private:
    llvm::Module *M;
    llvm::Function *F;

    G *g;

public:
    BlockProcessor(llvm::Module *_M, llvm::Function *_F, G *_g) {
        M = _M; F = _F; g = _g;
    }

    void run();

    void analyzeBB(llvm::BasicBlock *BB);

    vertexIterator analyzeInstruction(llvm::Instruction *I);
    vertexIterator analyzeOperandValue(llvm::Value *OP);

};

#endif /* BLOCK_PROCESSOR_H */
