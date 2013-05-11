/*
 * AnalyzeInstruction.cpp
 *
 *  Created on: Nov 11, 2012
 *      Author: bzcdress
 */

#include <LLVMGraph/BlockProcessor.h>

#include <llvm/Support/raw_ostream.h>

vertexIterator BlockProcessor::analyzeInstruction(llvm::Instruction *I) {

    auto v = g->addVertex(I);

    if (llvm::isa<llvm::AllocaInst>(I)) {

        //v.first->second.name = "Alloca";
        g->removeVertex(v.first);
        return g->vertex_end();

        //g->addEdge((*g)[I->getParent()], v.first);

    } else if (llvm::isa<llvm::LoadInst>(I)) {

        v.first->second.name = "Load";

//        auto LI = llvm::dyn_cast<llvm::LoadInst>(I);
/*
        auto PtrOP = LI->getPointerOperand();
        auto gPtrOP = analyzeOperandValue(PtrOP);
        g->addEdge(gPtrOP, v.first);
*/
    } else if (llvm::isa<llvm::StoreInst>(I)) {

        v.first->second.name = "Store";

        auto SI = llvm::dyn_cast<llvm::StoreInst>(I);
/*
        auto PtrOP = SI->getPointerOperand();
        auto gPtrOP = analyzeOperandValue(PtrOP);
        g->addEdge(v.first, gPtrOP);
*/
        auto ValOP = SI->getValueOperand();
        auto gValOP = analyzeOperandValue(ValOP);
        g->addEdge(gValOP, v.first);

    } else if (llvm::isa<llvm::BranchInst>(I)) {

        v.first->second.name = "BR";

        auto BR = llvm::dyn_cast<llvm::BranchInst>(I);

        if (BR->isConditional()) {
            g->addEdge((*g)[BR->getCondition()], v.first);

            auto e = g->addEdge(v.first, (*g)[BR->getSuccessor(0)]);
            e->second.name = "True";

            e = g->addEdge(v.first, (*g)[BR->getSuccessor(1)]);
            e->second.name = "False";

        } else {

            // Check if BR is first instruction in BB
            llvm::Instruction *first = BR->getParent()->begin();
            if (first == I) {
                g->addEdge((*g)[BR->getParent()], v.first);
            } else {
                g->addEdge((*g)[BR->getPrevNode()], v.first);
            }

            g->addEdge(v.first, (*g)[BR->getSuccessor(0)]);
        }

    } else if (llvm::isa<llvm::CmpInst>(I)) {

        v.first->second.name = "CMP";

        auto CMP = llvm::dyn_cast<llvm::CmpInst>(I);

        for (auto op = CMP->op_begin(); op != CMP->op_end(); ++op) {
            g->addEdge(analyzeOperandValue(*op), v.first);
        }

    } else if (llvm::isa<llvm::GetElementPtrInst>(I)) {

        v.first->second.name = "GEP";

        auto GEP = llvm::dyn_cast<llvm::GetElementPtrInst>(I);
        g->addEdge((*g)[GEP->getPointerOperand()], v.first);

        for (auto idx = GEP->idx_begin(); idx != GEP->idx_end(); ++idx) {

            auto gOP = analyzeOperandValue(idx->get());
            g->addEdge(gOP, v.first);

        }

    } else if (llvm::isa<llvm::CastInst>(I)) {

        v.first->second.name = "Cast";

        auto CAST = llvm::dyn_cast<llvm::CastInst>(I);
        g->addEdge((*g)[CAST->getOperand(0)], v.first);

    } else if (llvm::isa<llvm::BinaryOperator>(I)) {

        auto BOP = llvm::dyn_cast<llvm::BinaryOperator>(I);

        // Label with BOP-Type
        v.first->second.name = std::string("OP: ") + std::string(BOP->getOpcodeName());

        for (int i = 0; i < 2; i++) {

            auto gOP = analyzeOperandValue(BOP->getOperand(i));
            g->addEdge(gOP, v.first);

        }

    } else if (llvm::isa<llvm::ReturnInst>(I)) {

        v.first->second.name = "Ret";

        auto RET = llvm::dyn_cast<llvm::ReturnInst>(I);
        g->addEdge((*g)[RET->getParent()], v.first);

    } else if (llvm::isa<llvm::CallInst>(I)) {

        auto CALL = llvm::dyn_cast<llvm::CallInst>(I);

        v.first->second.name = CALL->getCalledFunction()->getName().str();

        // Add an edge for every argument
        for (int i = 0; i < CALL->getNumArgOperands(); i++) {
            auto gArg = analyzeOperandValue(CALL->getArgOperand(i));
            g->addEdge(gArg, v.first);
        }

    } else if (llvm::isa<llvm::PHINode>(I)) {

        v.first->second.name = "PHI";

        auto PHI = llvm::dyn_cast<llvm::PHINode>(I);

        PHI->dump();

        for (auto in = PHI->block_begin(); in != PHI->block_end(); ++in) {

            auto inBB  = *in;
            auto myBB  = PHI->getParent();
            auto inVal = PHI->getIncomingValueForBlock(inBB);

            // Connect to the corresponding BR instruction
            auto inEdges = g->inEdges((*g)[myBB]);
            for (auto inE = inEdges.begin(); inE != inEdges.end(); ++inE) {
                auto src = g->source(*inE);

                auto SrcBR = llvm::dyn_cast<llvm::BranchInst>(src->first);

                // Check for cast failure
                assert(SrcBR && "Not a branch!");

                // Correct branch?
                if (SrcBR->getParent() == inBB) {

                    // Remove outgoing edge to Block
                    g->removeEdge(*(g->outEdges((*g)[SrcBR]).begin()));

                    // Connect BR to PHI
                    auto e = g->addEdge((*g)[SrcBR], v.first);

                    // Label value
                    e->second.value = inVal;
                    e->second.name = inVal->getName().str();

                    break;
                }

            }




            //auto e = g->addEdge((*g)[inVal], v.first);

            // Label edge with arg
            //e->second.name = inBB->getName().str(); //PHI->getIncomingValueForBlock(inBB)->getName().str();

        }

    } else {

        llvm::errs() << "Instruction not handled: " << *I << "\n";
        assert(0);

    }

    return v.first;

}

