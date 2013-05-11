/*
 * DataDirection.h
 *
 *  Created on: Jan 12, 2013
 *      Author: bzcdress
 */

#ifndef DATADIRECTION_H_
#define DATADIRECTION_H_

#include <llvm/Module.h>
#include <llvm/Function.h>
#include <LLVMGraph/ValueGraph.h>

class DataDirection {
private:
    llvm::Module *M;
    llvm::Function *Callee;
    //llvm::Value *val;

    unsigned int arg_index;

    llvm::Value *f_arg;

    VG *g;

public:
    //DataDirection(llvm::Module *_M, llvm::Value *_val, llvm::Function *_F, VG *_g) {
    DataDirection(llvm::Module *_M, llvm::Function *_Callee, llvm::Value *_f_arg, unsigned int _arg_index, VG *_g) {

        M = _M;
        Callee = _Callee;
        arg_index = _arg_index;
        g = _g;
        f_arg = _f_arg;

        assert((g != NULL) && "Graph is NULL");

    }

    DataDirection(llvm::Module *_M, llvm::Function *_Callee, llvm::Value *_f_arg, unsigned int _arg_index) {

        M = _M;
        Callee = _Callee;
        arg_index = _arg_index;
        f_arg = _f_arg;

        g = new VG();

    /*
        M = _M; val = _val; F = _F;

        if (_g == NULL) {
            g = new VG();
        } else {
            g = _g;
        }
    */
    }

    //enum DDir { IN = 0, OUT, INOUT };

    VG* getValueGraph() { return g; }

    void run();
    bool IsOutputData(llvm::Value*);

private:
    void TraceValues(VG_vertexIterator);
/*
    std::pair<uint64_t, uint64_t> getLoadStoreCount(VG_vertexIterator, VG_edgeIterator);
    void getLoadStoreCountForEdge(VG_edgeIterator&, uint64_t&, uint64_t&);
*/
};

#endif /* DATADIRECTION_H_ */
