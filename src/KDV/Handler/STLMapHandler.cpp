//#include <KernelVolume/TypeHandlers/STLMapHandler.h>
#include <LLVMGraph/KDV/TypeHandlers/STLMapHandler.h>

#include <llvm/Attributes.h>

void STLMapHandler::createSizeFunction() {  
    
    // Check if there already is an _Rb_tree_iterator type defined
    StructType *Rb_tree_it_type = M->getTypeByName(
        "struct.std::_Rb_tree_iterator"
    );

    // If not found, create one
    if (Rb_tree_it_type == NULL) {
        errs() << "Creation of _Rb_tree_iterator not yet implemented.\n";
        assert(0);
    }

    // Allocate space for size
    auto *size_alloc = builder->CreateAlloca(
        IntegerType::get(M->getContext(), 64),
        0,
        "size_sum"
    );
    size_alloc->setAlignment(8);

    // Initialize size sum
    auto *size_sum_store_init = builder->CreateStore(
        ConstantInt::get(IntegerType::getInt64Ty(M->getContext()), 0),
        size_alloc
    );
    size_sum_store_init->setAlignment(8);
    
    // Create a iterator
    auto *map_it_alloc = builder->CreateAlloca(
        Rb_tree_it_type, 0, "map_it"
    );
    map_it_alloc->setAlignment(8);
    
    // Create a iterator
    auto *map_it_t1_alloc = builder->CreateAlloca(
        Rb_tree_it_type, 0, "map_it_tmp1"
    );
    map_it_t1_alloc->setAlignment(8);
    
    // Create a iterator
    auto *map_it_t2_alloc = builder->CreateAlloca(
        Rb_tree_it_type, 0, "map_it_tmp2"
    );
    map_it_t2_alloc->setAlignment(8);
    
    // Initialize it
    STLMapHandler_ElementIterator ei(this);
    Function *tree_map_iter_fun = ei.getSizeFunction();
    ei.CreateSizeFunctionFromPrototype(tree_map_iter_fun);
    builder->CreateCall(tree_map_iter_fun, map_it_alloc);
   
    // Function for std::map<K,V>::begin
    STLMapHandler_TreeIteratorBeginOrEnd TreeItBegin(this, true);
    Function *tree_map_iter_begin_fun = TreeItBegin.getSizeFunction();
    TreeItBegin.CreateSizeFunctionFromPrototype(tree_map_iter_begin_fun);
    
    auto *map_it_begin = builder->CreateCall(tree_map_iter_begin_fun, f_arg);

    auto *rb_tree_it_t1_gep = builder->CreateConstInBoundsGEP2_32(
        map_it_t1_alloc, 0, 0
    );
    builder->CreateStore(map_it_begin, rb_tree_it_t1_gep);

    auto *map_it_cast_to_i8 = builder->CreateBitCast(
        map_it_alloc,
        PointerType::getInt8PtrTy(M->getContext())
    );

    auto *map_it_t1_cast_to_i8 = builder->CreateBitCast(
        map_it_t1_alloc,
        PointerType::getInt8PtrTy(M->getContext())
    );

    builder->CreateMemCpy(
        map_it_cast_to_i8,
        map_it_t1_cast_to_i8,
        8,
        8,
        false        
    );
   
    BasicBlock *main_loop = BasicBlock::Create(
        M->getContext(), "main_loop", f
    ); 

    builder->CreateBr(main_loop);

    builder->SetInsertPoint(main_loop);

    // Function for std::map<K,V>::end
    STLMapHandler_TreeIteratorBeginOrEnd TreeItEnd(this, false);
    Function *tree_map_iter_end_fun = TreeItEnd.getSizeFunction();
    TreeItEnd.CreateSizeFunctionFromPrototype(tree_map_iter_end_fun);

    auto *map_it_end = builder->CreateCall(tree_map_iter_end_fun, f_arg);

    auto *rb_tree_it_t2_gep = builder->CreateConstInBoundsGEP2_32(
        map_it_t2_alloc, 0, 0
    );
    builder->CreateStore(map_it_end, rb_tree_it_t2_gep);
    
    STLMapHandler_TreeIteratorCmp TreeItCmp(
        this,
        PointerType::get(Rb_tree_it_type, 0)
    );
    TreeItCmp.CreateSizeFunctionFromPrototype(TreeItCmp.getSizeFunction());
    auto *tree_map_it_cmp = builder->CreateCall2(
        TreeItCmp.getSizeFunction(),
        map_it_alloc,
        map_it_t2_alloc
    );

    BasicBlock *ret_true_BB = BasicBlock::Create(
        M->getContext(), "cmp_true", f
    ); 
    BasicBlock *ret_false_BB = BasicBlock::Create(
        M->getContext(), "cmp_false", f
    );

    builder->CreateCondBr(tree_map_it_cmp, ret_true_BB, ret_false_BB);

    builder->SetInsertPoint(ret_true_BB);

    //StructType *node_type = argi.getMapNodeType();
    StructType *node_type = v->second.MapNodeType;
    
    STLMapHandler_TreeIteratorAccess TreeItAccess(
        this, node_type, PointerType::get(Rb_tree_it_type, 0)
    );
    Function *map_access_pair_fun = TreeItAccess.getSizeFunction();
    TreeItAccess.CreateSizeFunctionFromPrototype(map_access_pair_fun);
    auto *map_pair = builder->CreateCall(
        map_access_pair_fun, map_it_alloc
    );

    auto *map_pair_first_gep = builder->CreateConstInBoundsGEP2_32(
        map_pair, 0, 0
    );
   
    // PLACE CALL TO SIZE FOR FIRST TYPE HERE
    
    // Call the size function
    auto outEdges = g->outEdges(v);

    // Must be two edges
    assert(!(outEdges.size() != 2));

    // Select the edge with idx = 0
    TG_edgeIterator edge1, edge2;
    if (outEdges[0]->second.index == 0) {
        edge1 = outEdges[0]; edge2 = outEdges[1];
    }

    if (outEdges[1]->second.index == 0) {
        edge1 = outEdges[1]; edge2 = outEdges[0];
    }

    auto target_val_fun = BTH->runOnGraphEdge(edge1);

    auto *size_call_1 = builder->CreateCall(
        target_val_fun, ArrayRef<Value*>(map_pair_first_gep)
    );

    // Add result to size sum
    auto *size_sum_load = builder->CreateLoad(size_alloc);
          size_sum_load->setAlignment(8);

    auto *size_sum_add = builder->CreateNUWAdd(size_call_1, size_sum_load);

    // Store
    auto *size_sum_store = builder->CreateStore(size_sum_add, size_alloc);
          size_sum_store->setAlignment(8);

    map_pair = builder->CreateCall(map_access_pair_fun, map_it_alloc);
    auto *map_pair_second_gep = builder->CreateConstInBoundsGEP2_32(
        map_pair, 0, 1
    );
    
    // PLACE CALL TO SIZE FOR SECOND TYPE HERE
    target_val_fun = BTH->runOnGraphEdge(edge2);

    auto *size_call_2 = builder->CreateCall(
        target_val_fun, ArrayRef<Value*>(map_pair_second_gep)
    );

    // Add result to size sum
    size_sum_load = builder->CreateLoad(size_alloc);
    size_sum_load->setAlignment(8);

    size_sum_add = builder->CreateNUWAdd(size_call_2, size_sum_load);

    // Store
    size_sum_store = builder->CreateStore(size_sum_add, size_alloc);
    size_sum_store->setAlignment(8);
    
    STLMapHandler_ElementIteratorInc IterInc(
        this, PointerType::get(Rb_tree_it_type, 0)
    );
    Function *map_it_inc_fun = IterInc.getSizeFunction();
    IterInc.CreateSizeFunctionFromPrototype(map_it_inc_fun);
    builder->CreateCall(map_it_inc_fun, map_it_alloc);

    builder->CreateBr(main_loop);

    builder->SetInsertPoint(ret_false_BB);
    
    // Return the cumulated size
    auto *size_sum_ret_load = builder->CreateLoad(size_alloc);
          size_sum_ret_load->setAlignment(8);

    builder->CreateRet(size_sum_ret_load);

}

