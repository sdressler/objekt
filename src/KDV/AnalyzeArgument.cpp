#include <LLVMGraph/KDV/ArgParser.h>

#include <llvm/Support/raw_ostream.h>
#include <llvm/DataLayout.h>

GraphVertexProperties ArgParser::analyzeArgument(Value *arg, Type *type) {

    //ArgType at;
    GraphVertexProperties at;

    Type *t = NULL;

    if (type == NULL) {
        
        // Check if it is a BitCast, if so we need the SrcTy!
        if (llvm::BitCastInst::classof(arg)) {
            t = llvm::dyn_cast<BitCastInst>(arg)->getSrcTy();
        } else {
            t = arg->getType();
        }

    } else if (arg == NULL) {

        t = type;

    }

    assert(t && "t == NULL");

    if (t->isDoubleTy()) {

        at.name = "Double";
        at.t = Types::POD;
        at.bitwidth = 64;

    } else if (t->isIntegerTy()) {

        at.name = "Integer";
        at.t = Types::POD;
        at.bitwidth = t->getIntegerBitWidth();

    } else if (t->isFloatTy()) {

        at.name = "Float";
        at.t = Types::POD;
        at.bitwidth = 32;

    } else if (t->isArrayTy()) {

        at.name = "Array";
        at.t = Types::ARRAY;

        //auto *array = llvm::dyn_cast<llvm::ArrayType>(t);

        //at.elems = array->getArrayNumElements();
        //at.bitwidth = array->getArrayElementType()


    } else if (t->isStructTy()) {

        auto *st = cast<StructType>(t);

        auto tName = st->getStructName();       
        at.name = tName;
        
        // Capture the data size of the struct
        // Module M is defined in header
        DataLayout DL(M);

        // Get the type of the structure
        if (tName.find("class.std::vector") != std::string::npos) {
        
            at.t = Types::STL_CONTAINER_VECTOR;
            errs() << "Container Template Type: "; t->dump(); errs() << "\n";
//            analyzeContainerTemplateType(st, tName);

        } else if (tName.find("class.std::map") != std::string::npos) {

            at.t = Types::STL_CONTAINER_MAP;

        } else if (tName.find("class.std::basic_string") != std::string::npos) {

            at.t = Types::STL_STRING;
            
            // Note: std::string contains *char, therefore std::string::size
            // needs to divide by bitwidth of pointer at the very end.
            at.bitwidth = DL.getPointerSizeInBits();

        } else {

            // Use the class analyzer to investigate the struct more deeply
            at.t = Types::STRUCT;
            at.name = st->getStructName(); 
            at.bitwidth = DL.getStructLayout(const_cast<StructType*>(st))->getSizeInBytes() * 8;
        }

    } else if (t->isPointerTy()) {
        
        unsigned int ptrDepth = 0;

        llvm::Type *ptr = t;
        while (llvm::dyn_cast<PointerType>(ptr) != NULL) {
            ptrDepth++;
            ptr = ptr->getContainedType(0);
        }

        at = analyzeArgument(NULL, ptr);

        at.isPointer = true;
        at.ptrDepth = ptrDepth;

    } else if (t->isFunctionTy()) {

        errs() << "Warning: FunctionTy was ignored.\n";
        at.name = "_function";
        at.t = Types::FUNCTION;
        return at;
    
    } else {
        errs() << "Not recognized Type: " << t->getTypeID() << ", ";
        t->dump();
        errs() << "\n";
        assert(0);
    }

    at.value = arg;
    at.type = t;

    return at;
}
