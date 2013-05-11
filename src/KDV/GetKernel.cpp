//#include <KernelVolume/KernelVolume.h>
#include <LLVMGraph/KDV.h>

void KernelVolume::getKernelsFromGlobalAnnotations(GlobalValue *gv) {

    assert((gv != NULL) && "GlobalValue == NULL!");
    
    auto *a = gv->getOperand(0);

    if (a == NULL) {
        errs() << "Something went wrong, going to assert!\n";
        assert(0);
    }

    // Operand 0 Should be a constant array
    if (a->getValueID() != Value::ConstantArrayVal) {
        errs() << "Something went wrong, going to assert!\n";
        assert(0);
    }

    auto *annoStruct = cast<ConstantStruct>(cast<ConstantArray>(a)->getOperand(0));

    if (annoStruct == NULL) {
        errs() << "Something went wrong, going to assert!\n";
        assert(0);
    }

    // Retrieve the name of the annotation, check if it's "kernel"
    auto *aStringVariable = cast<GlobalVariable>(
        cast<ConstantExpr>(annoStruct->getOperand(1))->getOperand(0)
    );

    if (aStringVariable == NULL) {
        errs() << "Something went wrong, going to assert!\n";
        assert(0);
    }

    auto aString = cast<ConstantDataArray>(aStringVariable->getOperand(0))->getAsString();

    // Test for kernel
    if (aString.find("kernel") != std::string::npos) {
        // Proceed
        auto *top = cast<ConstantExpr>(annoStruct->getOperand(0));

        // Test if it's a cast
        if (top->isCast()) {
            KernelFunctions.insert(cast<Function>(top->getOperand(0)));
        } else {
            assert(0);
        }

    } else {
        // Should not happen
        errs() << "Something went wrong, going to assert!\n";
        assert(0);
    }

}