void STLMapHandler_ElementIterator::createSizeFunction() {  
    
    // Check if there already is an _Rb_tree_iterator type defined
    StructType *Rb_tree_it_type = M->getTypeByName(
        "struct.std::_Rb_tree_iterator"
    );
    PointerType *t = PointerType::get(Rb_tree_it_type, 0);

    auto *iter_alloc = builder->CreateAlloca(t);
    iter_alloc->setAlignment(8);

    auto *iter_store = builder->CreateStore(f_arg, iter_alloc);
    iter_store->setAlignment(8);

    auto *iter_load = builder->CreateLoad(iter_alloc);

    auto *iter_gep = builder->CreateConstInBoundsGEP2_32(iter_load, 0, 0); 

    // Let the iterator point to _Rb_tree_node_base
    builder->CreateStore(
        ConstantPointerNull::get(
            cast<PointerType>(Rb_tree_it_type->getContainedType(0))
        ), iter_gep
    );

    builder->CreateRetVoid();

}

void STLMapHandler_TreeIteratorBeginOrEndL2::createSizeFunction() {
    
    StructType *Rb_tree_it_type = M->getTypeByName(
        "struct.std::_Rb_tree_iterator"
    );
    auto *it_alloc = builder->CreateAlloca(Rb_tree_it_type);
    it_alloc->setAlignment(8); 

    auto *tree_alloc = builder->CreateAlloca(t);
    tree_alloc->setAlignment(8);

    auto *tree_store = builder->CreateStore(f_arg, tree_alloc);
    tree_store->setAlignment(8);

    auto *tree_load = builder->CreateLoad(tree_alloc);

    auto *tree_gep = builder->CreateConstInBoundsGEP2_32(tree_load, 0, 0);
    tree_gep = builder->CreateConstInBoundsGEP2_32(tree_gep, 0, 1);

    //StructType *node_type = arg.getMapNodeType();
    StructType *node_type = v->second.MapNodeType;

    Value *tree_base_to_node_cast = NULL;

    if (begin) {

        tree_gep = builder->CreateConstInBoundsGEP2_32(tree_gep, 0, 2);

        auto *tree_base_load = builder->CreateLoad(tree_gep);
        tree_base_load->setAlignment(8);
   
        tree_base_to_node_cast = builder->CreateBitCast(
            tree_base_load,
            PointerType::get(node_type, 0)
        );

    } else {

        tree_base_to_node_cast = builder->CreateBitCast(
            tree_gep,
            PointerType::get(node_type, 0)
        );

    }
    
    STLMapHandler_CreateRbTreeIterator CreateRbTreeIter(
        this, it_alloc->getType(), PointerType::get(node_type, 0)
    );
    Function *size_fun = CreateRbTreeIter.getSizeFunction();
    CreateRbTreeIter.CreateSizeFunctionFromPrototype(size_fun);
    builder->CreateCall2(
        size_fun, it_alloc, tree_base_to_node_cast
    );

    auto *rb_tree_it_gep = builder->CreateConstInBoundsGEP2_32(it_alloc, 0, 0);

    auto *rb_tree_base_load = builder->CreateLoad(rb_tree_it_gep);
        
    builder->CreateRet(rb_tree_base_load);

}

