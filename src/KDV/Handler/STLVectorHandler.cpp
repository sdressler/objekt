#include <LLVMGraph/KDV/TypeHandlers/STLVectorHandler.h>

#include <llvm/Support/raw_ostream.h>

#include <stdlib.h>

void STLVectorHandler::createSizeFunction() {

	StructType *base_type_struct = cast<StructType>(t->getContainedType(0));
	StructType *base_type_base_struct = cast<StructType>(base_type_struct->getElementType(0));
    StructType *base_type_alloc_struct = cast<StructType>(base_type_base_struct->getElementType(0));
    template_type = base_type_alloc_struct->getElementType(0);

    iterator_type = llvm::StructType::create(template_type, getSizeFunctionName() + "_iterator_ty");

	// Create additional functions
	STLVectorHandler_IteratorBegin iter_begin(this, template_type, iterator_type);
	Function *iter_begin_fun = iter_begin.getSizeFunction();
	iter_begin.CreateSizeFunctionFromPrototype(iter_begin_fun);

	STLVectorHandler_IteratorEnd iter_end(this, template_type, iterator_type);
	Function *iter_end_fun = iter_end.getSizeFunction();
	iter_end.CreateSizeFunctionFromPrototype(iter_end_fun);

	STLVectorHandler_IteratorOpNeq iter_op_neq(this, template_type, iterator_type);
	Function *iter_op_neq_fun = iter_op_neq.getSizeFunction();
	iter_op_neq.CreateSizeFunctionFromPrototype(iter_op_neq_fun);

	STLVectorHandler_IteratorOpIndir iter_op_indir(this, template_type, iterator_type);
	Function *iter_op_indir_fun = iter_op_indir.getSizeFunction();
	iter_op_indir.CreateSizeFunctionFromPrototype(iter_op_indir_fun);

    STLVectorHandler_IteratorOpInc iter_op_inc(this, iterator_type);
    Function *iter_op_inc_fun = iter_op_inc.getSizeFunction();
    iter_op_inc.CreateSizeFunctionFromPrototype(iter_op_inc_fun);

	// Define additionally needed BBs
	BasicBlock *BB_for_cond = BasicBlock::Create(M->getContext(), "for.cond", f);
	BasicBlock *BB_for_body = BasicBlock::Create(M->getContext(), "for.body", f);
	BasicBlock *BB_for_inc  = BasicBlock::Create(M->getContext(), "for.inc" , f);
	BasicBlock *BB_for_end  = BasicBlock::Create(M->getContext(), "for.end", f);

	auto *this_addr_alloc = builder->CreateAlloca(t, 0, "vec.addr");
	      this_addr_alloc->setAlignment(8);

	auto *size_alloc      = builder->CreateAlloca(IntegerType::get(M->getContext(), 64), 0, "size");
		  size_alloc->setAlignment(8);

    auto *iter_alloc      = builder->CreateAlloca(iterator_type, 0, "it");
          iter_alloc->setAlignment(8);

    auto *iter_alloc_ref  = builder->CreateAlloca(iterator_type, 0, "ref.tmp");
          iter_alloc_ref->setAlignment(8);

    auto *store_this      = builder->CreateStore(f_arg, this_addr_alloc);
          store_this->setAlignment(8);

    auto *store_size      = builder->CreateStore(ConstantInt::get(IntegerType::getInt64Ty(M->getContext()), 0), size_alloc);
          store_size->setAlignment(8);

    auto *this_addr_load  = builder->CreateLoad(this_addr_alloc);
          this_addr_load->setAlignment(8);


    auto *iter_begin_call = builder->CreateCall(iter_begin_fun, this_addr_load, "iter_begin_call");

    auto *iter_begin_gep  = builder->CreateConstInBoundsGEP2_32(iter_alloc, 0, 0, "iter_begin");
                            builder->CreateStore(iter_begin_call, iter_begin_gep);

                            builder->CreateBr(BB_for_cond);

/*
 * define i32 @_Z8vec_sizePSt6vectorIP1ASaIS1_EE(%"class.std::vector"* %vec) nounwind uwtable {
entry:
  %vec.addr = alloca %"class.std::vector"*, align 8
  %size = alloca i32, align 4
  %it = alloca %"class.__gnu_cxx::__normal_iterator", align 8
  %ref.tmp = alloca %"class.__gnu_cxx::__normal_iterator", align 8
  store %"class.std::vector"* %vec, %"class.std::vector"** %vec.addr, align 8
  store i32 0, i32* %size, align 4
  %0 = load %"class.std::vector"** %vec.addr, align 8

  %call = call %class.A** @_ZNSt6vectorIP1ASaIS1_EE5beginEv(%"class.std::vector"* %0)

  %coerce.dive = getelementptr %"class.__gnu_cxx::__normal_iterator"* %it, i32 0, i32 0
  store %class.A** %call, %class.A*** %coerce.dive
  br label %for.cond
*/

    builder->SetInsertPoint(BB_for_cond);

    auto *vec_addr_load  = builder->CreateLoad(this_addr_alloc);
          vec_addr_load->setAlignment(8);

    auto *iter_end_call  = builder->CreateCall(iter_end_fun, vec_addr_load, "iter_end_call");

    auto *iter_end_gep   = builder->CreateConstInBoundsGEP2_32(iter_alloc_ref, 0, 0, "iter_end");
                           builder->CreateStore(iter_end_call, iter_end_gep);

    auto *iter_cmp_call  = builder->CreateCall2(iter_op_neq_fun, iter_alloc, iter_alloc_ref, "iter_cmp_call");

                           builder->CreateCondBr(iter_cmp_call, BB_for_body, BB_for_end);

/*
for.cond:                                         ; preds = %for.inc, %entry
  %1 = load %"class.std::vector"** %vec.addr, align 8
  %call1 = call %class.A** @_ZNSt6vectorIP1ASaIS1_EE3endEv(%"class.std::vector"* %1)
  %coerce.dive2 = getelementptr %"class.__gnu_cxx::__normal_iterator"* %ref.tmp, i32 0, i32 0
  store %class.A** %call1, %class.A*** %coerce.dive2
  %call3 = call zeroext i1 @_ZN9__gnu_cxxneIPP1ASt6vectorIS2_SaIS2_EEEEbRKNS_17__normal_iteratorIT_T0_EESC_(%"class.__gnu_cxx::__normal_iterator"* %it, %"class.__gnu_cxx::__normal_iterator"* %ref.tmp)
  br i1 %call3, label %for.body, label %for.end
*/

    builder->SetInsertPoint(BB_for_body);

    llvm::Value *iter_indir = builder->CreateCall(iter_op_indir_fun, iter_alloc, "iter_indir_call");

    if (llvm::dyn_cast<llvm::PointerType>(iter_indir->getType()->getPointerElementType()) != NULL) {
        iter_indir = builder->CreateLoad(iter_indir);
    }

    // Get the size function of the contained element
    auto oe = g->outEdges(v)[0];
    auto *size_F = BTH->runOnGraphEdge(oe);

/*
    size_F->dump();
    iter_indir->dump();
    iter_indir->getType()->dump();
*/
    auto *size_fun_call  = builder->CreateCall(size_F, iter_indir, "size_fun_call");

    auto *size_load      = builder->CreateLoad(size_alloc);
          size_load->setAlignment(8);

    // Add sizes
    auto *sizes_add      = builder->CreateNSWAdd(size_load, size_fun_call, "size.add");

    // Write back
    auto *sizes_store    = builder->CreateStore(sizes_add, size_alloc);
          sizes_store->setAlignment(8);

                         builder->CreateBr(BB_for_inc);


/*
for.body:                                         ; preds = %for.cond
  %call4 = call %class.A** @_ZNK9__gnu_cxx17__normal_iteratorIPP1ASt6vectorIS2_SaIS2_EEEdeEv(%"class.__gnu_cxx::__normal_iterator"* %it)
  %2 = load %class.A** %call4
  %call5 = call i32 @_ZN1A4sizeEv(%class.A* %2)
  %3 = load i32* %size, align 4
  %add = add nsw i32 %3, %call5
  store i32 %add, i32* %size, align 4
  br label %for.inc
*/


    builder->SetInsertPoint(BB_for_inc);

    builder->CreateCall(iter_op_inc_fun, iter_alloc, "iter_inc_call");
    builder->CreateBr(BB_for_cond);


/*
for.inc:                                          ; preds = %for.body
  %call6 = call %"class.__gnu_cxx::__normal_iterator"* @_ZN9__gnu_cxx17__normal_iteratorIPP1ASt6vectorIS2_SaIS2_EEEppEv(%"class.__gnu_cxx::__normal_iterator"* %it)
  br label %for.cond
*/

    builder->SetInsertPoint(BB_for_end);

    auto *size_final_load = builder->CreateLoad(size_alloc);
          size_final_load->setAlignment(8);

                            builder->CreateRet(size_final_load);

/*
for.end:                                          ; preds = %for.cond
  %4 = load i32* %size, align 4
  ret i32 %4
}

*/

}

