#include <LLVMGraph/DataDirection.h>

#include <llvm/Support/raw_ostream.h>

#include <llvm/Instructions.h>
#include <llvm/Type.h>
#include <llvm/DerivedTypes.h>

void DataDirection::run() {

    auto v_f_Arg = g->addVertex(f_arg).first;
    v_f_Arg->second.name = "Arg: " + f_arg->getName().str();

    auto v_Callee = g->addVertex(Callee).first;
    v_Callee->second.name = Callee->getName().str();

    g->addEdge(v_f_Arg, v_Callee);

    // Place argument
    unsigned int i = 0;
    auto arg = Callee->arg_begin();
    while (i != arg_index) {
        arg++;
        i++;
    }

    auto v_Arg = g->addVertex(arg).first;
    v_Arg->second.name = arg->getName().str();

    g->addEdge(v_Callee, v_Arg);

    TraceValues(v_Arg);

}

void DataDirection::TraceValues(VG_vertexIterator v) {

    for (auto BB = Callee->begin(); BB != Callee->end(); ++BB) {
        for (auto I = BB->begin(); I != BB->end(); ++I) {

            if (llvm::StoreInst::classof(I)) {

                auto *SI = llvm::dyn_cast<llvm::StoreInst>(I);

                if ((*g)[SI->getPointerOperand()] != g->vertex_end()) {

                    // Pointer is known, check if it may be a new store
                    if ((*g)[SI->getValueOperand()] == g->vertex_end()) {
                        // New Store
                        //SI->dump();
                        auto v_val = g->addVertex(SI->getValueOperand()).first;
                        v_val->second.name = SI->getValueOperand()->getName().str();

                        auto v_ptr = (*g)[SI->getPointerOperand()];

                        g->addEdge(v_val, v_ptr);

                        auto v_arg = (*g)[f_arg];

                        auto e = g->addEdge(v_ptr, v_arg);
                        e->second.memop = STORE;
                        e->second.name  = "STORE";

                    }

                    continue;

                }

                if ((*g)[SI->getValueOperand()] == g->vertex_end()) {
                    continue; // Does not belong to us
                }

                auto v_SI_ptr = g->addVertex(SI->getPointerOperand()).first;
                auto e = g->addEdge(v, v_SI_ptr);

                if (llvm::AllocaInst::classof(v_SI_ptr->first)) {

                    auto *AI = llvm::dyn_cast<llvm::AllocaInst>(v_SI_ptr->first);
                    v_SI_ptr->second.name = AI->getName().str();

                    TraceValues(v_SI_ptr);

                } else {

                    e->second.memop = STORE;
                    e->second.name  = "STORE";

                }

            } else if (llvm::LoadInst::classof(I)) {

                auto *LI = llvm::dyn_cast<llvm::LoadInst>(I);

                if ((*g)[LI] != g->vertex_end()) {
                    continue; // We were already here
                }

                auto v_LI_ptr = (*g)[LI->getPointerOperand()];

                if (v_LI_ptr == g->vertex_end()) {
                    continue; // Does not belong to us
                }

                auto v_LI = g->addVertex(LI).first;
                v_LI->second.name = LI->getName().str();

                g->addEdge(v_LI_ptr, v_LI);

                if (
                        llvm::AllocaInst::classof(v_LI_ptr->first)
                ) {
                    auto e = g->addEdge(v_LI, (*g)[f_arg]);
                    e->second.memop = LOAD;
                    e->second.name  = "LOAD";
                }

                TraceValues(v_LI);

            } else if (llvm::CallInst::classof(I)) {

                auto *CI = llvm::dyn_cast<llvm::CallInst>(I);

                if ((*g)[CI] != g->vertex_end()) {
                    continue; // We already were here
                }

                // One of the args must have to do with "us"
                unsigned int index = 0;
                for (; index < CI->getNumArgOperands(); index++) {
                    if (CI->getArgOperand(index) == v->first) { break; }
                }

                if (index == CI->getNumArgOperands()) {
                    continue; // No matching arg
                }

                // Traverse the called function
                DataDirection DD(M, CI->getCalledFunction(), f_arg, index, g);
                DD.run();

                // Connect edges between this and that function
                //g->addEdge(v, (*g)[CI->getCalledFunction()]);

                // Add vertex for call itself
                auto v_CI = g->addVertex(CI).first;
                v_CI->second.name = CI->getName().str();

                // Connect Function to Call
                g->addEdge((*g)[CI->getCalledFunction()], v_CI);

                v_CI->first->dump();
                TraceValues(v_CI);

            } else if (llvm::CastInst::classof(I)) {

                auto *CI = llvm::dyn_cast<llvm::CastInst>(I);

                if ((*g)[CI] != g->vertex_end()) {
                    continue; // We already were here
                }

                if ((*g)[CI->getOperand(0)] == g->vertex_end()) {
                    continue; // Does not belong to us
                }

                auto v_CI = g->addVertex(CI).first;
                v_CI->second.name = "Cast " + CI->getName().str();

                g->addEdge(v, v_CI);

                TraceValues(v_CI);

            } else if (llvm::GetElementPtrInst::classof(I)) {

                auto *GEP = llvm::dyn_cast<llvm::GetElementPtrInst>(I);

                if ((*g)[GEP] != g->vertex_end()) {
                    continue;
                }

                if ((*g)[GEP->getPointerOperand()] == g->vertex_end()) {
                    continue;
                }

                auto v_GEP = g->addVertex(GEP).first;
                v_GEP->second.name = "GEP " + GEP->getName().str();

                g->addEdge(v, v_GEP);

                TraceValues(v_GEP);

            }

        }
    }

}

    //auto v_Arg = g->addVertex(Callee->getArgumentList()

    //assert(g && "Graph == NULL!");