void STLMapHandler_CreateRbTreeIterator::createSizeFunction() {

    // Name first argument as "this"
    auto arg_it = f->arg_begin();
    arg_it->setName("it");

    auto arg_tree_node = ++(f->arg_begin());
    arg_tree_node->setName("tree_node");

    auto *it_alloc = builder->CreateAlloca(it_ty);
    it_alloc->setAlignment(8);

    auto *tree_node_alloc = builder->CreateAlloca(node_ty);
    tree_node_alloc->setAlignment(8);

    auto *it_store = builder->CreateStore(arg_it, it_alloc);
    it_store->setAlignment(8);

    auto *tree_node_store = builder->CreateStore(
        arg_tree_node, tree_node_alloc
    );
    tree_node_store->setAlignment(8);

    auto *it_load = builder->CreateLoad(it_alloc);

    auto *it_gep = builder->CreateConstInBoundsGEP2_32(it_load, 0, 0);

    auto *tree_node_load = builder->CreateLoad(tree_node_alloc);
    tree_node_load->setAlignment(8);

    auto *tree_node_to_base_cast = builder->CreateBitCast(
        tree_node_load,
        it_gep->getType()->getContainedType(0)
    );

    auto *tree_node_base_store = builder->CreateStore(
        tree_node_to_base_cast, it_gep
    );
    tree_node_base_store->setAlignment(8);

    builder->CreateRetVoid();

}