void STLVectorHandler_IteratorBegin::createSizeFunction() {

    STLVectorHandler_IteratorL1 iterator_l1(this, template_type, iterator_type);
    Function *iterator_fun = iterator_l1.getSizeFunction();
    iterator_l1.CreateSizeFunctionFromPrototype(iterator_fun);

	auto *retval_alloca    = builder->CreateAlloca(iterator_type, 0);
		  retval_alloca->setAlignment(8);

    auto *this_addr_alloca = builder->CreateAlloca(t, 0, "this.addr");
          this_addr_alloca->setAlignment(8);

    auto *store_this       = builder->CreateStore(f_arg, this_addr_alloca);
          store_this->setAlignment(8);

    auto *load_this        = builder->CreateLoad(this_addr_alloca, "this");

    auto *cast_to_base     = builder->CreateBitCast(
    		                     load_this,
    		                     PointerType::get(
    		                         t->getContainedType(0)->getContainedType(0),
    		                         0
    		                     )
    		                 );

    auto *gep_impl         = builder->CreateConstInBoundsGEP2_32(cast_to_base, 0, 0, "_M_impl");
    auto *gep_start        = builder->CreateConstInBoundsGEP2_32(gep_impl, 0, 0, "_M_start");

                             builder->CreateCall2(iterator_fun, retval_alloca, gep_start);

    auto *gep_iter         = builder->CreateConstInBoundsGEP2_32(retval_alloca, 0, 0, "coerce.dive");
    auto *iter_load        = builder->CreateLoad(gep_iter);

    builder->CreateRet(iter_load);

	/*
	 * define linkonce_odr %class.A** @_ZNSt6vectorIP1ASaIS1_EE5beginEv(%"class.std::vector"* %this) nounwind uwtable align 2 {
entry:
  %retval = alloca %"class.__gnu_cxx::__normal_iterator", align 8
  %this.addr = alloca %"class.std::vector"*, align 8
  store %"class.std::vector"* %this, %"class.std::vector"** %this.addr, align 8
  %this1 = load %"class.std::vector"** %this.addr
  %0 = bitcast %"class.std::vector"* %this1 to %"struct.std::_Vector_base"*
  %_M_impl = getelementptr inbounds %"struct.std::_Vector_base"* %0, i32 0, i32 0
  %_M_start = getelementptr inbounds %"struct.std::_Vector_base<A *, std::allocator<A *> >::_Vector_impl"* %_M_impl, i32 0, i32 0

  call void @_ZN9__gnu_cxx17__normal_iteratorIPP1ASt6vectorIS2_SaIS2_EEEC1ERKS3_(%"class.__gnu_cxx::__normal_iterator"* %retval, %class.A*** %_M_start)

  %coerce.dive = getelementptr %"class.__gnu_cxx::__normal_iterator"* %retval, i32 0, i32 0
  %1 = load %class.A*** %coerce.dive
  ret %class.A** %1
}
	 *
	 */

}

