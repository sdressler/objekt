#include <LLVMGraph/KDV.h>
#include <LLVMGraph/KDV/TypeHandlers/BaseTypeHandler.h>
#include <LLVMGraph/DataDirection.h>

#include <llvm/Support/CallSite.h>

void KernelVolume::callProcessor(Module *M, Function *kernelF, Function *printF) {

    assert(kernelF != NULL && "No kernel function!");

    // Check if the former User is an invoke or call
    auto *p = kernelF->use_back();

    if (CallInst::classof(p) || InvokeInst::classof(p)) {
        
        //auto *ci = cast<CallInst>(p);

        // Set the global current ci
        //current_ci = ci;

        errs() << "Found call: "; p->dump(); errs() << "\n";

        errs() << "\n";
        errs() << "Argument analysis.\n";
        errs() << "==================\n\n";

        auto *I = llvm::dyn_cast<llvm::Instruction>(p);

        parser->performArgumentAnalysis(I, "K");

        MicroGraphWriter<TG> writer("graph.dot", *g);
        writer.writeGraphToFile();

        errs() << "\n";
        errs() << "Size value analysis.\n";
        errs() << "====================\n\n";

        CallSite CS(I);

        TG_vertexIterator gCallRoot = (*g)[CS.getCalledFunction()->getFunctionType()];

        //gCallRoot->second.breadcrumb = true;

        // Create an instance of BaseTypeHandler class
        BTH = new BaseTypeHandler(M, g);

        // Assign size functions to arguments

        auto edges = g->outEdges(gCallRoot);

        for (auto e = edges.begin(); e != edges.end(); ++e) {


            llvm::Value *arg = (*e)->second.value;

            /*
            llvm::errs() << (*e)->second.index << "\n";
            */

            //DataDirection DD(M, arg, CS.getCalledFunction(), NULL);
            DataDirection DD(M, CS.getCalledFunction(), (*e)->second.value, (*e)->second.index);
            DD.run();

            auto *DD_Graph = DD.getValueGraph();

            MicroGraphWriter<VG> writer("graphs/graph_ddir_" + arg->getName().str() + ".dot", *DD_Graph);
            writer.writeGraphToFile();

            auto isOutput = DD.IsOutputData((*e)->second.value);

            BTH->createVG();

                          BTH->GenerateAllocGraph(I->getParent()->getParent(), arg);
        	auto size_F = BTH->runOnGraphEdge(*e);

        	BTH->destroyVG();

            llvm::errs() << "ASF Map Insert\n";

        	ASF_Map.insert(std::make_pair(
        	    (*e)->second.value,
        	    std::make_pair(size_F, isOutput)
        	));

        }

        //gCallRoot->second.breadcrumb = false;

        // Test
        errs() << "\n";
        errs() << "Resulting sizes\n";
        errs() << "===============\n\n";

        // Create a builder object, insertion point is prior current call
        //llvm::Instruction *I = llvm::dyn_cast<llvm::Instruction>(p);
        IRBuilder<> builder(I);

        Value *in    = ConstantInt::get(IntegerType::get(M->getContext(), 64), 0);
        Value *out   = ConstantInt::get(IntegerType::get(M->getContext(), 64), 0);

        for (auto &sfmap : ASF_Map) {

            Value    *arg      = sfmap.first;
            Function *size_F   = sfmap.second.first;
            bool      isOutput = sfmap.second.second;

            size_F->dump();

            // If arguments type is not a pointer, create one
            if (!arg->getType()->isPointerTy()) {

                // Create storage
                auto *arg_alloca = builder.CreateAlloca(arg->getType(), 0);

                // Store argument
                builder.CreateStore(arg, arg_alloca);

                // Replace argument
                arg = arg_alloca;
            }

            // Create a call to the size function with the given argument
            auto *size_C = builder.CreateCall(size_F, arg);

            if (isOutput == false) {
                in = builder.CreateAdd(size_C, in);
            }

            if (isOutput == true) {
                out = builder.CreateAdd(size_C, out);
            }

        }

        // Insert the call to the print function
        builder.CreateCall2(printF, in, out);

    } else {

        errs() << "Not a call function, something must be wrong.\n";
        assert(0);

    }


}
