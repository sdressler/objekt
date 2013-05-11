/*
 * PointerAllocations.cpp
 *
 *  Created on: Jan 18, 2013
 *      Author: bzcdress
 */

#include <LLVMGraph/KDV/TypeHandlers/BaseTypeHandler.h>
#include <LLVMGraph/DataDirection.h>

#include <llvm/Type.h>
#include <llvm/IRBuilder.h>
#include <llvm/Analysis/AliasAnalysis.h>
#include <llvm/DataLayout.h>

#include <stdlib.h>

std::vector<llvm::GlobalValue*>
BaseTypeHandler::ProcessPointerAllocations(TG_edgeIterator &e) {

    std::vector<llvm::GlobalValue*> GVs;

    //e->second.type->dump();
	assert(!(e->second.ptrDepth > 2) && "ptrDepth > 2 is not supported.");

    llvm::errs() << "This is a pointer. We should check for new...\n";

    //llvm::errs() << "Depth: " << e->second.ptrDepth << "\n";
/*
    llvm::Value *arg = e->second.value;
    DataDirection DD(M, arg, baseF, NULL);
    DD.run();
*/
    auto *DD_g = vg;

    assert(DD_g && "DD_g == NULL");

    auto *new_array_funs = M->getFunction("_Znam");
    if (new_array_funs == NULL) { return GVs; }

    llvm::IRBuilder<> builder(M->getContext());

    for (auto u = new_array_funs->use_begin(); u != new_array_funs->use_end(); ++u) {

        llvm::Instruction *I  = llvm::dyn_cast<llvm::Instruction>(*u);
        llvm::CallInst    *CI = llvm::dyn_cast<llvm::CallInst>(*u);

        assert(I && "No instruction!");

        // Find the store
        llvm::StoreInst *SI = NULL;
        while (SI == NULL) {
            I = I->getNextNode();
            SI = llvm::dyn_cast<llvm::StoreInst>(I);
        }

        // Pointer Operand of SI in DD Graph?
        auto v_DD = (*DD_g)[SI->getPointerOperand()];
        if (v_DD == DD_g->vertex_end()) {
            continue;
        }

        //llvm::errs() << "Match #1\n";

        // Check for second match
        if (llvm::GetElementPtrInst::classof(v_DD->first)) {

            auto *GEP = llvm::dyn_cast<llvm::GetElementPtrInst>(v_DD->first);

            // Types must match
            if (e->second.type != GEP->getType()->getPointerElementType()) {
                continue;
            }

            unsigned this_index = 0;
            for (auto idx = GEP->idx_begin(); idx != GEP->idx_end(); ++idx) {
                this_index = llvm::dyn_cast<ConstantInt>(idx->get())->getZExtValue();
            }

            // Indices must match
            if (this_index != e->second.index) { continue; }

        } else if (llvm::LoadInst::classof(v_DD->first)) {

            auto *LI = llvm::dyn_cast<llvm::LoadInst>(v_DD->first);

            // Types must match
            if (e->second.type != LI->getType()->getPointerElementType()) {
                continue;
            }

        } else if (llvm::AllocaInst::classof(v_DD->first)) {

            auto *AI = llvm::dyn_cast<llvm::AllocaInst>(v_DD->first);

            // Types must match
            if (e->second.type != AI->getType()->getPointerElementType()) {
                continue;
            }

        } else if (llvm::Argument::classof(v_DD->first)) {

            // Types must match
            if (e->second.type != v_DD->first->getType()->getPointerElementType()) {
                continue;
            }

        } else {
            v_DD->first->dump();
            assert(0);
        }

        //llvm::errs() << "Match #2\n";

        /********************************************/
        /* Below this point a clear match was found */
        /********************************************/

        // Create a new global value.
        auto *GV = new llvm::GlobalVariable(
            *M,
            llvm::IntegerType::getInt64Ty(M->getContext()),
            false,
            llvm::GlobalValue::LinkageTypes::PrivateLinkage,
            llvm::ConstantInt::get(llvm::IntegerType::getInt64Ty(M->getContext()), 0),
            "mgv"
        );
        GV->setAlignment(8);

        GVs.push_back(GV);

        // Insert store to GV prior call
        builder.SetInsertPoint(CI);

        //e->second.type->dump(); llvm::errs() << "\n";

        llvm::Type *this_type = e->second.type;

        DataLayout DL(M);

        this_type = this_type->getPointerElementType();

        uint64_t base_size = DL.getTypeSizeInBits(this_type) / 8;

        //llvm::errs() << "Store: "; SI->dump();
        //llvm::errs() << "Base size: " << base_size << " Type: "; this_type->dump(); llvm::errs() << "\n";

        auto *num_elems = builder.CreateSDiv(
            CI->getArgOperand(0),
            llvm::ConstantInt::get(
                llvm::IntegerType::getInt64Ty(M->getContext()), base_size
            )
        );

        auto *GEP_GV    = builder.CreateConstInBoundsGEP1_32(GV, 0);
                          builder.CreateAlignedStore(num_elems, GEP_GV, 8);

    }

    return GVs;

}