void STLVectorHandler_IteratorEnd::createSizeFunction() {

    STLVectorHandler_IteratorL1 iterator_l1(this, template_type, iterator_type);
    Function *iterator_fun = iterator_l1.getSizeFunction();
    iterator_l1.CreateSizeFunctionFromPrototype(iterator_fun);

    auto *retval_alloca    = builder->CreateAlloca(iterator_type, 0);
          retval_alloca->setAlignment(8);

    auto *this_addr_alloca = builder->CreateAlloca(t, 0, "this.addr");
          this_addr_alloca->setAlignment(8);

    auto *store_this       = builder->CreateStore(f_arg, this_addr_alloca);
          store_this->setAlignment(8);

    auto *load_this        = builder->CreateLoad(this_addr_alloca, "this");

    auto *cast_to_base     = builder->CreateBitCast(
                                 load_this,
                                 PointerType::get(
                                     t->getContainedType(0)->getContainedType(0),
                                     0
                                 )
                             );

    auto *gep_impl         = builder->CreateConstInBoundsGEP2_32(cast_to_base, 0, 0, "_M_impl");
    auto *gep_finish       = builder->CreateConstInBoundsGEP2_32(gep_impl, 0, 1, "_M_finish");

                             builder->CreateCall2(iterator_fun, retval_alloca, gep_finish);

    auto *gep_iter         = builder->CreateConstInBoundsGEP2_32(retval_alloca, 0, 0, "coerce.dive");
    auto *iter_load        = builder->CreateLoad(gep_iter);

    builder->CreateRet(iter_load);

    /*
define linkonce_odr %class.A** @_ZNSt6vectorIP1ASaIS1_EE3endEv(%"class.std::vector"* %this) nounwind uwtable align 2 {
entry:
  %retval = alloca %"class.__gnu_cxx::__normal_iterator", align 8
  %this.addr = alloca %"class.std::vector"*, align 8
  store %"class.std::vector"* %this, %"class.std::vector"** %this.addr, align 8
  %this1 = load %"class.std::vector"** %this.addr
  %0 = bitcast %"class.std::vector"* %this1 to %"struct.std::_Vector_base"*
  %_M_impl = getelementptr inbounds %"struct.std::_Vector_base"* %0, i32 0, i32 0
  %_M_finish = getelementptr inbounds %"struct.std::_Vector_base<A *, std::allocator<A *> >::_Vector_impl"* %_M_impl, i32 0, i32 1
  call void @_ZN9__gnu_cxx17__normal_iteratorIPP1ASt6vectorIS2_SaIS2_EEEC1ERKS3_(%"class.__gnu_cxx::__normal_iterator"* %retval, %class.A*** %_M_finish)
  %coerce.dive = getelementptr %"class.__gnu_cxx::__normal_iterator"* %retval, i32 0, i32 0
  %1 = load %class.A*** %coerce.dive
  ret %class.A** %1
}
     */

    //f->dump();
    //std::terminate();

    //exit(0);

}

