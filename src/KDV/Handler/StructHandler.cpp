#include <LLVMGraph/KDV/TypeHandlers/TypeHandlers.h>

void StructHandler::createSizeFunction() {
    
    // This is for the summation
    Value *x = ConstantInt::get(IntegerType::get(M->getContext(), 64), 0);

    // Now we need to traverse all outgoing edges and
    // add calls to their respective size functions
    //auto oe = boost::out_edges(v, *g);
    auto oe = g->outEdges(v);

    for (auto e = oe.begin(); e != oe.end(); ++e) {

        // Retrieve the size function for this edge
        Function *sf = BTH->runOnGraphEdge(*e);

        // Create a GEP to retrieve the correct pointer for the call
        llvm::Value *gep;

        //g->source(*e)->second.value->dump();

        //llvm::errs() << (*e)->second.isPointer << " " << (*e)->second.ptrDepth << "\n";

        if ((*e)->second.isPointer) {

            auto gep_ptr  = builder->CreateConstInBoundsGEP2_32(f_arg, 0, (*e)->second.index);

            auto depth = (*e)->second.ptrDepth;

            assert((depth <= 2) && "Ptr depth > 2 not supported");

            if (depth == 1) {
                auto gep_load = builder->CreateLoad(gep_ptr);
                     gep      = builder->CreateConstInBoundsGEP1_32(gep_load, 0);
            }

            if (depth == 2) {

                auto gep_load = builder->CreateLoad(gep_ptr);
                     gep      = builder->CreateConstInBoundsGEP1_32(gep_load, 0);

                     //gep      = builder->CreateLoad(gep);

            }

        } else {

            if (llvm::PointerType::classof(f_arg->getType()->getPointerElementType())) {
                gep = builder->CreateLoad(f_arg);
                gep = builder->CreateConstInBoundsGEP2_32(gep, 0, (*e)->second.index);
            } else {
                gep = builder->CreateConstInBoundsGEP2_32(f_arg, 0, (*e)->second.index);
            }

        }

        // Create a call to that function
        auto *result = builder->CreateCall(sf, gep);

        // Sum up
        x = builder->CreateNUWAdd(result, x);
    }

    // Create a return
    builder->CreateRet(x);

} 

RegisterTypeHandler<StructHandler> StructHandler::reg(Types::STRUCT);