void BaseTypeHandler::GenerateAllocGraph(llvm::Function *F, llvm::Value *val) {
/*
    if (vg == NULL) {
        vg = new VG();
        llvm::errs() << "new\n";
    }
*/
    assert(vg && "Graph == NULL!");

    if (F->getBasicBlockList().size() == 0) { return; }

    auto v_val = vg->addVertex(val).first;
    v_val->second.name = val->getName().str();

    // Get the "base"
    VG_vertexIterator v_base;
    if (llvm::LoadInst::classof(val)) {

        auto *LI = llvm::dyn_cast<llvm::LoadInst>(val);

        v_base = vg->addVertex(LI->getPointerOperand()).first;
        v_base->second.name = LI->getPointerOperand()->getName().str();

        vg->addEdge(v_val, v_base);

    } else if (llvm::Argument::classof(val)) {

        v_base = v_val;

    } else if (llvm::AllocaInst::classof(val)) {

        //v_base = vg->addVertex(val).first;
        return;

    } else if (llvm::GetElementPtrInst::classof(val)) {

        auto *GEP = llvm::dyn_cast<llvm::GetElementPtrInst>(val);

        v_base = vg->addVertex(GEP->getPointerOperand()).first;
        v_base->second.name = GEP->getPointerOperand()->getName().str();

        vg->addEdge(v_val, v_base);

    } else if (llvm::Constant::classof(val)) {

        //auto *CONST = llvm::dyn_cast<llvm::Constant>(val);

        v_base = vg->addVertex(val).first;
        v_base->second.name = "const";

        vg->addEdge(v_val, v_base);

    } else if (llvm::BitCastInst::classof(val)) {

        auto *BC = llvm::dyn_cast<llvm::BitCastInst>(val);

        v_base = vg->addVertex(BC->getOperand(0)).first;
        v_base->second.name = BC->getOperand(0)->getName().str();

    } else {

        llvm::errs() << val->getValueID() << "\n";
        assert(0);

    }

    // Search for other reference to v_base
    for (auto BB = F->begin(); BB != F->end(); ++BB) {
        for (auto I = BB->begin(); I != BB->end(); ++I) {

            if (llvm::StoreInst::classof(I)) {

                auto *SI = llvm::dyn_cast<llvm::StoreInst>(I);

                auto v_si_val = vg->addVertex(SI->getValueOperand());

                auto v_si_ptr = vg->addVertex(SI->getPointerOperand());

                if (v_si_val.second && v_si_ptr.second) {
                    vg->removeVertex(v_si_ptr.first);
                    vg->removeVertex(v_si_val.first);
                } else {

                    //auto v_si_val = g->addVertex(SI->getValueOperand());
                    v_si_ptr.first->second.name = SI->getPointerOperand()->getName().str();
                    v_si_val.first->second.name = SI->getValueOperand()->getName().str();

                    auto e = vg->addEdge(v_si_val.first, v_si_ptr.first);
                    e->second.name = "Store";

                }

            } else if (llvm::LoadInst::classof(I)) {

                auto *LI = llvm::dyn_cast<llvm::LoadInst>(I);

                auto v_li_ptr = vg->addVertex(LI->getPointerOperand());

                if (v_li_ptr.second) {
                    vg->removeVertex(v_li_ptr.first);
                } else {

                    auto v_li = vg->addVertex(LI);
                    v_li.first->second.name = LI->getName().str();

                    auto e = vg->addEdge(v_li_ptr.first, v_li.first);
                    e->second.name = "Load";

                }

            } else if (llvm::GetElementPtrInst::classof(I)) {

                auto *GEP = llvm::dyn_cast<llvm::GetElementPtrInst>(I);

                auto v_gep_ptr = vg->addVertex(GEP->getPointerOperand());

                if (v_gep_ptr.second) {

                    vg->removeVertex(v_gep_ptr.first);

                } else {

                    auto v_gep = vg->addVertex(GEP);
                    v_gep.first->second.name = GEP->getName().str();

                    auto e = vg->addEdge(v_gep_ptr.first, v_gep.first);
                    e->second.name = "GEP";

                }

            } else if (llvm::CallInst::classof(I)) {

                auto *CI = llvm::dyn_cast<llvm::CallInst>(I);

                // Inspect arguments
                unsigned args = CI->getNumArgOperands();
                for (unsigned i = 0; i < args; i++) {

                    llvm::Value *arg = CI->getArgOperand(i);
                    //arg->dump();

                    auto v_arg = vg->addVertex(arg);
                    if (!v_arg.second) {
                        // Already added

                        // Add the call and an edge
                        auto v_call = vg->addVertex(CI).first;

                        auto *called_F = CI->getCalledFunction();
                        v_call->second.name = called_F->getName().str();

                        // Dive into Function and analyse further
                        //DataDirection(M, )

                        auto e = vg->addEdge(v_base, v_call);
                        e->second.index = i;

                        // Get the argument on the "other" side
                        //auto *called_F_arg = called_F->getArgumentList()[i];
                        //auto *called_F_arg = (called_F->arg_begin() + i);
                        unsigned j = 0;
                        auto it = called_F->arg_begin();
                        for (; it != called_F->arg_end(); ++it) {
                            if (j == i) { break; }
                            j++;
                        }

                        auto v_called = vg->addVertex(it).first;
                        v_called->second.name = it->getName().str();

                        auto e_called = vg->addEdge(v_call, v_called);
                        e_called->second.index = j;

                        // "Dive into the function and proceed
                        /*
                        DataDirection DD(M, v_called->first, called_F, g);
                        DD.run();
                        */
                        GenerateAllocGraph(called_F, v_called->first);

                    } else {
                        vg->removeVertex(v_arg.first);
                    }

                }

            }

        }
    }
/*
    MicroGraphWriter<VG> writer("graph_ddir.dot", *vg);
    writer.writeGraphToFile();
*/
    //llvm::errs() << "DataDirection done.\n";

}