void STLVectorHandler_IteratorL1::createSizeFunction() {

    STLVectorHandler_IteratorL2 iterator_l2(
        this,
        template_type->getPointerElementType(),
        iterator_type->getPointerElementType()
    );
    Function *iterator_l2_fun = iterator_l2.getSizeFunction();
    iterator_l2.CreateSizeFunctionFromPrototype(iterator_l2_fun);

    // Rename args
    auto arg_this = f->arg_begin();

    auto arg_i = f->arg_begin();
    arg_i++;

    arg_i->setName("__i");

    auto *this_addr_alloc = builder->CreateAlloca(iterator_type, 0, "this.addr");
          this_addr_alloc->setAlignment(8);

    auto *i_addr_alloc    = builder->CreateAlloca(template_type, 0, "__i.addr");
          i_addr_alloc->setAlignment(8);

    auto *store_this_addr = builder->CreateStore(arg_this, this_addr_alloc);
          store_this_addr->setAlignment(8);

    auto *store_i_addr    = builder->CreateStore(arg_i, i_addr_alloc);
          store_i_addr->setAlignment(8);

    auto *load_this       = builder->CreateLoad(this_addr_alloc, "this");
    auto *load_i          = builder->CreateLoad(i_addr_alloc);

                            builder->CreateCall2(iterator_l2_fun, load_this, load_i);

    builder->CreateRetVoid();

    /*
     * define linkonce_odr void @_ZN9__gnu_cxx17__normal_iteratorIPP1ASt6vectorIS2_SaIS2_EEEC1ERKS3_(%"class.__gnu_cxx::__normal_iterator"* %this, %class.A*** %__i) unnamed_addr nounwind uwtable align 2 {
entry:
  %this.addr = alloca %"class.__gnu_cxx::__normal_iterator"*, align 8
  %__i.addr = alloca %class.A***, align 8
  store %"class.__gnu_cxx::__normal_iterator"* %this, %"class.__gnu_cxx::__normal_iterator"** %this.addr, align 8
  store %class.A*** %__i, %class.A**** %__i.addr, align 8
  %this1 = load %"class.__gnu_cxx::__normal_iterator"** %this.addr
  %0 = load %class.A**** %__i.addr
  call void @_ZN9__gnu_cxx17__normal_iteratorIPP1ASt6vectorIS2_SaIS2_EEEC2ERKS3_(%"class.__gnu_cxx::__normal_iterator"* %this1, %class.A*** %0)
  ret void
}


     *
     */
}