void STLMapHandler_TreeIteratorBeginOrEnd::createSizeFunction() {

    StructType *Rb_tree_node_base_type = M->getTypeByName(
        "struct.std::_Rb_tree_node_base"
    );
    if (Rb_tree_node_base_type == NULL) {
        errs() << "Rb_tree_node_base_type not found!\n";
        assert(0);
    }

    PointerType *Rb_tree_node_base_ptr_type = PointerType::get(
        Rb_tree_node_base_type, 0
    );
    
    StructType *Rb_tree_it_type = M->getTypeByName(
        "struct.std::_Rb_tree_iterator"
    );
    
    auto *it_alloc = builder->CreateAlloca(Rb_tree_it_type);
    it_alloc->setAlignment(8); 

    auto *map_alloc = builder->CreateAlloca(t);
    map_alloc->setAlignment(8);

    auto *map_store = builder->CreateStore(f_arg, map_alloc);
    map_store->setAlignment(8);

    auto *map_load = builder->CreateLoad(map_alloc);

    auto *map_gep = builder->CreateConstInBoundsGEP2_32(map_load, 0, 0);
    
    STLMapHandler_TreeIteratorBeginOrEndL2 TreeIterBeginOrEndL2(
       this, begin,
       Rb_tree_node_base_ptr_type,  
       cast<PointerType>(map_gep->getType())
    );

    Function *tree_iter_begin_or_end_l2_fun = TreeIterBeginOrEndL2.getSizeFunction();
    TreeIterBeginOrEndL2.CreateSizeFunctionFromPrototype(tree_iter_begin_or_end_l2_fun);

    auto *tree_map_it_2nd = builder->CreateCall(
        tree_iter_begin_or_end_l2_fun,
        map_gep
    );

    auto *rb_tree_it_gep = builder->CreateConstInBoundsGEP2_32(it_alloc, 0, 0);

    builder->CreateStore(tree_map_it_2nd, rb_tree_it_gep);

    rb_tree_it_gep = builder->CreateConstInBoundsGEP2_32(it_alloc, 0, 0);

    auto *rb_tree_node_base_load = builder->CreateLoad(rb_tree_it_gep);

    builder->CreateRet(rb_tree_node_base_load);

}

void STLMapHandler_MapSize::createSizeFunction() {  
    
    auto *map_alloc = builder->CreateAlloca(t);
    map_alloc->setAlignment(8);

    auto *map_store = builder->CreateStore(f_arg, map_alloc);
    map_store->setAlignment(8);

    auto *map_load = builder->CreateLoad(map_alloc);

    auto *rb_tree_gep = builder->CreateConstInBoundsGEP2_32(map_load, 0, 0);

    auto *rb_tree_alloc = builder->CreateAlloca(rb_tree_gep->getType());
    rb_tree_alloc->setAlignment(8);

    auto *rb_tree_store = builder->CreateStore(rb_tree_gep, rb_tree_alloc);
    rb_tree_store->setAlignment(8);

    auto *rb_tree_load = builder->CreateLoad(rb_tree_alloc);

    rb_tree_gep = builder->CreateConstInBoundsGEP2_32(rb_tree_load, 0, 0);
    rb_tree_gep = builder->CreateConstInBoundsGEP2_32(rb_tree_gep, 0, 2);

    auto *rb_size_load = builder->CreateLoad(rb_tree_gep);
    rb_size_load->setAlignment(8);

    // Return the size
    builder->CreateRet(rb_size_load);

}

