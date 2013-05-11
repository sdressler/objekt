#include <LLVMGraph/KDV/TypeHandlers/TypeHandlers.h>

#include <llvm/Support/raw_ostream.h>

void ArrayHandler::createSizeFunction() {

    // This is for the result
    Value *x = ConstantInt::get(IntegerType::get(M->getContext(), 64), 0);

    // Get the outgoing edge, to get the base type
    auto out_edge = g->outEdges(v)[0];

    // Retrieve the size function for this edge
    Function *size_F = BTH->runOnGraphEdge(out_edge);

    // Create a call to this function
    // We simply use the first element for base size
    auto *base_elem = builder->CreateConstInBoundsGEP2_32(f_arg, 0, 0);
    auto *base_size = builder->CreateCall(size_F, base_elem);

    // Strip Pointer if needed
    llvm::ArrayType *array;
    if (v->second.isPointer) {
        array = llvm::dyn_cast<llvm::ArrayType>(v->second.type->getPointerElementType());
    } else {
        array = llvm::dyn_cast<llvm::ArrayType>(v->second.type);
    }

    assert(array && "Could not convert to ArrayType");

    // Multiply result with number of elements in array
    auto *num_elems = llvm::ConstantInt::get(
    	IntegerType::get(M->getContext(), 64), array->getArrayNumElements()
    );

    // Multiply base size with number of elements
    x = builder->CreateMul(base_size, num_elems);

	// Create a return
	builder->CreateRet(x);

} 

RegisterTypeHandler<ArrayHandler> ArrayHandler::reg(Types::ARRAY);