void STLVectorHandler_IteratorL2::createSizeFunction() {

    // Rename args
    auto arg_this = f->arg_begin();

    auto arg_i = f->arg_begin();
    arg_i++;

    arg_i->setName("__i");

    auto *this_addr_alloc = builder->CreateAlloca(iterator_type, 0, "this.addr");
          this_addr_alloc->setAlignment(8);

    auto *i_addr_alloc    = builder->CreateAlloca(template_type, 0, "__i.addr");
          i_addr_alloc->setAlignment(8);

    auto *store_this_addr = builder->CreateStore(arg_this, this_addr_alloc);
          store_this_addr->setAlignment(8);

    auto *store_i_addr    = builder->CreateStore(arg_i, i_addr_alloc);
          store_i_addr->setAlignment(8);

    auto *load_this       = builder->CreateLoad(this_addr_alloc, "this");
          load_this->setAlignment(8);

    auto *gep_iter        = builder->CreateConstInBoundsGEP2_32(load_this, 0, 0, "_M_current");

    auto *load_i          = builder->CreateLoad(i_addr_alloc);
          load_i->setAlignment(8);

    auto *load_ii         = builder->CreateLoad(load_i);
          load_ii->setAlignment(8);

    auto *store_iter      = builder->CreateStore(load_ii, gep_iter);
          store_iter->setAlignment(8);

    builder->CreateRetVoid();

/*
    define linkonce_odr void @_ZN9__gnu_cxx17__normal_iteratorIPP1ASt6vectorIS2_SaIS2_EEEC2ERKS3_(%"class.__gnu_cxx::__normal_iterator"* %this, %class.A*** %__i) unnamed_addr nounwind uwtable align 2 {
    entry:
      %this.addr = alloca %"class.__gnu_cxx::__normal_iterator"*, align 8
      %__i.addr = alloca %class.A***, align 8
      store %"class.__gnu_cxx::__normal_iterator"* %this, %"class.__gnu_cxx::__normal_iterator"** %this.addr, align 8
      store %class.A*** %__i, %class.A**** %__i.addr, align 8
      %this1 = load %"class.__gnu_cxx::__normal_iterator"** %this.addr
      %_M_current = getelementptr inbounds %"class.__gnu_cxx::__normal_iterator"* %this1, i32 0, i32 0
      %0 = load %class.A**** %__i.addr, align 8
      %1 = load %class.A*** %0, align 8
      store %class.A** %1, %class.A*** %_M_current, align 8
      ret void
    }
*/

}

