#include <LLVMGraph/KDV/TypeHandlers/BaseTypeHandler.h>

#include <llvm/Support/raw_ostream.h>

llvm::Function* BaseTypeHandler::runOnGraphEdge(TG_edgeIterator &e, VG *_vg) {

    //llvm::errs() << "\n\n";

    if (_vg != NULL) {
        vg = _vg;
    }

    assert(vg && "Value Graph is NULL");

    //auto type = e->second.type;
/*
    // Try to find the size function in the map
    auto it = TSF_Map.find(type);
    if (it != TSF_Map.end()) {
        return it->second;
    }
*/
    // Not found...
    auto t = e->second.t;
    auto *handler = TypeHandlerFactory::createInstance(t);

    // Check handler
    if (handler == NULL) {
        errs() << "No handler found for ArgType " << t << "!\n";
        assert(0);
    }

    // Handler initialization
    handler->deployHandlerData(this, M, tg, e);

    // Create the function declaration
    auto *f = handler->createFunctionDecl();

    // Insert the function declaration into the map
/*
    TSF_Map.insert(std::make_pair(
        type, f
    ));
*/
    // Fill the function prototype
    handler->CreateSizeFunctionFromPrototype(f);

    if (e->second.isPointer) {
        auto GVs = ProcessPointerAllocations(e);
        //llvm::errs() << "Malloc GVs: " << GVs.size() << "\n";

        std::stringstream ss;
        for (auto GV = GVs.begin(); GV != GVs.end(); ++GV) {
            ss << "_" << (*GV)->getName().str();
        }

        if (GVs.size() != 0) {
            auto *malloc_F = llvm::dyn_cast<Function>(M->getOrInsertFunction(
                f->getName().str() + "_malloc" + ss.str(),
                IntegerType::get(M->getContext(), 64),
                handler->getType(),
                NULL
            ));

            assert(malloc_F && "malloc_F == NULL");

            malloc_F->setDoesNotThrow();
            malloc_F->needsUnwindTableEntry();

            // Name first argument as "this"
            auto f_arg = malloc_F->arg_begin();
            f_arg->setName("this");

            llvm::IRBuilder<> builder(M->getContext());

            auto *BB = llvm::BasicBlock::Create(M->getContext(), "entry", malloc_F);

            builder.SetInsertPoint(BB);

            Value *x = ConstantInt::get(IntegerType::get(M->getContext(), 64), 0);
            for (auto GV = GVs.begin(); GV != GVs.end(); ++GV) {
                auto *load_gv = builder.CreateAlignedLoad(*GV, 8, (*GV)->getName());
                      x       = builder.CreateNUWAdd(x, load_gv);
            }

            auto *obj_size = builder.CreateCall(f, f_arg, "obj_size");
            auto *mult     = builder.CreateNUWMul(x, obj_size);

                             builder.CreateRet(mult);

            return malloc_F;

        }
    }

    return f;

}

TypeHandlerFactory::handler_map_type *TypeHandlerFactory::handlerMap = new handler_map_type();