void STLMapHandler_TreeIteratorCmp::createSizeFunction() {
    
    // Name first argument as "this"
    auto arg_it1 = f->arg_begin();
    arg_it1->setName("it1");
    
    auto arg_it2 = ++(f->arg_begin());
    arg_it2->setName("it2");
    
    auto *it1_alloc = builder->CreateAlloca(t);
    it1_alloc->setAlignment(8);

    auto *it2_alloc = builder->CreateAlloca(t);
    it2_alloc->setAlignment(8);

    auto *it1_store = builder->CreateStore(arg_it1, it1_alloc);
    it1_store->setAlignment(8);

    auto *it2_store = builder->CreateStore(arg_it2, it2_alloc);
    it2_store->setAlignment(8);

    auto *it1_load = builder->CreateLoad(it1_alloc);

    auto *it1_gep = builder->CreateConstInBoundsGEP2_32(it1_load, 0, 0);

    auto *tree_node_base_1_load = builder->CreateLoad(it1_gep);
    tree_node_base_1_load->setAlignment(8);

    auto *it2_load = builder->CreateLoad(it2_alloc);

    auto *it2_gep = builder->CreateConstInBoundsGEP2_32(it2_load, 0, 0);

    auto *tree_node_base_2_load = builder->CreateLoad(it2_gep);
    tree_node_base_2_load->setAlignment(8);

    auto *it_cmp = builder->CreateICmpNE(
        tree_node_base_1_load,
        tree_node_base_2_load
    );

    builder->CreateRet(it_cmp);

}
    
void STLMapHandler_TreeIteratorAccess::createSizeFunction() {

    auto *it_alloc = builder->CreateAlloca(it_ty);
    it_alloc->setAlignment(8);

    auto *it_store = builder->CreateStore(f_arg, it_alloc);
    it_store->setAlignment(8);

    auto *it_load = builder->CreateLoad(it_alloc);

    auto *it_gep = builder->CreateConstInBoundsGEP2_32(it_load, 0, 0);

    auto *tree_node_base_load = builder->CreateLoad(it_gep);
    tree_node_base_load->setAlignment(8);

    auto *tree_node_base_to_tree_node_cast = builder->CreateBitCast(
        tree_node_base_load,
        PointerType::get(node_ty, 0)
    );

    auto *tree_node_gep = builder->CreateConstInBoundsGEP2_32(
        tree_node_base_to_tree_node_cast,
        0,
        1
    );

    builder->CreateRet(tree_node_gep);

}
    
void STLMapHandler_ElementIteratorInc::createSizeFunction() {
    
    auto *it_alloc = builder->CreateAlloca(t);
    it_alloc->setAlignment(8);

    auto *it_store = builder->CreateStore(f_arg, it_alloc);
    it_store->setAlignment(8);

    auto *it_load = builder->CreateLoad(it_alloc);

    auto *it_gep = builder->CreateConstInBoundsGEP2_32(it_load, 0, 0);

    auto *tree_node_base_load = builder->CreateLoad(it_gep);
    tree_node_base_load->setAlignment(8);

    Function *tree_inc_fun = cast<Function>(M->getOrInsertFunction(
        "_ZSt18_Rb_tree_incrementPSt18_Rb_tree_node_base",
        tree_node_base_load->getType(),
        tree_node_base_load->getType(),
        NULL
    ));
    auto *tree_inc = builder->CreateCall(tree_inc_fun, tree_node_base_load);

    it_gep = builder->CreateConstInBoundsGEP2_32(it_load, 0, 0);

    auto *tree_node_base_store = builder->CreateStore(tree_inc, it_gep);
    tree_node_base_store->setAlignment(8);

    builder->CreateRet(it_load);

}

RegisterTypeHandler<STLMapHandler> STLMapHandler::reg(
    Types::STL_CONTAINER_MAP
);