void STLVectorHandler_IteratorOpNeq::createSizeFunction() {

    STLVectorHandler_IteratorBase iterator_base(
        this, template_type, iterator_type->getPointerElementType()
    );
    Function *iterator_base_fun = iterator_base.getSizeFunction();
    iterator_base.CreateSizeFunctionFromPrototype(iterator_base_fun);

    // Rename args
    auto arg_lhs = f->arg_begin();
    arg_lhs->setName("__lhs");

    auto arg_rhs = f->arg_begin();
    arg_rhs++;
    arg_rhs->setName("__rhs");

    auto *lhs_addr_alloc = builder->CreateAlloca(iterator_type, 0, "__lhs.addr");
          lhs_addr_alloc->setAlignment(8);

    auto *rhs_addr_alloc = builder->CreateAlloca(iterator_type, 0, "__rhs.addr");
          rhs_addr_alloc->setAlignment(8);

    auto *lhs_store      = builder->CreateStore(arg_lhs, lhs_addr_alloc);
          lhs_store->setAlignment(8);

    auto *rhs_store      = builder->CreateStore(arg_rhs, rhs_addr_alloc);
          rhs_store->setAlignment(8);

    auto *load_lhs_iter  = builder->CreateLoad(lhs_addr_alloc);
          load_lhs_iter->setAlignment(8);

    auto *lhs_base_call  = builder->CreateCall(iterator_base_fun, load_lhs_iter, "lhs_base_call");

    auto *lhs_val        = builder->CreateLoad(lhs_base_call);

    auto *load_rhs_iter  = builder->CreateLoad(rhs_addr_alloc);
          load_rhs_iter->setAlignment(8);

    auto *rhs_base_call  = builder->CreateCall(iterator_base_fun, load_rhs_iter, "rhs_base_call");

    auto *rhs_val        = builder->CreateLoad(rhs_base_call);

    auto *cmp            = builder->CreateICmpNE(lhs_val, rhs_val, "cmp");

    builder->CreateRet(cmp);

 /*
  *
define linkonce_odr zeroext i1 @_ZN9__gnu_cxxneIPP1ASt6vectorIS2_SaIS2_EEEEbRKNS_17__normal_iteratorIT_T0_EESC_(%"class.__gnu_cxx::__normal_iterator"* %__lhs, %"class.__gnu_cxx::__normal_iterator"* %__rhs)      nounwind uwtable inlinehint {
entry:
  %__lhs.addr = alloca %"class.__gnu_cxx::__normal_iterator"*, align 8
  %__rhs.addr = alloca %"class.__gnu_cxx::__normal_iterator"*, align 8
  store %"class.__gnu_cxx::__normal_iterator"* %__lhs, %"class.__gnu_cxx::__normal_iterator"** %__lhs.addr, align 8
  store %"class.__gnu_cxx::__normal_iterator"* %__rhs, %"class.__gnu_cxx::__normal_iterator"** %__rhs.addr, align 8
  %0 = load %"class.__gnu_cxx::__normal_iterator"** %__lhs.addr, align 8
  %call = call %class.A*** @_ZNK9__gnu_cxx17__normal_iteratorIPP1ASt6vectorIS2_SaIS2_EEE4baseEv(%"class.__gnu_cxx::__normal_iterator"* %0)
  %1 = load %class.A*** %call
  %2 = load %"class.__gnu_cxx::__normal_iterator"** %__rhs.addr, align 8
  %call1 = call %class.A*** @_ZNK9__gnu_cxx17__normal_iteratorIPP1ASt6vectorIS2_SaIS2_EEE4baseEv(%"class.__gnu_cxx::__normal_iterator"* %2)
  %3 = load %class.A*** %call1
  %cmp = icmp ne %class.A** %1, %3
  ret i1 %cmp
}
  * */

}

void STLVectorHandler_IteratorBase::createSizeFunction() {

    auto *this_addr_alloc = builder->CreateAlloca(iterator_type, 0, "this.addr");
          this_addr_alloc->setAlignment(8);

    auto *this_store      = builder->CreateStore(f_arg, this_addr_alloc);
          this_store->setAlignment(8);

    auto *iter_load       = builder->CreateLoad(this_addr_alloc, "this");

    auto *iter_gep        = builder->CreateConstInBoundsGEP2_32(iter_load, 0, 0, "_M_current");

    builder->CreateRet(iter_gep);

/*
    define linkonce_odr %class.A*** @_ZNK9__gnu_cxx17__normal_iteratorIPP1ASt6vectorIS2_SaIS2_EEE4baseEv(%"class.__gnu_cxx::__normal_iterator"* %this) nounwind uwtable align 2 {
    entry:
      %this.addr = alloca %"class.__gnu_cxx::__normal_iterator"*, align 8
      store %"class.__gnu_cxx::__normal_iterator"* %this, %"class.__gnu_cxx::__normal_iterator"** %this.addr, align 8
      %this1 = load %"class.__gnu_cxx::__normal_iterator"** %this.addr
      %_M_current = getelementptr inbounds %"class.__gnu_cxx::__normal_iterator"* %this1, i32 0, i32 0
      ret %class.A*** %_M_current
    }
*/

}

