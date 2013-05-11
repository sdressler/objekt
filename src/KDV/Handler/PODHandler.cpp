#include <LLVMGraph/KDV/TypeHandlers/TypeHandlers.h>

#include <llvm/DataLayout.h>

void PODHandler::createSizeFunction() {

    DataLayout DL(M);

    //llvm::errs() << "POD: " << e->second.ptrDepth << " "; e->second.type->dump(); llvm::errs() << " " << DL.getTypeSizeInBits(e->second.type) / 8 << "\n";

    llvm::Type *this_type = e->second.type;
    if (e->second.ptrDepth > 0) {
        this_type = this_type->getPointerElementType();
    }

    builder->CreateRet(
        ConstantInt::get(
            Type::getInt64Ty(M->getContext()),   // Type definition
            //(uint64_t)(arg.bitwidth / 8),        // Size
            DL.getTypeSizeInBits(this_type) / 8,
            false                               // Signed
        )
    );

}

RegisterTypeHandler<PODHandler> PODHandler::reg(Types::POD);