/*
    if (F->getBasicBlockList().size() == 0) { return; }

    auto v_F = g->addVertex(F).first;
    v_F->second.name = F->getName().str();
*/
/*
    auto v_val = g->addVertex(val).first;
    v_val->second.name = val->getName().str();

    for (auto BB = F->begin(); BB != F->end(); ++BB) {
        for (auto I = BB->begin(); I != BB->end(); ++I) {



        }
    }
*/
/*
	auto v_val_x = g->addVertex(val);

	// Check for possible infinite loops
	//if (v_val_x.second != false) {

	auto v_val = v_val_x.first;
	v_val->second.name = val->getName().str();

	val->dump();

	// Get the "base"
	VG_vertexIterator v_base;
	if (llvm::LoadInst::classof(val)) {

	    auto *LI = llvm::dyn_cast<llvm::LoadInst>(val);

	    v_base = g->addVertex(LI->getPointerOperand()).first;
	    v_base->second.name = LI->getPointerOperand()->getName().str();

	    g->addEdge(v_val, v_base);

	} else if (llvm::Argument::classof(val)) {

	    v_base = v_val;

	} else if (llvm::AllocaInst::classof(val)) {

	    //v_base = g->addVertex(val).first;
	    //return;
	    v_base = v_val;


	} else if (llvm::GetElementPtrInst::classof(val)) {

	    auto *GEP = llvm::dyn_cast<llvm::GetElementPtrInst>(val);

	    v_base = g->addVertex(GEP->getPointerOperand()).first;
	    v_base->second.name = GEP->getPointerOperand()->getName().str();

	    g->addEdge(v_val, v_base);

	} else if (llvm::Constant::classof(val)) {

	    //auto *CONST = llvm::dyn_cast<llvm::Constant>(val);

	    v_base = g->addVertex(val).first;
	    v_base->second.name = "const";

	    g->addEdge(v_val, v_base);

	} else {

	    llvm::errs() << val->getValueID() << "\n";
	    assert(0);

	}

	// Search for other reference to v_base
	for (auto BB = F->begin(); BB != F->end(); ++BB) {
	    for (auto I = BB->begin(); I != BB->end(); ++I) {

	        if (llvm::StoreInst::classof(I)) {

	            auto *SI = llvm::dyn_cast<llvm::StoreInst>(I);

	            auto v_si_val = g->addVertex(SI->getValueOperand());

	            auto v_si_ptr = g->addVertex(SI->getPointerOperand());

	            if (v_si_val.second && v_si_ptr.second) {
	                g->removeVertex(v_si_ptr.first);
	                g->removeVertex(v_si_val.first);
	            } else {

	                //auto v_si_val = g->addVertex(SI->getValueOperand());
	                v_si_ptr.first->second.name = SI->getPointerOperand()->getName().str();
	                v_si_val.first->second.name = SI->getValueOperand()->getName().str();

	                auto e = g->addEdge(v_si_val.first, v_si_ptr.first);
	                e->second.name = "Store";
	                e->second.memop = MemOps::STORE;

	            }

	        } else if (llvm::LoadInst::classof(I)) {

	            auto *LI = llvm::dyn_cast<llvm::LoadInst>(I);

	            auto v_li_ptr = g->addVertex(LI->getPointerOperand());

	            if (v_li_ptr.second) {
	                g->removeVertex(v_li_ptr.first);
	            } else {

	                auto v_li = g->addVertex(LI);
	                v_li.first->second.name = LI->getName().str();

	                auto e = g->addEdge(v_li_ptr.first, v_li.first);
	                e->second.name = "Load";
	                e->second.memop = MemOps::LOAD;

	            }

	        } else if (llvm::GetElementPtrInst::classof(I)) {

	            auto *GEP = llvm::dyn_cast<llvm::GetElementPtrInst>(I);

	            auto v_gep_ptr = g->addVertex(GEP->getPointerOperand());

	            if (v_gep_ptr.second) {

	                g->removeVertex(v_gep_ptr.first);

	            } else {

	                auto v_gep = g->addVertex(GEP);
	                v_gep.first->second.name = GEP->getName().str();

	                auto e = g->addEdge(v_gep_ptr.first, v_gep.first);
	                e->second.name = "GEP";

	            }

	        } else if (llvm::CallInst::classof(I)) {

	            auto *CI = llvm::dyn_cast<llvm::CallInst>(I);

	            // Inspect arguments
	            unsigned args = CI->getNumArgOperands();
	            for (unsigned i = 0; i < args; i++) {

	                llvm::Value *arg = CI->getArgOperand(i);
	                //arg->dump();

	                auto v_arg = g->addVertex(arg);
	                if (!v_arg.second) {
	                    // Already added

	                    // Add the call and an edge
	                    //auto v_call = g->addVertex(CI).first;

	                    auto *called_F = CI->getCalledFunction();
	                    //v_call->second.name = "Call: " + called_F->getName().str();

	                    auto v_call = g->addVertex(called_F).first;
	                    v_call->second.name = called_F->getName().str();

	                    //g->addEdge(v_called_F, v_call);

	                    // Dive into Function and analyse further
	                    //DataDirection(M, )

	                    auto e = g->addEdge(v_base, v_call);
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

	                    auto v_called = g->addVertex(it).first;
	                    v_called->second.name = it->getName().str();

	                    auto e_called = g->addEdge(v_call, v_called);
	                    e_called->second.index = j;

	                    // "Dive into the function and proceed
	                    DataDirection DD(M, v_called->first, called_F, g);
	                    DD.run();

	                } else {
	                    g->removeVertex(v_arg.first);
	                }

	            }

	        }

	    }
	}
*/
	//llvm::errs() << "DataDirection done.\n";