void STLVectorHandler_IteratorOpIndir::createSizeFunction() {

    auto *this_addr_alloc = builder->CreateAlloca(iterator_type, 0, "this.addr");
          this_addr_alloc->setAlignment(8);

    auto *this_store      = builder->CreateStore(f_arg, this_addr_alloc);
          this_store->setAlignment(8);

    auto *iter_load       = builder->CreateLoad(this_addr_alloc, "this");

    auto *iter_gep        = builder->CreateConstInBoundsGEP2_32(iter_load, 0, 0, "_M_current");

    auto *iter_cur_load   = builder->CreateLoad(iter_gep);
          iter_cur_load->setAlignment(8);

    builder->CreateRet(iter_cur_load);


/*
    define linkonce_odr %class.A** @_ZNK9__gnu_cxx17__normal_iteratorIPP1ASt6vectorIS2_SaIS2_EEEdeEv(%"class.__gnu_cxx::__normal_iterator"* %this) nounwind uwtable align 2 {
    entry:
      %this.addr = alloca %"class.__gnu_cxx::__normal_iterator"*, align 8
      store %"class.__gnu_cxx::__normal_iterator"* %this, %"class.__gnu_cxx::__normal_iterator"** %this.addr, align 8
      %this1 = load %"class.__gnu_cxx::__normal_iterator"** %this.addr
      %_M_current = getelementptr inbounds %"class.__gnu_cxx::__normal_iterator"* %this1, i32 0, i32 0
      %0 = load %class.A*** %_M_current, align 8
      ret %class.A** %0
    }
*/

}

void STLVectorHandler_IteratorOpInc::createSizeFunction() {

    auto *this_addr_alloc = builder->CreateAlloca(iterator_type, 0, "this.addr");
          this_addr_alloc->setAlignment(8);

    auto *this_store      = builder->CreateStore(f_arg, this_addr_alloc);
          this_store->setAlignment(8);

    auto *iter_load       = builder->CreateLoad(this_addr_alloc, "this");

    auto *iter_gep        = builder->CreateConstInBoundsGEP2_32(iter_load, 0, 0, "_M_current");

    auto *iter_cur_load   = builder->CreateLoad(iter_gep);
          iter_cur_load->setAlignment(8);

    auto *gep_inc_ptr     = builder->CreateConstInBoundsGEP1_32(iter_cur_load, 1);

    auto *store_inc_ptr   = builder->CreateStore(gep_inc_ptr, iter_gep);
          store_inc_ptr->setAlignment(8);

    builder->CreateRet(iter_load);

/*
    define linkonce_odr %"class.__gnu_cxx::__normal_iterator"* @_ZN9__gnu_cxx17__normal_iteratorIPP1ASt6vectorIS2_SaIS2_EEEppEv(%"class.__gnu_cxx::__normal_iterator"* %this) nounwind uwtable align 2 {
    entry:
      %this.addr = alloca %"class.__gnu_cxx::__normal_iterator"*, align 8
      store %"class.__gnu_cxx::__normal_iterator"* %this, %"class.__gnu_cxx::__normal_iterator"** %this.addr, align 8
      %this1 = load %"class.__gnu_cxx::__normal_iterator"** %this.addr
      %_M_current = getelementptr inbounds %"class.__gnu_cxx::__normal_iterator"* %this1, i32 0, i32 0
      %0 = load %class.A*** %_M_current, align 8
      %incdec.ptr = getelementptr inbounds %class.A** %0, i32 1
      store %class.A** %incdec.ptr, %class.A*** %_M_current, align 8
      ret %"class.__gnu_cxx::__normal_iterator"* %this1
    }
*/

}

RegisterTypeHandler<STLVectorHandler> STLVectorHandler::reg(Types::STL_CONTAINER_VECTOR);
