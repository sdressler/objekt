#ifndef _MatvecOp_hpp_
#define _MatvecOp_hpp_

#ifndef KERNEL_PREFIX
#define KERNEL_PREFIX
#endif

template<typename MatrixType,
         typename VectorType>
struct MatvecOp {

  size_t n;

  typedef typename VectorType::GlobalOrdinalType GlobalOrdinalType;
  typedef typename VectorType::LocalOrdinalType LocalOrdinalType;
  typedef typename VectorType::ScalarType ScalarType;

  const LocalOrdinalType*  Arowoffsets;
  const GlobalOrdinalType* Acols;
  const ScalarType*        Acoefs;

  const ScalarType* x;
        ScalarType* y;
  ScalarType beta;

  inline KERNEL_PREFIX void operator()(int row)
  {
    //we count on the caller (ComputeNode) to pass in 'row'
    //in range 0..n-1
  
    ScalarType sum = beta*y[row];

    for(LocalOrdinalType i=Arowoffsets[row]; i<Arowoffsets[row+1]; ++i) {
      sum += Acoefs[i]*x[Acols[i]];
    }

    y[row] = sum;
  }

};//struct MatvecOp

#endif