//}

bool DataDirection::IsOutputData(llvm::Value *arg) {

    // Get the vertex
    auto v = (*g)[arg];

    assert((v != g->vertex_end()) && "Could not find value in graph.");

    // Test if one incoming edge is a store, then direction is OUT else it is IN
    auto ie = g->inEdges(v);
    for (auto e = ie.begin(); e != ie.end(); ++e) {

        if ((*e)->second.memop == STORE) { return true; }

    }

    return false;

/*
    auto LSC = getLoadStoreCount(v_F, *(g->outEdges(v_F).begin()));

    auto loads  = LSC.first;
    auto stores = LSC.second;

    //llvm::errs() << loads << ", " << stores << ": ";

    if (stores > 1 && loads > 1) {
        //llvm::errs() << "INOUT\n";
        return DDir::INOUT;
    }

    if (stores <= 1 && loads > 0) {
        //llvm::errs() << "IN\n";
        return DDir::IN;
    }

    if (stores > 1 && loads <= 1) {
        //llvm::errs() << "OUT\n";
        return DDir::OUT;
    }

    assert(0 && "DDir Detection Failed.");
    return DDir::OUT;
*/
}
/*
std::pair<uint64_t, uint64_t> DataDirection::getLoadStoreCount(VG_vertexIterator v, VG_edgeIterator in_e) {

    uint64_t loads = 0, stores = 0;

    // Multiple incoming edges?
    auto ie = g->inEdges(v);
    for (auto e = ie.begin(); e != ie.end(); ++e) {

        if (*e == in_e) { continue; }

        getLoadStoreCountForEdge(*e, loads, stores);

    }

    auto oe = g->outEdges(v);
    for (auto e = oe.begin(); e != oe.end(); ++e) {

        getLoadStoreCountForEdge(*e, loads, stores);

        auto LSC = getLoadStoreCount(g->target(*e), *e);
        loads  += LSC.first;
        stores += LSC.second;

    }

    return std::make_pair(loads, stores);

}

void DataDirection::getLoadStoreCountForEdge(VG_edgeIterator &e, uint64_t &loads, uint64_t &stores) {

    if (e->second.memop == LOAD) {
        loads++;
    }

    if (e->second.memop == STORE) {
        stores++;
    }

}
*/
