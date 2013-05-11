; ModuleID = 'simplecase.cpp'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%"class.std::ios_base::Init" = type { i8 }
%"class.std::basic_ostream" = type { i32 (...)**, %"class.std::basic_ios" }
%"class.std::basic_ios" = type { %"class.std::ios_base", %"class.std::basic_ostream"*, i8, i8, %"class.std::basic_streambuf"*, %"class.std::ctype"*, %"class.std::num_put"*, %"class.std::num_get"* }
%"class.std::ios_base" = type { i32 (...)**, i64, i64, i32, i32, i32, %"struct.std::ios_base::_Callback_list"*, %"struct.std::ios_base::_Words", [8 x %"struct.std::ios_base::_Words"], i32, %"struct.std::ios_base::_Words"*, %"class.std::locale" }
%"struct.std::ios_base::_Callback_list" = type { %"struct.std::ios_base::_Callback_list"*, void (i32, %"class.std::ios_base"*, i32)*, i32, i32 }
%"struct.std::ios_base::_Words" = type { i8*, i64 }
%"class.std::locale" = type { %"class.std::locale::_Impl"* }
%"class.std::locale::_Impl" = type { i32, %"class.std::locale::facet"**, i64, %"class.std::locale::facet"**, i8** }
%"class.std::locale::facet" = type { i32 (...)**, i32 }
%"class.std::basic_streambuf" = type { i32 (...)**, i8*, i8*, i8*, i8*, i8*, i8*, %"class.std::locale" }
%"class.std::ctype" = type { %"class.std::locale::facet", %struct.__locale_struct*, i8, i32*, i32*, i16*, i8, [256 x i8], [256 x i8], i8 }
%struct.__locale_struct = type { [13 x %struct.__locale_data*], i16*, i32*, i32*, [13 x i8*] }
%struct.__locale_data = type opaque
%"class.std::num_put" = type { %"class.std::locale::facet" }
%"class.std::num_get" = type { %"class.std::locale::facet" }
%"class.std::vector" = type { %"struct.std::_Vector_base" }
%"struct.std::_Vector_base" = type { %"struct.std::_Vector_base<Y, std::allocator<Y> >::_Vector_impl" }
%"struct.std::_Vector_base<Y, std::allocator<Y> >::_Vector_impl" = type { %class.Y*, %class.Y*, %class.Y* }
%class.Y = type { i32, double }
%"class.__gnu_cxx::__normal_iterator" = type { %class.Y* }
%"class.std::allocator" = type { i8 }
%"class.__gnu_cxx::__normal_iterator.0" = type { %class.Y* }
%"class.__gnu_cxx::new_allocator" = type { i8 }

@_ZStL8__ioinit = internal global %"class.std::ios_base::Init" zeroinitializer, align 1
@__dso_handle = external global i8
@.str = private unnamed_addr constant [7 x i8] c"kernel\00", section "llvm.metadata"
@.str1 = private unnamed_addr constant [15 x i8] c"simplecase.cpp\00", section "llvm.metadata"
@_ZSt4cout = external global %"class.std::basic_ostream"
@.str2 = private unnamed_addr constant [8 x i8] c"Elems: \00", align 1
@.str3 = private unnamed_addr constant [9 x i8] c" Sizes: \00", align 1
@.str4 = private unnamed_addr constant [9 x i8] c"\0ATotal: \00", align 1
@.str5 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@llvm.global_ctors = appending global [1 x { i32, void ()* }] [{ i32, void ()* } { i32 65535, void ()* @_GLOBAL__I_a }]
@llvm.global.annotations = appending global [1 x { i8*, i8*, i8*, i32 }] [{ i8*, i8*, i8*, i32 } { i8* bitcast (void (%"class.std::vector"*)* @_Z3addSt6vectorI1YSaIS0_EE to i8*), i8* getelementptr inbounds ([7 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([15 x i8]* @.str1, i32 0, i32 0), i32 12 }], section "llvm.metadata"

define internal void @__cxx_global_var_init() nounwind section ".text.startup" {
entry:
  call void @_ZNSt8ios_base4InitC1Ev(%"class.std::ios_base::Init"* @_ZStL8__ioinit)
  %0 = call i32 @__cxa_atexit(void (i8*)* bitcast (void (%"class.std::ios_base::Init"*)* @_ZNSt8ios_base4InitD1Ev to void (i8*)*), i8* getelementptr inbounds (%"class.std::ios_base::Init"* @_ZStL8__ioinit, i32 0, i32 0), i8* @__dso_handle)
  ret void
}

declare void @_ZNSt8ios_base4InitC1Ev(%"class.std::ios_base::Init"*)

declare void @_ZNSt8ios_base4InitD1Ev(%"class.std::ios_base::Init"*)

declare i32 @__cxa_atexit(void (i8*)*, i8*, i8*) nounwind

define void @_Z3addSt6vectorI1YSaIS0_EE(%"class.std::vector"* %X) nounwind uwtable {
entry:
  %it = alloca %"class.__gnu_cxx::__normal_iterator", align 8
  %ref.tmp = alloca %"class.__gnu_cxx::__normal_iterator", align 8
  %call = call %class.Y* @_ZNSt6vectorI1YSaIS0_EE5beginEv(%"class.std::vector"* %X)
  %coerce.dive = getelementptr %"class.__gnu_cxx::__normal_iterator"* %it, i32 0, i32 0
  store %class.Y* %call, %class.Y** %coerce.dive
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %call1 = call %class.Y* @_ZNSt6vectorI1YSaIS0_EE3endEv(%"class.std::vector"* %X)
  %coerce.dive2 = getelementptr %"class.__gnu_cxx::__normal_iterator"* %ref.tmp, i32 0, i32 0
  store %class.Y* %call1, %class.Y** %coerce.dive2
  %call3 = call zeroext i1 @_ZN9__gnu_cxxneIP1YSt6vectorIS1_SaIS1_EEEEbRKNS_17__normal_iteratorIT_T0_EESB_(%"class.__gnu_cxx::__normal_iterator"* %it, %"class.__gnu_cxx::__normal_iterator"* %ref.tmp)
  br i1 %call3, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %call4 = call %class.Y* @_ZNK9__gnu_cxx17__normal_iteratorIP1YSt6vectorIS1_SaIS1_EEEptEv(%"class.__gnu_cxx::__normal_iterator"* %it)
  %a = getelementptr inbounds %class.Y* %call4, i32 0, i32 0
  store i32 1, i32* %a, align 4
  %call5 = call %class.Y* @_ZNK9__gnu_cxx17__normal_iteratorIP1YSt6vectorIS1_SaIS1_EEEptEv(%"class.__gnu_cxx::__normal_iterator"* %it)
  %b = getelementptr inbounds %class.Y* %call5, i32 0, i32 1
  store double 1.100000e+00, double* %b, align 8
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %call6 = call %"class.__gnu_cxx::__normal_iterator"* @_ZN9__gnu_cxx17__normal_iteratorIP1YSt6vectorIS1_SaIS1_EEEppEv(%"class.__gnu_cxx::__normal_iterator"* %it)
  br label %for.cond

for.end:                                          ; preds = %for.cond
  ret void
}

define linkonce_odr %class.Y* @_ZNSt6vectorI1YSaIS0_EE5beginEv(%"class.std::vector"* %this) nounwind uwtable align 2 {
entry:
  %retval = alloca %"class.__gnu_cxx::__normal_iterator", align 8
  %this.addr = alloca %"class.std::vector"*, align 8
  store %"class.std::vector"* %this, %"class.std::vector"** %this.addr, align 8
  %this1 = load %"class.std::vector"** %this.addr
  %0 = bitcast %"class.std::vector"* %this1 to %"struct.std::_Vector_base"*
  %_M_impl = getelementptr inbounds %"struct.std::_Vector_base"* %0, i32 0, i32 0
  %_M_start = getelementptr inbounds %"struct.std::_Vector_base<Y, std::allocator<Y> >::_Vector_impl"* %_M_impl, i32 0, i32 0
  call void @_ZN9__gnu_cxx17__normal_iteratorIP1YSt6vectorIS1_SaIS1_EEEC1ERKS2_(%"class.__gnu_cxx::__normal_iterator"* %retval, %class.Y** %_M_start)
  %coerce.dive = getelementptr %"class.__gnu_cxx::__normal_iterator"* %retval, i32 0, i32 0
  %1 = load %class.Y** %coerce.dive
  ret %class.Y* %1
}

define linkonce_odr zeroext i1 @_ZN9__gnu_cxxneIP1YSt6vectorIS1_SaIS1_EEEEbRKNS_17__normal_iteratorIT_T0_EESB_(%"class.__gnu_cxx::__normal_iterator"* %__lhs, %"class.__gnu_cxx::__normal_iterator"* %__rhs) nounwind uwtable inlinehint {
entry:
  %__lhs.addr = alloca %"class.__gnu_cxx::__normal_iterator"*, align 8
  %__rhs.addr = alloca %"class.__gnu_cxx::__normal_iterator"*, align 8
  store %"class.__gnu_cxx::__normal_iterator"* %__lhs, %"class.__gnu_cxx::__normal_iterator"** %__lhs.addr, align 8
  store %"class.__gnu_cxx::__normal_iterator"* %__rhs, %"class.__gnu_cxx::__normal_iterator"** %__rhs.addr, align 8
  %0 = load %"class.__gnu_cxx::__normal_iterator"** %__lhs.addr, align 8
  %call = call %class.Y** @_ZNK9__gnu_cxx17__normal_iteratorIP1YSt6vectorIS1_SaIS1_EEE4baseEv(%"class.__gnu_cxx::__normal_iterator"* %0)
  %1 = load %class.Y** %call
  %2 = load %"class.__gnu_cxx::__normal_iterator"** %__rhs.addr, align 8
  %call1 = call %class.Y** @_ZNK9__gnu_cxx17__normal_iteratorIP1YSt6vectorIS1_SaIS1_EEE4baseEv(%"class.__gnu_cxx::__normal_iterator"* %2)
  %3 = load %class.Y** %call1
  %cmp = icmp ne %class.Y* %1, %3
  ret i1 %cmp
}

define linkonce_odr %class.Y* @_ZNSt6vectorI1YSaIS0_EE3endEv(%"class.std::vector"* %this) nounwind uwtable align 2 {
entry:
  %retval = alloca %"class.__gnu_cxx::__normal_iterator", align 8
  %this.addr = alloca %"class.std::vector"*, align 8
  store %"class.std::vector"* %this, %"class.std::vector"** %this.addr, align 8
  %this1 = load %"class.std::vector"** %this.addr
  %0 = bitcast %"class.std::vector"* %this1 to %"struct.std::_Vector_base"*
  %_M_impl = getelementptr inbounds %"struct.std::_Vector_base"* %0, i32 0, i32 0
  %_M_finish = getelementptr inbounds %"struct.std::_Vector_base<Y, std::allocator<Y> >::_Vector_impl"* %_M_impl, i32 0, i32 1
  call void @_ZN9__gnu_cxx17__normal_iteratorIP1YSt6vectorIS1_SaIS1_EEEC1ERKS2_(%"class.__gnu_cxx::__normal_iterator"* %retval, %class.Y** %_M_finish)
  %coerce.dive = getelementptr %"class.__gnu_cxx::__normal_iterator"* %retval, i32 0, i32 0
  %1 = load %class.Y** %coerce.dive
  ret %class.Y* %1
}

define linkonce_odr %class.Y* @_ZNK9__gnu_cxx17__normal_iteratorIP1YSt6vectorIS1_SaIS1_EEEptEv(%"class.__gnu_cxx::__normal_iterator"* %this) nounwind uwtable align 2 {
entry:
  %this.addr = alloca %"class.__gnu_cxx::__normal_iterator"*, align 8
  store %"class.__gnu_cxx::__normal_iterator"* %this, %"class.__gnu_cxx::__normal_iterator"** %this.addr, align 8
  %this1 = load %"class.__gnu_cxx::__normal_iterator"** %this.addr
  %_M_current = getelementptr inbounds %"class.__gnu_cxx::__normal_iterator"* %this1, i32 0, i32 0
  %0 = load %class.Y** %_M_current, align 8
  ret %class.Y* %0
}

define linkonce_odr %"class.__gnu_cxx::__normal_iterator"* @_ZN9__gnu_cxx17__normal_iteratorIP1YSt6vectorIS1_SaIS1_EEEppEv(%"class.__gnu_cxx::__normal_iterator"* %this) nounwind uwtable align 2 {
entry:
  %this.addr = alloca %"class.__gnu_cxx::__normal_iterator"*, align 8
  store %"class.__gnu_cxx::__normal_iterator"* %this, %"class.__gnu_cxx::__normal_iterator"** %this.addr, align 8
  %this1 = load %"class.__gnu_cxx::__normal_iterator"** %this.addr
  %_M_current = getelementptr inbounds %"class.__gnu_cxx::__normal_iterator"* %this1, i32 0, i32 0
  %0 = load %class.Y** %_M_current, align 8
  %incdec.ptr = getelementptr inbounds %class.Y* %0, i32 1
  store %class.Y* %incdec.ptr, %class.Y** %_M_current, align 8
  ret %"class.__gnu_cxx::__normal_iterator"* %this1
}

define i32 @main() nounwind uwtable {
entry:
  %retval = alloca i32, align 4
  %X = alloca %"class.std::vector", align 8
  %ref.tmp = alloca %class.Y, align 8
  %ref.tmp1 = alloca %"class.std::allocator", align 1
  %agg.tmp = alloca %"class.std::vector", align 8
  %cleanup.dest.slot = alloca i32
  store i32 0, i32* %retval
  %0 = bitcast %class.Y* %ref.tmp to i8*
  call void @llvm.memset.p0i8.i64(i8* %0, i8 0, i64 16, i32 8, i1 false)
  call void @_ZNSaI1YEC1Ev(%"class.std::allocator"* %ref.tmp1) nounwind
  call void @_ZNSt6vectorI1YSaIS0_EEC1EmRKS0_RKS1_(%"class.std::vector"* %X, i64 10, %class.Y* %ref.tmp, %"class.std::allocator"* %ref.tmp1)
  call void @_ZNSaI1YED1Ev(%"class.std::allocator"* %ref.tmp1) nounwind
  %call = call %"class.std::basic_ostream"* @_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc(%"class.std::basic_ostream"* @_ZSt4cout, i8* getelementptr inbounds ([8 x i8]* @.str2, i32 0, i32 0))
  %call2 = call i64 @_ZNKSt6vectorI1YSaIS0_EE4sizeEv(%"class.std::vector"* %X)
  %call3 = call %"class.std::basic_ostream"* @_ZNSolsEm(%"class.std::basic_ostream"* %call, i64 %call2)
  %call4 = call %"class.std::basic_ostream"* @_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc(%"class.std::basic_ostream"* %call3, i8* getelementptr inbounds ([9 x i8]* @.str3, i32 0, i32 0))
  %call5 = call %"class.std::basic_ostream"* @_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc(%"class.std::basic_ostream"* @_ZSt4cout, i8* getelementptr inbounds ([9 x i8]* @.str4, i32 0, i32 0))
  %call6 = call i64 @_ZNKSt6vectorI1YSaIS0_EE4sizeEv(%"class.std::vector"* %X)
  %mul = mul i64 %call6, 12
  %call7 = call %"class.std::basic_ostream"* @_ZNSolsEm(%"class.std::basic_ostream"* %call5, i64 %mul)
  %call8 = call %"class.std::basic_ostream"* @_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc(%"class.std::basic_ostream"* %call7, i8* getelementptr inbounds ([2 x i8]* @.str5, i32 0, i32 0))
  call void @_ZNSt6vectorI1YSaIS0_EEC1ERKS2_(%"class.std::vector"* %agg.tmp, %"class.std::vector"* %X)
  call void @_Z3addSt6vectorI1YSaIS0_EE(%"class.std::vector"* %agg.tmp)
  call void @_ZNSt6vectorI1YSaIS0_EED1Ev(%"class.std::vector"* %agg.tmp)
  store i32 0, i32* %retval
  store i32 1, i32* %cleanup.dest.slot
  call void @_ZNSt6vectorI1YSaIS0_EED1Ev(%"class.std::vector"* %X)
  %1 = load i32* %retval
  ret i32 %1
}

define linkonce_odr void @_ZNSt6vectorI1YSaIS0_EEC1EmRKS0_RKS1_(%"class.std::vector"* %this, i64 %__n, %class.Y* %__value, %"class.std::allocator"* %__a) unnamed_addr nounwind uwtable align 2 {
entry:
  %this.addr = alloca %"class.std::vector"*, align 8
  %__n.addr = alloca i64, align 8
  %__value.addr = alloca %class.Y*, align 8
  %__a.addr = alloca %"class.std::allocator"*, align 8
  store %"class.std::vector"* %this, %"class.std::vector"** %this.addr, align 8
  store i64 %__n, i64* %__n.addr, align 8
  store %class.Y* %__value, %class.Y** %__value.addr, align 8
  store %"class.std::allocator"* %__a, %"class.std::allocator"** %__a.addr, align 8
  %this1 = load %"class.std::vector"** %this.addr
  %0 = load i64* %__n.addr, align 8
  %1 = load %class.Y** %__value.addr
  %2 = load %"class.std::allocator"** %__a.addr
  call void @_ZNSt6vectorI1YSaIS0_EEC2EmRKS0_RKS1_(%"class.std::vector"* %this1, i64 %0, %class.Y* %1, %"class.std::allocator"* %2)
  ret void
}

declare void @llvm.memset.p0i8.i64(i8* nocapture, i8, i64, i32, i1) nounwind

define linkonce_odr void @_ZNSaI1YEC1Ev(%"class.std::allocator"* %this) unnamed_addr nounwind uwtable align 2 {
entry:
  %this.addr = alloca %"class.std::allocator"*, align 8
  store %"class.std::allocator"* %this, %"class.std::allocator"** %this.addr, align 8
  %this1 = load %"class.std::allocator"** %this.addr
  call void @_ZNSaI1YEC2Ev(%"class.std::allocator"* %this1) nounwind
  ret void
}

define linkonce_odr void @_ZNSaI1YED1Ev(%"class.std::allocator"* %this) unnamed_addr nounwind uwtable align 2 {
entry:
  %this.addr = alloca %"class.std::allocator"*, align 8
  store %"class.std::allocator"* %this, %"class.std::allocator"** %this.addr, align 8
  %this1 = load %"class.std::allocator"** %this.addr
  call void @_ZNSaI1YED2Ev(%"class.std::allocator"* %this1) nounwind
  ret void
}

declare %"class.std::basic_ostream"* @_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc(%"class.std::basic_ostream"*, i8*)

declare %"class.std::basic_ostream"* @_ZNSolsEm(%"class.std::basic_ostream"*, i64)

define linkonce_odr i64 @_ZNKSt6vectorI1YSaIS0_EE4sizeEv(%"class.std::vector"* %this) nounwind uwtable align 2 {
entry:
  %this.addr = alloca %"class.std::vector"*, align 8
  store %"class.std::vector"* %this, %"class.std::vector"** %this.addr, align 8
  %this1 = load %"class.std::vector"** %this.addr
  %0 = bitcast %"class.std::vector"* %this1 to %"struct.std::_Vector_base"*
  %_M_impl = getelementptr inbounds %"struct.std::_Vector_base"* %0, i32 0, i32 0
  %_M_finish = getelementptr inbounds %"struct.std::_Vector_base<Y, std::allocator<Y> >::_Vector_impl"* %_M_impl, i32 0, i32 1
  %1 = load %class.Y** %_M_finish, align 8
  %2 = bitcast %"class.std::vector"* %this1 to %"struct.std::_Vector_base"*
  %_M_impl2 = getelementptr inbounds %"struct.std::_Vector_base"* %2, i32 0, i32 0
  %_M_start = getelementptr inbounds %"struct.std::_Vector_base<Y, std::allocator<Y> >::_Vector_impl"* %_M_impl2, i32 0, i32 0
  %3 = load %class.Y** %_M_start, align 8
  %sub.ptr.lhs.cast = ptrtoint %class.Y* %1 to i64
  %sub.ptr.rhs.cast = ptrtoint %class.Y* %3 to i64
  %sub.ptr.sub = sub i64 %sub.ptr.lhs.cast, %sub.ptr.rhs.cast
  %sub.ptr.div = sdiv exact i64 %sub.ptr.sub, 16
  ret i64 %sub.ptr.div
}

define linkonce_odr void @_ZNSt6vectorI1YSaIS0_EEC1ERKS2_(%"class.std::vector"* %this, %"class.std::vector"* %__x) unnamed_addr nounwind uwtable align 2 {
entry:
  %this.addr = alloca %"class.std::vector"*, align 8
  %__x.addr = alloca %"class.std::vector"*, align 8
  store %"class.std::vector"* %this, %"class.std::vector"** %this.addr, align 8
  store %"class.std::vector"* %__x, %"class.std::vector"** %__x.addr, align 8
  %this1 = load %"class.std::vector"** %this.addr
  %0 = load %"class.std::vector"** %__x.addr
  call void @_ZNSt6vectorI1YSaIS0_EEC2ERKS2_(%"class.std::vector"* %this1, %"class.std::vector"* %0)
  ret void
}

define linkonce_odr void @_ZNSt6vectorI1YSaIS0_EED1Ev(%"class.std::vector"* %this) unnamed_addr nounwind uwtable align 2 {
entry:
  %this.addr = alloca %"class.std::vector"*, align 8
  store %"class.std::vector"* %this, %"class.std::vector"** %this.addr, align 8
  %this1 = load %"class.std::vector"** %this.addr
  call void @_ZNSt6vectorI1YSaIS0_EED2Ev(%"class.std::vector"* %this1)
  ret void
}

define linkonce_odr void @_ZNSt6vectorI1YSaIS0_EEC2ERKS2_(%"class.std::vector"* %this, %"class.std::vector"* %__x) unnamed_addr nounwind uwtable align 2 {
entry:
  %this.addr = alloca %"class.std::vector"*, align 8
  %__x.addr = alloca %"class.std::vector"*, align 8
  %agg.tmp = alloca %"class.__gnu_cxx::__normal_iterator.0", align 8
  %agg.tmp5 = alloca %"class.__gnu_cxx::__normal_iterator.0", align 8
  store %"class.std::vector"* %this, %"class.std::vector"** %this.addr, align 8
  store %"class.std::vector"* %__x, %"class.std::vector"** %__x.addr, align 8
  %this1 = load %"class.std::vector"** %this.addr
  %0 = bitcast %"class.std::vector"* %this1 to %"struct.std::_Vector_base"*
  %1 = load %"class.std::vector"** %__x.addr, align 8
  %call = call i64 @_ZNKSt6vectorI1YSaIS0_EE4sizeEv(%"class.std::vector"* %1)
  %2 = load %"class.std::vector"** %__x.addr, align 8
  %3 = bitcast %"class.std::vector"* %2 to %"struct.std::_Vector_base"*
  %call2 = call %"class.std::allocator"* @_ZNKSt12_Vector_baseI1YSaIS0_EE19_M_get_Tp_allocatorEv(%"struct.std::_Vector_base"* %3)
  %call3 = call %"class.std::allocator"* @_ZN9__gnu_cxx14__alloc_traitsISaI1YEE17_S_select_on_copyERKS2_(%"class.std::allocator"* %call2)
  call void @_ZNSt12_Vector_baseI1YSaIS0_EEC2EmRKS1_(%"struct.std::_Vector_base"* %0, i64 %call, %"class.std::allocator"* %call3)
  %4 = load %"class.std::vector"** %__x.addr, align 8
  %call4 = call %class.Y* @_ZNKSt6vectorI1YSaIS0_EE5beginEv(%"class.std::vector"* %4)
  %coerce.dive = getelementptr %"class.__gnu_cxx::__normal_iterator.0"* %agg.tmp, i32 0, i32 0
  store %class.Y* %call4, %class.Y** %coerce.dive
  %5 = load %"class.std::vector"** %__x.addr, align 8
  %call6 = call %class.Y* @_ZNKSt6vectorI1YSaIS0_EE3endEv(%"class.std::vector"* %5)
  %coerce.dive7 = getelementptr %"class.__gnu_cxx::__normal_iterator.0"* %agg.tmp5, i32 0, i32 0
  store %class.Y* %call6, %class.Y** %coerce.dive7
  %6 = bitcast %"class.std::vector"* %this1 to %"struct.std::_Vector_base"*
  %_M_impl = getelementptr inbounds %"struct.std::_Vector_base"* %6, i32 0, i32 0
  %_M_start = getelementptr inbounds %"struct.std::_Vector_base<Y, std::allocator<Y> >::_Vector_impl"* %_M_impl, i32 0, i32 0
  %7 = load %class.Y** %_M_start, align 8
  %8 = bitcast %"class.std::vector"* %this1 to %"struct.std::_Vector_base"*
  %call8 = call %"class.std::allocator"* @_ZNSt12_Vector_baseI1YSaIS0_EE19_M_get_Tp_allocatorEv(%"struct.std::_Vector_base"* %8)
  %coerce.dive9 = getelementptr %"class.__gnu_cxx::__normal_iterator.0"* %agg.tmp, i32 0, i32 0
  %9 = load %class.Y** %coerce.dive9
  %coerce.dive10 = getelementptr %"class.__gnu_cxx::__normal_iterator.0"* %agg.tmp5, i32 0, i32 0
  %10 = load %class.Y** %coerce.dive10
  %call11 = call %class.Y* @_ZSt22__uninitialized_copy_aIN9__gnu_cxx17__normal_iteratorIPK1YSt6vectorIS2_SaIS2_EEEEPS2_S2_ET0_T_SB_SA_RSaIT1_E(%class.Y* %9, %class.Y* %10, %class.Y* %7, %"class.std::allocator"* %call8)
  %11 = bitcast %"class.std::vector"* %this1 to %"struct.std::_Vector_base"*
  %_M_impl12 = getelementptr inbounds %"struct.std::_Vector_base"* %11, i32 0, i32 0
  %_M_finish = getelementptr inbounds %"struct.std::_Vector_base<Y, std::allocator<Y> >::_Vector_impl"* %_M_impl12, i32 0, i32 1
  store %class.Y* %call11, %class.Y** %_M_finish, align 8
  ret void
}

define linkonce_odr void @_ZNSt12_Vector_baseI1YSaIS0_EEC2EmRKS1_(%"struct.std::_Vector_base"* %this, i64 %__n, %"class.std::allocator"* %__a) unnamed_addr nounwind uwtable align 2 {
entry:
  %this.addr = alloca %"struct.std::_Vector_base"*, align 8
  %__n.addr = alloca i64, align 8
  %__a.addr = alloca %"class.std::allocator"*, align 8
  store %"struct.std::_Vector_base"* %this, %"struct.std::_Vector_base"** %this.addr, align 8
  store i64 %__n, i64* %__n.addr, align 8
  store %"class.std::allocator"* %__a, %"class.std::allocator"** %__a.addr, align 8
  %this1 = load %"struct.std::_Vector_base"** %this.addr
  %_M_impl = getelementptr inbounds %"struct.std::_Vector_base"* %this1, i32 0, i32 0
  %0 = load %"class.std::allocator"** %__a.addr, align 8
  call void @_ZNSt12_Vector_baseI1YSaIS0_EE12_Vector_implC1ERKS1_(%"struct.std::_Vector_base<Y, std::allocator<Y> >::_Vector_impl"* %_M_impl, %"class.std::allocator"* %0)
  %1 = load i64* %__n.addr, align 8
  call void @_ZNSt12_Vector_baseI1YSaIS0_EE17_M_create_storageEm(%"struct.std::_Vector_base"* %this1, i64 %1)
  ret void
}

define linkonce_odr %"class.std::allocator"* @_ZN9__gnu_cxx14__alloc_traitsISaI1YEE17_S_select_on_copyERKS2_(%"class.std::allocator"* %__a) nounwind uwtable align 2 {
entry:
  %__a.addr = alloca %"class.std::allocator"*, align 8
  store %"class.std::allocator"* %__a, %"class.std::allocator"** %__a.addr, align 8
  %0 = load %"class.std::allocator"** %__a.addr, align 8
  ret %"class.std::allocator"* %0
}

define linkonce_odr %"class.std::allocator"* @_ZNKSt12_Vector_baseI1YSaIS0_EE19_M_get_Tp_allocatorEv(%"struct.std::_Vector_base"* %this) nounwind uwtable align 2 {
entry:
  %this.addr = alloca %"struct.std::_Vector_base"*, align 8
  store %"struct.std::_Vector_base"* %this, %"struct.std::_Vector_base"** %this.addr, align 8
  %this1 = load %"struct.std::_Vector_base"** %this.addr
  %_M_impl = getelementptr inbounds %"struct.std::_Vector_base"* %this1, i32 0, i32 0
  %0 = bitcast %"struct.std::_Vector_base<Y, std::allocator<Y> >::_Vector_impl"* %_M_impl to %"class.std::allocator"*
  ret %"class.std::allocator"* %0
}

define linkonce_odr %class.Y* @_ZSt22__uninitialized_copy_aIN9__gnu_cxx17__normal_iteratorIPK1YSt6vectorIS2_SaIS2_EEEEPS2_S2_ET0_T_SB_SA_RSaIT1_E(%class.Y* %__first.coerce, %class.Y* %__last.coerce, %class.Y* %__result, %"class.std::allocator"*) nounwind uwtable inlinehint {
entry:
  %__first = alloca %"class.__gnu_cxx::__normal_iterator.0", align 8
  %__last = alloca %"class.__gnu_cxx::__normal_iterator.0", align 8
  %__result.addr = alloca %class.Y*, align 8
  %.addr = alloca %"class.std::allocator"*, align 8
  %agg.tmp = alloca %"class.__gnu_cxx::__normal_iterator.0", align 8
  %agg.tmp2 = alloca %"class.__gnu_cxx::__normal_iterator.0", align 8
  %coerce.dive = getelementptr %"class.__gnu_cxx::__normal_iterator.0"* %__first, i32 0, i32 0
  store %class.Y* %__first.coerce, %class.Y** %coerce.dive
  %coerce.dive1 = getelementptr %"class.__gnu_cxx::__normal_iterator.0"* %__last, i32 0, i32 0
  store %class.Y* %__last.coerce, %class.Y** %coerce.dive1
  store %class.Y* %__result, %class.Y** %__result.addr, align 8
  store %"class.std::allocator"* %0, %"class.std::allocator"** %.addr, align 8
  %1 = bitcast %"class.__gnu_cxx::__normal_iterator.0"* %agg.tmp to i8*
  %2 = bitcast %"class.__gnu_cxx::__normal_iterator.0"* %__first to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %1, i8* %2, i64 8, i32 8, i1 false)
  %3 = bitcast %"class.__gnu_cxx::__normal_iterator.0"* %agg.tmp2 to i8*
  %4 = bitcast %"class.__gnu_cxx::__normal_iterator.0"* %__last to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %3, i8* %4, i64 8, i32 8, i1 false)
  %5 = load %class.Y** %__result.addr, align 8
  %coerce.dive3 = getelementptr %"class.__gnu_cxx::__normal_iterator.0"* %agg.tmp, i32 0, i32 0
  %6 = load %class.Y** %coerce.dive3
  %coerce.dive4 = getelementptr %"class.__gnu_cxx::__normal_iterator.0"* %agg.tmp2, i32 0, i32 0
  %7 = load %class.Y** %coerce.dive4
  %call = call %class.Y* @_ZSt18uninitialized_copyIN9__gnu_cxx17__normal_iteratorIPK1YSt6vectorIS2_SaIS2_EEEEPS2_ET0_T_SB_SA_(%class.Y* %6, %class.Y* %7, %class.Y* %5)
  ret %class.Y* %call
}

define linkonce_odr %class.Y* @_ZNKSt6vectorI1YSaIS0_EE5beginEv(%"class.std::vector"* %this) nounwind uwtable align 2 {
entry:
  %retval = alloca %"class.__gnu_cxx::__normal_iterator.0", align 8
  %this.addr = alloca %"class.std::vector"*, align 8
  %ref.tmp = alloca %class.Y*, align 8
  store %"class.std::vector"* %this, %"class.std::vector"** %this.addr, align 8
  %this1 = load %"class.std::vector"** %this.addr
  %0 = bitcast %"class.std::vector"* %this1 to %"struct.std::_Vector_base"*
  %_M_impl = getelementptr inbounds %"struct.std::_Vector_base"* %0, i32 0, i32 0
  %_M_start = getelementptr inbounds %"struct.std::_Vector_base<Y, std::allocator<Y> >::_Vector_impl"* %_M_impl, i32 0, i32 0
  %1 = load %class.Y** %_M_start, align 8
  store %class.Y* %1, %class.Y** %ref.tmp, align 8
  call void @_ZN9__gnu_cxx17__normal_iteratorIPK1YSt6vectorIS1_SaIS1_EEEC1ERKS3_(%"class.__gnu_cxx::__normal_iterator.0"* %retval, %class.Y** %ref.tmp)
  %coerce.dive = getelementptr %"class.__gnu_cxx::__normal_iterator.0"* %retval, i32 0, i32 0
  %2 = load %class.Y** %coerce.dive
  ret %class.Y* %2
}

define linkonce_odr %class.Y* @_ZNKSt6vectorI1YSaIS0_EE3endEv(%"class.std::vector"* %this) nounwind uwtable align 2 {
entry:
  %retval = alloca %"class.__gnu_cxx::__normal_iterator.0", align 8
  %this.addr = alloca %"class.std::vector"*, align 8
  %ref.tmp = alloca %class.Y*, align 8
  store %"class.std::vector"* %this, %"class.std::vector"** %this.addr, align 8
  %this1 = load %"class.std::vector"** %this.addr
  %0 = bitcast %"class.std::vector"* %this1 to %"struct.std::_Vector_base"*
  %_M_impl = getelementptr inbounds %"struct.std::_Vector_base"* %0, i32 0, i32 0
  %_M_finish = getelementptr inbounds %"struct.std::_Vector_base<Y, std::allocator<Y> >::_Vector_impl"* %_M_impl, i32 0, i32 1
  %1 = load %class.Y** %_M_finish, align 8
  store %class.Y* %1, %class.Y** %ref.tmp, align 8
  call void @_ZN9__gnu_cxx17__normal_iteratorIPK1YSt6vectorIS1_SaIS1_EEEC1ERKS3_(%"class.__gnu_cxx::__normal_iterator.0"* %retval, %class.Y** %ref.tmp)
  %coerce.dive = getelementptr %"class.__gnu_cxx::__normal_iterator.0"* %retval, i32 0, i32 0
  %2 = load %class.Y** %coerce.dive
  ret %class.Y* %2
}

define linkonce_odr %"class.std::allocator"* @_ZNSt12_Vector_baseI1YSaIS0_EE19_M_get_Tp_allocatorEv(%"struct.std::_Vector_base"* %this) nounwind uwtable align 2 {
entry:
  %this.addr = alloca %"struct.std::_Vector_base"*, align 8
  store %"struct.std::_Vector_base"* %this, %"struct.std::_Vector_base"** %this.addr, align 8
  %this1 = load %"struct.std::_Vector_base"** %this.addr
  %_M_impl = getelementptr inbounds %"struct.std::_Vector_base"* %this1, i32 0, i32 0
  %0 = bitcast %"struct.std::_Vector_base<Y, std::allocator<Y> >::_Vector_impl"* %_M_impl to %"class.std::allocator"*
  ret %"class.std::allocator"* %0
}

define linkonce_odr void @_ZN9__gnu_cxx17__normal_iteratorIPK1YSt6vectorIS1_SaIS1_EEEC1ERKS3_(%"class.__gnu_cxx::__normal_iterator.0"* %this, %class.Y** %__i) unnamed_addr nounwind uwtable align 2 {
entry:
  %this.addr = alloca %"class.__gnu_cxx::__normal_iterator.0"*, align 8
  %__i.addr = alloca %class.Y**, align 8
  store %"class.__gnu_cxx::__normal_iterator.0"* %this, %"class.__gnu_cxx::__normal_iterator.0"** %this.addr, align 8
  store %class.Y** %__i, %class.Y*** %__i.addr, align 8
  %this1 = load %"class.__gnu_cxx::__normal_iterator.0"** %this.addr
  %0 = load %class.Y*** %__i.addr
  call void @_ZN9__gnu_cxx17__normal_iteratorIPK1YSt6vectorIS1_SaIS1_EEEC2ERKS3_(%"class.__gnu_cxx::__normal_iterator.0"* %this1, %class.Y** %0)
  ret void
}

define linkonce_odr void @_ZN9__gnu_cxx17__normal_iteratorIPK1YSt6vectorIS1_SaIS1_EEEC2ERKS3_(%"class.__gnu_cxx::__normal_iterator.0"* %this, %class.Y** %__i) unnamed_addr nounwind uwtable align 2 {
entry:
  %this.addr = alloca %"class.__gnu_cxx::__normal_iterator.0"*, align 8
  %__i.addr = alloca %class.Y**, align 8
  store %"class.__gnu_cxx::__normal_iterator.0"* %this, %"class.__gnu_cxx::__normal_iterator.0"** %this.addr, align 8
  store %class.Y** %__i, %class.Y*** %__i.addr, align 8
  %this1 = load %"class.__gnu_cxx::__normal_iterator.0"** %this.addr
  %_M_current = getelementptr inbounds %"class.__gnu_cxx::__normal_iterator.0"* %this1, i32 0, i32 0
  %0 = load %class.Y*** %__i.addr, align 8
  %1 = load %class.Y** %0, align 8
  store %class.Y* %1, %class.Y** %_M_current, align 8
  ret void
}

define linkonce_odr %class.Y* @_ZSt18uninitialized_copyIN9__gnu_cxx17__normal_iteratorIPK1YSt6vectorIS2_SaIS2_EEEEPS2_ET0_T_SB_SA_(%class.Y* %__first.coerce, %class.Y* %__last.coerce, %class.Y* %__result) nounwind uwtable inlinehint {
entry:
  %__first = alloca %"class.__gnu_cxx::__normal_iterator.0", align 8
  %__last = alloca %"class.__gnu_cxx::__normal_iterator.0", align 8
  %__result.addr = alloca %class.Y*, align 8
  %agg.tmp = alloca %"class.__gnu_cxx::__normal_iterator.0", align 8
  %agg.tmp2 = alloca %"class.__gnu_cxx::__normal_iterator.0", align 8
  %coerce.dive = getelementptr %"class.__gnu_cxx::__normal_iterator.0"* %__first, i32 0, i32 0
  store %class.Y* %__first.coerce, %class.Y** %coerce.dive
  %coerce.dive1 = getelementptr %"class.__gnu_cxx::__normal_iterator.0"* %__last, i32 0, i32 0
  store %class.Y* %__last.coerce, %class.Y** %coerce.dive1
  store %class.Y* %__result, %class.Y** %__result.addr, align 8
  %0 = bitcast %"class.__gnu_cxx::__normal_iterator.0"* %agg.tmp to i8*
  %1 = bitcast %"class.__gnu_cxx::__normal_iterator.0"* %__first to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %0, i8* %1, i64 8, i32 8, i1 false)
  %2 = bitcast %"class.__gnu_cxx::__normal_iterator.0"* %agg.tmp2 to i8*
  %3 = bitcast %"class.__gnu_cxx::__normal_iterator.0"* %__last to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %2, i8* %3, i64 8, i32 8, i1 false)
  %4 = load %class.Y** %__result.addr, align 8
  %coerce.dive3 = getelementptr %"class.__gnu_cxx::__normal_iterator.0"* %agg.tmp, i32 0, i32 0
  %5 = load %class.Y** %coerce.dive3
  %coerce.dive4 = getelementptr %"class.__gnu_cxx::__normal_iterator.0"* %agg.tmp2, i32 0, i32 0
  %6 = load %class.Y** %coerce.dive4
  %call = call %class.Y* @_ZNSt20__uninitialized_copyILb1EE13__uninit_copyIN9__gnu_cxx17__normal_iteratorIPK1YSt6vectorIS4_SaIS4_EEEEPS4_EET0_T_SD_SC_(%class.Y* %5, %class.Y* %6, %class.Y* %4)
  ret %class.Y* %call
}

declare void @llvm.memcpy.p0i8.p0i8.i64(i8* nocapture, i8* nocapture, i64, i32, i1) nounwind

define linkonce_odr %class.Y* @_ZNSt20__uninitialized_copyILb1EE13__uninit_copyIN9__gnu_cxx17__normal_iteratorIPK1YSt6vectorIS4_SaIS4_EEEEPS4_EET0_T_SD_SC_(%class.Y* %__first.coerce, %class.Y* %__last.coerce, %class.Y* %__result) nounwind uwtable align 2 {
entry:
  %__first = alloca %"class.__gnu_cxx::__normal_iterator.0", align 8
  %__last = alloca %"class.__gnu_cxx::__normal_iterator.0", align 8
  %__result.addr = alloca %class.Y*, align 8
  %agg.tmp = alloca %"class.__gnu_cxx::__normal_iterator.0", align 8
  %agg.tmp2 = alloca %"class.__gnu_cxx::__normal_iterator.0", align 8
  %coerce.dive = getelementptr %"class.__gnu_cxx::__normal_iterator.0"* %__first, i32 0, i32 0
  store %class.Y* %__first.coerce, %class.Y** %coerce.dive
  %coerce.dive1 = getelementptr %"class.__gnu_cxx::__normal_iterator.0"* %__last, i32 0, i32 0
  store %class.Y* %__last.coerce, %class.Y** %coerce.dive1
  store %class.Y* %__result, %class.Y** %__result.addr, align 8
  %0 = bitcast %"class.__gnu_cxx::__normal_iterator.0"* %agg.tmp to i8*
  %1 = bitcast %"class.__gnu_cxx::__normal_iterator.0"* %__first to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %0, i8* %1, i64 8, i32 8, i1 false)
  %2 = bitcast %"class.__gnu_cxx::__normal_iterator.0"* %agg.tmp2 to i8*
  %3 = bitcast %"class.__gnu_cxx::__normal_iterator.0"* %__last to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %2, i8* %3, i64 8, i32 8, i1 false)
  %4 = load %class.Y** %__result.addr, align 8
  %coerce.dive3 = getelementptr %"class.__gnu_cxx::__normal_iterator.0"* %agg.tmp, i32 0, i32 0
  %5 = load %class.Y** %coerce.dive3
  %coerce.dive4 = getelementptr %"class.__gnu_cxx::__normal_iterator.0"* %agg.tmp2, i32 0, i32 0
  %6 = load %class.Y** %coerce.dive4
  %call = call %class.Y* @_ZSt4copyIN9__gnu_cxx17__normal_iteratorIPK1YSt6vectorIS2_SaIS2_EEEEPS2_ET0_T_SB_SA_(%class.Y* %5, %class.Y* %6, %class.Y* %4)
  ret %class.Y* %call
}

define linkonce_odr %class.Y* @_ZSt4copyIN9__gnu_cxx17__normal_iteratorIPK1YSt6vectorIS2_SaIS2_EEEEPS2_ET0_T_SB_SA_(%class.Y* %__first.coerce, %class.Y* %__last.coerce, %class.Y* %__result) nounwind uwtable inlinehint {
entry:
  %__first = alloca %"class.__gnu_cxx::__normal_iterator.0", align 8
  %__last = alloca %"class.__gnu_cxx::__normal_iterator.0", align 8
  %__result.addr = alloca %class.Y*, align 8
  %agg.tmp = alloca %"class.__gnu_cxx::__normal_iterator.0", align 8
  %agg.tmp2 = alloca %"class.__gnu_cxx::__normal_iterator.0", align 8
  %agg.tmp5 = alloca %"class.__gnu_cxx::__normal_iterator.0", align 8
  %agg.tmp6 = alloca %"class.__gnu_cxx::__normal_iterator.0", align 8
  %coerce.dive = getelementptr %"class.__gnu_cxx::__normal_iterator.0"* %__first, i32 0, i32 0
  store %class.Y* %__first.coerce, %class.Y** %coerce.dive
  %coerce.dive1 = getelementptr %"class.__gnu_cxx::__normal_iterator.0"* %__last, i32 0, i32 0
  store %class.Y* %__last.coerce, %class.Y** %coerce.dive1
  store %class.Y* %__result, %class.Y** %__result.addr, align 8
  %0 = bitcast %"class.__gnu_cxx::__normal_iterator.0"* %agg.tmp2 to i8*
  %1 = bitcast %"class.__gnu_cxx::__normal_iterator.0"* %__first to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %0, i8* %1, i64 8, i32 8, i1 false)
  %coerce.dive3 = getelementptr %"class.__gnu_cxx::__normal_iterator.0"* %agg.tmp2, i32 0, i32 0
  %2 = load %class.Y** %coerce.dive3
  %call = call %class.Y* @_ZSt12__miter_baseIN9__gnu_cxx17__normal_iteratorIPK1YSt6vectorIS2_SaIS2_EEEEENSt11_Miter_baseIT_E13iterator_typeESA_(%class.Y* %2)
  %coerce.dive4 = getelementptr %"class.__gnu_cxx::__normal_iterator.0"* %agg.tmp, i32 0, i32 0
  store %class.Y* %call, %class.Y** %coerce.dive4
  %3 = bitcast %"class.__gnu_cxx::__normal_iterator.0"* %agg.tmp6 to i8*
  %4 = bitcast %"class.__gnu_cxx::__normal_iterator.0"* %__last to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %3, i8* %4, i64 8, i32 8, i1 false)
  %coerce.dive7 = getelementptr %"class.__gnu_cxx::__normal_iterator.0"* %agg.tmp6, i32 0, i32 0
  %5 = load %class.Y** %coerce.dive7
  %call8 = call %class.Y* @_ZSt12__miter_baseIN9__gnu_cxx17__normal_iteratorIPK1YSt6vectorIS2_SaIS2_EEEEENSt11_Miter_baseIT_E13iterator_typeESA_(%class.Y* %5)
  %coerce.dive9 = getelementptr %"class.__gnu_cxx::__normal_iterator.0"* %agg.tmp5, i32 0, i32 0
  store %class.Y* %call8, %class.Y** %coerce.dive9
  %6 = load %class.Y** %__result.addr, align 8
  %coerce.dive10 = getelementptr %"class.__gnu_cxx::__normal_iterator.0"* %agg.tmp, i32 0, i32 0
  %7 = load %class.Y** %coerce.dive10
  %coerce.dive11 = getelementptr %"class.__gnu_cxx::__normal_iterator.0"* %agg.tmp5, i32 0, i32 0
  %8 = load %class.Y** %coerce.dive11
  %call12 = call %class.Y* @_ZSt14__copy_move_a2ILb0EN9__gnu_cxx17__normal_iteratorIPK1YSt6vectorIS2_SaIS2_EEEEPS2_ET1_T0_SB_SA_(%class.Y* %7, %class.Y* %8, %class.Y* %6)
  ret %class.Y* %call12
}

define linkonce_odr %class.Y* @_ZSt14__copy_move_a2ILb0EN9__gnu_cxx17__normal_iteratorIPK1YSt6vectorIS2_SaIS2_EEEEPS2_ET1_T0_SB_SA_(%class.Y* %__first.coerce, %class.Y* %__last.coerce, %class.Y* %__result) nounwind uwtable inlinehint {
entry:
  %__first = alloca %"class.__gnu_cxx::__normal_iterator.0", align 8
  %__last = alloca %"class.__gnu_cxx::__normal_iterator.0", align 8
  %__result.addr = alloca %class.Y*, align 8
  %agg.tmp = alloca %"class.__gnu_cxx::__normal_iterator.0", align 8
  %agg.tmp3 = alloca %"class.__gnu_cxx::__normal_iterator.0", align 8
  %coerce.dive = getelementptr %"class.__gnu_cxx::__normal_iterator.0"* %__first, i32 0, i32 0
  store %class.Y* %__first.coerce, %class.Y** %coerce.dive
  %coerce.dive1 = getelementptr %"class.__gnu_cxx::__normal_iterator.0"* %__last, i32 0, i32 0
  store %class.Y* %__last.coerce, %class.Y** %coerce.dive1
  store %class.Y* %__result, %class.Y** %__result.addr, align 8
  %0 = bitcast %"class.__gnu_cxx::__normal_iterator.0"* %agg.tmp to i8*
  %1 = bitcast %"class.__gnu_cxx::__normal_iterator.0"* %__first to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %0, i8* %1, i64 8, i32 8, i1 false)
  %coerce.dive2 = getelementptr %"class.__gnu_cxx::__normal_iterator.0"* %agg.tmp, i32 0, i32 0
  %2 = load %class.Y** %coerce.dive2
  %call = call %class.Y* @_ZSt12__niter_baseIN9__gnu_cxx17__normal_iteratorIPK1YSt6vectorIS2_SaIS2_EEEEENSt11_Niter_baseIT_E13iterator_typeESA_(%class.Y* %2)
  %3 = bitcast %"class.__gnu_cxx::__normal_iterator.0"* %agg.tmp3 to i8*
  %4 = bitcast %"class.__gnu_cxx::__normal_iterator.0"* %__last to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %3, i8* %4, i64 8, i32 8, i1 false)
  %coerce.dive4 = getelementptr %"class.__gnu_cxx::__normal_iterator.0"* %agg.tmp3, i32 0, i32 0
  %5 = load %class.Y** %coerce.dive4
  %call5 = call %class.Y* @_ZSt12__niter_baseIN9__gnu_cxx17__normal_iteratorIPK1YSt6vectorIS2_SaIS2_EEEEENSt11_Niter_baseIT_E13iterator_typeESA_(%class.Y* %5)
  %6 = load %class.Y** %__result.addr, align 8
  %call6 = call %class.Y* @_ZSt12__niter_baseIP1YENSt11_Niter_baseIT_E13iterator_typeES3_(%class.Y* %6)
  %call7 = call %class.Y* @_ZSt13__copy_move_aILb0EPK1YPS0_ET1_T0_S5_S4_(%class.Y* %call, %class.Y* %call5, %class.Y* %call6)
  ret %class.Y* %call7
}

define linkonce_odr %class.Y* @_ZSt12__miter_baseIN9__gnu_cxx17__normal_iteratorIPK1YSt6vectorIS2_SaIS2_EEEEENSt11_Miter_baseIT_E13iterator_typeESA_(%class.Y* %__it.coerce) nounwind uwtable inlinehint {
entry:
  %retval = alloca %"class.__gnu_cxx::__normal_iterator.0", align 8
  %__it = alloca %"class.__gnu_cxx::__normal_iterator.0", align 8
  %agg.tmp = alloca %"class.__gnu_cxx::__normal_iterator.0", align 8
  %coerce.dive = getelementptr %"class.__gnu_cxx::__normal_iterator.0"* %__it, i32 0, i32 0
  store %class.Y* %__it.coerce, %class.Y** %coerce.dive
  %0 = bitcast %"class.__gnu_cxx::__normal_iterator.0"* %agg.tmp to i8*
  %1 = bitcast %"class.__gnu_cxx::__normal_iterator.0"* %__it to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %0, i8* %1, i64 8, i32 8, i1 false)
  %coerce.dive1 = getelementptr %"class.__gnu_cxx::__normal_iterator.0"* %agg.tmp, i32 0, i32 0
  %2 = load %class.Y** %coerce.dive1
  %call = call %class.Y* @_ZNSt10_Iter_baseIN9__gnu_cxx17__normal_iteratorIPK1YSt6vectorIS2_SaIS2_EEEELb0EE7_S_baseES8_(%class.Y* %2)
  %coerce.dive2 = getelementptr %"class.__gnu_cxx::__normal_iterator.0"* %retval, i32 0, i32 0
  store %class.Y* %call, %class.Y** %coerce.dive2
  %coerce.dive3 = getelementptr %"class.__gnu_cxx::__normal_iterator.0"* %retval, i32 0, i32 0
  %3 = load %class.Y** %coerce.dive3
  ret %class.Y* %3
}

define linkonce_odr %class.Y* @_ZNSt10_Iter_baseIN9__gnu_cxx17__normal_iteratorIPK1YSt6vectorIS2_SaIS2_EEEELb0EE7_S_baseES8_(%class.Y* %__it.coerce) nounwind uwtable align 2 {
entry:
  %retval = alloca %"class.__gnu_cxx::__normal_iterator.0", align 8
  %__it = alloca %"class.__gnu_cxx::__normal_iterator.0", align 8
  %coerce.dive = getelementptr %"class.__gnu_cxx::__normal_iterator.0"* %__it, i32 0, i32 0
  store %class.Y* %__it.coerce, %class.Y** %coerce.dive
  %0 = bitcast %"class.__gnu_cxx::__normal_iterator.0"* %retval to i8*
  %1 = bitcast %"class.__gnu_cxx::__normal_iterator.0"* %__it to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %0, i8* %1, i64 8, i32 8, i1 false)
  %coerce.dive1 = getelementptr %"class.__gnu_cxx::__normal_iterator.0"* %retval, i32 0, i32 0
  %2 = load %class.Y** %coerce.dive1
  ret %class.Y* %2
}

define linkonce_odr %class.Y* @_ZSt13__copy_move_aILb0EPK1YPS0_ET1_T0_S5_S4_(%class.Y* %__first, %class.Y* %__last, %class.Y* %__result) nounwind uwtable inlinehint {
entry:
  %__first.addr = alloca %class.Y*, align 8
  %__last.addr = alloca %class.Y*, align 8
  %__result.addr = alloca %class.Y*, align 8
  %__simple = alloca i8, align 1
  store %class.Y* %__first, %class.Y** %__first.addr, align 8
  store %class.Y* %__last, %class.Y** %__last.addr, align 8
  store %class.Y* %__result, %class.Y** %__result.addr, align 8
  store i8 1, i8* %__simple, align 1
  %0 = load %class.Y** %__first.addr, align 8
  %1 = load %class.Y** %__last.addr, align 8
  %2 = load %class.Y** %__result.addr, align 8
  %call = call %class.Y* @_ZNSt11__copy_moveILb0ELb1ESt26random_access_iterator_tagE8__copy_mI1YEEPT_PKS4_S7_S5_(%class.Y* %0, %class.Y* %1, %class.Y* %2)
  ret %class.Y* %call
}

define linkonce_odr %class.Y* @_ZSt12__niter_baseIN9__gnu_cxx17__normal_iteratorIPK1YSt6vectorIS2_SaIS2_EEEEENSt11_Niter_baseIT_E13iterator_typeESA_(%class.Y* %__it.coerce) nounwind uwtable inlinehint {
entry:
  %__it = alloca %"class.__gnu_cxx::__normal_iterator.0", align 8
  %agg.tmp = alloca %"class.__gnu_cxx::__normal_iterator.0", align 8
  %coerce.dive = getelementptr %"class.__gnu_cxx::__normal_iterator.0"* %__it, i32 0, i32 0
  store %class.Y* %__it.coerce, %class.Y** %coerce.dive
  %0 = bitcast %"class.__gnu_cxx::__normal_iterator.0"* %agg.tmp to i8*
  %1 = bitcast %"class.__gnu_cxx::__normal_iterator.0"* %__it to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %0, i8* %1, i64 8, i32 8, i1 false)
  %coerce.dive1 = getelementptr %"class.__gnu_cxx::__normal_iterator.0"* %agg.tmp, i32 0, i32 0
  %2 = load %class.Y** %coerce.dive1
  %call = call %class.Y* @_ZNSt10_Iter_baseIN9__gnu_cxx17__normal_iteratorIPK1YSt6vectorIS2_SaIS2_EEEELb1EE7_S_baseES8_(%class.Y* %2)
  ret %class.Y* %call
}

define linkonce_odr %class.Y* @_ZSt12__niter_baseIP1YENSt11_Niter_baseIT_E13iterator_typeES3_(%class.Y* %__it) nounwind uwtable inlinehint {
entry:
  %__it.addr = alloca %class.Y*, align 8
  store %class.Y* %__it, %class.Y** %__it.addr, align 8
  %0 = load %class.Y** %__it.addr, align 8
  %call = call %class.Y* @_ZNSt10_Iter_baseIP1YLb0EE7_S_baseES1_(%class.Y* %0)
  ret %class.Y* %call
}

define linkonce_odr %class.Y* @_ZNSt10_Iter_baseIP1YLb0EE7_S_baseES1_(%class.Y* %__it) nounwind uwtable align 2 {
entry:
  %__it.addr = alloca %class.Y*, align 8
  store %class.Y* %__it, %class.Y** %__it.addr, align 8
  %0 = load %class.Y** %__it.addr, align 8
  ret %class.Y* %0
}

define linkonce_odr %class.Y* @_ZNSt10_Iter_baseIN9__gnu_cxx17__normal_iteratorIPK1YSt6vectorIS2_SaIS2_EEEELb1EE7_S_baseES8_(%class.Y* %__it.coerce) nounwind uwtable align 2 {
entry:
  %__it = alloca %"class.__gnu_cxx::__normal_iterator.0", align 8
  %coerce.dive = getelementptr %"class.__gnu_cxx::__normal_iterator.0"* %__it, i32 0, i32 0
  store %class.Y* %__it.coerce, %class.Y** %coerce.dive
  %call = call %class.Y** @_ZNK9__gnu_cxx17__normal_iteratorIPK1YSt6vectorIS1_SaIS1_EEE4baseEv(%"class.__gnu_cxx::__normal_iterator.0"* %__it)
  %0 = load %class.Y** %call
  ret %class.Y* %0
}

define linkonce_odr %class.Y** @_ZNK9__gnu_cxx17__normal_iteratorIPK1YSt6vectorIS1_SaIS1_EEE4baseEv(%"class.__gnu_cxx::__normal_iterator.0"* %this) nounwind uwtable align 2 {
entry:
  %this.addr = alloca %"class.__gnu_cxx::__normal_iterator.0"*, align 8
  store %"class.__gnu_cxx::__normal_iterator.0"* %this, %"class.__gnu_cxx::__normal_iterator.0"** %this.addr, align 8
  %this1 = load %"class.__gnu_cxx::__normal_iterator.0"** %this.addr
  %_M_current = getelementptr inbounds %"class.__gnu_cxx::__normal_iterator.0"* %this1, i32 0, i32 0
  ret %class.Y** %_M_current
}

define linkonce_odr %class.Y* @_ZNSt11__copy_moveILb0ELb1ESt26random_access_iterator_tagE8__copy_mI1YEEPT_PKS4_S7_S5_(%class.Y* %__first, %class.Y* %__last, %class.Y* %__result) nounwind uwtable align 2 {
entry:
  %__first.addr = alloca %class.Y*, align 8
  %__last.addr = alloca %class.Y*, align 8
  %__result.addr = alloca %class.Y*, align 8
  %_Num = alloca i64, align 8
  store %class.Y* %__first, %class.Y** %__first.addr, align 8
  store %class.Y* %__last, %class.Y** %__last.addr, align 8
  store %class.Y* %__result, %class.Y** %__result.addr, align 8
  %0 = load %class.Y** %__last.addr, align 8
  %1 = load %class.Y** %__first.addr, align 8
  %sub.ptr.lhs.cast = ptrtoint %class.Y* %0 to i64
  %sub.ptr.rhs.cast = ptrtoint %class.Y* %1 to i64
  %sub.ptr.sub = sub i64 %sub.ptr.lhs.cast, %sub.ptr.rhs.cast
  %sub.ptr.div = sdiv exact i64 %sub.ptr.sub, 16
  store i64 %sub.ptr.div, i64* %_Num, align 8
  %2 = load i64* %_Num, align 8
  %tobool = icmp ne i64 %2, 0
  br i1 %tobool, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  %3 = load %class.Y** %__result.addr, align 8
  %4 = bitcast %class.Y* %3 to i8*
  %5 = load %class.Y** %__first.addr, align 8
  %6 = bitcast %class.Y* %5 to i8*
  %7 = load i64* %_Num, align 8
  %mul = mul i64 16, %7
  call void @llvm.memmove.p0i8.p0i8.i64(i8* %4, i8* %6, i64 %mul, i32 8, i1 false)
  br label %if.end

if.end:                                           ; preds = %if.then, %entry
  %8 = load %class.Y** %__result.addr, align 8
  %9 = load i64* %_Num, align 8
  %add.ptr = getelementptr inbounds %class.Y* %8, i64 %9
  ret %class.Y* %add.ptr
}

declare void @llvm.memmove.p0i8.p0i8.i64(i8* nocapture, i8* nocapture, i64, i32, i1) nounwind

define linkonce_odr void @_ZNSt12_Vector_baseI1YSaIS0_EE12_Vector_implC1ERKS1_(%"struct.std::_Vector_base<Y, std::allocator<Y> >::_Vector_impl"* %this, %"class.std::allocator"* %__a) unnamed_addr nounwind uwtable align 2 {
entry:
  %this.addr = alloca %"struct.std::_Vector_base<Y, std::allocator<Y> >::_Vector_impl"*, align 8
  %__a.addr = alloca %"class.std::allocator"*, align 8
  store %"struct.std::_Vector_base<Y, std::allocator<Y> >::_Vector_impl"* %this, %"struct.std::_Vector_base<Y, std::allocator<Y> >::_Vector_impl"** %this.addr, align 8
  store %"class.std::allocator"* %__a, %"class.std::allocator"** %__a.addr, align 8
  %this1 = load %"struct.std::_Vector_base<Y, std::allocator<Y> >::_Vector_impl"** %this.addr
  %0 = load %"class.std::allocator"** %__a.addr
  call void @_ZNSt12_Vector_baseI1YSaIS0_EE12_Vector_implC2ERKS1_(%"struct.std::_Vector_base<Y, std::allocator<Y> >::_Vector_impl"* %this1, %"class.std::allocator"* %0)
  ret void
}

define linkonce_odr void @_ZNSt12_Vector_baseI1YSaIS0_EE17_M_create_storageEm(%"struct.std::_Vector_base"* %this, i64 %__n) nounwind uwtable align 2 {
entry:
  %this.addr = alloca %"struct.std::_Vector_base"*, align 8
  %__n.addr = alloca i64, align 8
  store %"struct.std::_Vector_base"* %this, %"struct.std::_Vector_base"** %this.addr, align 8
  store i64 %__n, i64* %__n.addr, align 8
  %this1 = load %"struct.std::_Vector_base"** %this.addr
  %0 = load i64* %__n.addr, align 8
  %call = call %class.Y* @_ZNSt12_Vector_baseI1YSaIS0_EE11_M_allocateEm(%"struct.std::_Vector_base"* %this1, i64 %0)
  %_M_impl = getelementptr inbounds %"struct.std::_Vector_base"* %this1, i32 0, i32 0
  %_M_start = getelementptr inbounds %"struct.std::_Vector_base<Y, std::allocator<Y> >::_Vector_impl"* %_M_impl, i32 0, i32 0
  store %class.Y* %call, %class.Y** %_M_start, align 8
  %_M_impl2 = getelementptr inbounds %"struct.std::_Vector_base"* %this1, i32 0, i32 0
  %_M_start3 = getelementptr inbounds %"struct.std::_Vector_base<Y, std::allocator<Y> >::_Vector_impl"* %_M_impl2, i32 0, i32 0
  %1 = load %class.Y** %_M_start3, align 8
  %_M_impl4 = getelementptr inbounds %"struct.std::_Vector_base"* %this1, i32 0, i32 0
  %_M_finish = getelementptr inbounds %"struct.std::_Vector_base<Y, std::allocator<Y> >::_Vector_impl"* %_M_impl4, i32 0, i32 1
  store %class.Y* %1, %class.Y** %_M_finish, align 8
  %_M_impl5 = getelementptr inbounds %"struct.std::_Vector_base"* %this1, i32 0, i32 0
  %_M_start6 = getelementptr inbounds %"struct.std::_Vector_base<Y, std::allocator<Y> >::_Vector_impl"* %_M_impl5, i32 0, i32 0
  %2 = load %class.Y** %_M_start6, align 8
  %3 = load i64* %__n.addr, align 8
  %add.ptr = getelementptr inbounds %class.Y* %2, i64 %3
  %_M_impl7 = getelementptr inbounds %"struct.std::_Vector_base"* %this1, i32 0, i32 0
  %_M_end_of_storage = getelementptr inbounds %"struct.std::_Vector_base<Y, std::allocator<Y> >::_Vector_impl"* %_M_impl7, i32 0, i32 2
  store %class.Y* %add.ptr, %class.Y** %_M_end_of_storage, align 8
  ret void
}

define linkonce_odr %class.Y* @_ZNSt12_Vector_baseI1YSaIS0_EE11_M_allocateEm(%"struct.std::_Vector_base"* %this, i64 %__n) nounwind uwtable align 2 {
entry:
  %this.addr = alloca %"struct.std::_Vector_base"*, align 8
  %__n.addr = alloca i64, align 8
  store %"struct.std::_Vector_base"* %this, %"struct.std::_Vector_base"** %this.addr, align 8
  store i64 %__n, i64* %__n.addr, align 8
  %this1 = load %"struct.std::_Vector_base"** %this.addr
  %0 = load i64* %__n.addr, align 8
  %cmp = icmp ne i64 %0, 0
  br i1 %cmp, label %cond.true, label %cond.false

cond.true:                                        ; preds = %entry
  %_M_impl = getelementptr inbounds %"struct.std::_Vector_base"* %this1, i32 0, i32 0
  %1 = bitcast %"struct.std::_Vector_base<Y, std::allocator<Y> >::_Vector_impl"* %_M_impl to %"class.__gnu_cxx::new_allocator"*
  %2 = load i64* %__n.addr, align 8
  %call = call %class.Y* @_ZN9__gnu_cxx13new_allocatorI1YE8allocateEmPKv(%"class.__gnu_cxx::new_allocator"* %1, i64 %2, i8* null)
  br label %cond.end

cond.false:                                       ; preds = %entry
  br label %cond.end

cond.end:                                         ; preds = %cond.false, %cond.true
  %cond = phi %class.Y* [ %call, %cond.true ], [ null, %cond.false ]
  ret %class.Y* %cond
}

define linkonce_odr %class.Y* @_ZN9__gnu_cxx13new_allocatorI1YE8allocateEmPKv(%"class.__gnu_cxx::new_allocator"* %this, i64 %__n, i8*) nounwind uwtable align 2 {
entry:
  %this.addr = alloca %"class.__gnu_cxx::new_allocator"*, align 8
  %__n.addr = alloca i64, align 8
  %.addr = alloca i8*, align 8
  store %"class.__gnu_cxx::new_allocator"* %this, %"class.__gnu_cxx::new_allocator"** %this.addr, align 8
  store i64 %__n, i64* %__n.addr, align 8
  store i8* %0, i8** %.addr, align 8
  %this1 = load %"class.__gnu_cxx::new_allocator"** %this.addr
  %1 = load i64* %__n.addr, align 8
  %call = call i64 @_ZNK9__gnu_cxx13new_allocatorI1YE8max_sizeEv(%"class.__gnu_cxx::new_allocator"* %this1) nounwind
  %cmp = icmp ugt i64 %1, %call
  br i1 %cmp, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  call void @_ZSt17__throw_bad_allocv() noreturn
  unreachable

if.end:                                           ; preds = %entry
  %2 = load i64* %__n.addr, align 8
  %mul = mul i64 %2, 16
  %call2 = call noalias i8* @_Znwm(i64 %mul)
  %3 = bitcast i8* %call2 to %class.Y*
  ret %class.Y* %3
}

define linkonce_odr i64 @_ZNK9__gnu_cxx13new_allocatorI1YE8max_sizeEv(%"class.__gnu_cxx::new_allocator"* %this) nounwind uwtable align 2 {
entry:
  %this.addr = alloca %"class.__gnu_cxx::new_allocator"*, align 8
  store %"class.__gnu_cxx::new_allocator"* %this, %"class.__gnu_cxx::new_allocator"** %this.addr, align 8
  %this1 = load %"class.__gnu_cxx::new_allocator"** %this.addr
  ret i64 1152921504606846975
}

declare void @_ZSt17__throw_bad_allocv() noreturn

declare noalias i8* @_Znwm(i64)

define linkonce_odr void @_ZNSt12_Vector_baseI1YSaIS0_EE12_Vector_implC2ERKS1_(%"struct.std::_Vector_base<Y, std::allocator<Y> >::_Vector_impl"* %this, %"class.std::allocator"* %__a) unnamed_addr nounwind uwtable align 2 {
entry:
  %this.addr = alloca %"struct.std::_Vector_base<Y, std::allocator<Y> >::_Vector_impl"*, align 8
  %__a.addr = alloca %"class.std::allocator"*, align 8
  store %"struct.std::_Vector_base<Y, std::allocator<Y> >::_Vector_impl"* %this, %"struct.std::_Vector_base<Y, std::allocator<Y> >::_Vector_impl"** %this.addr, align 8
  store %"class.std::allocator"* %__a, %"class.std::allocator"** %__a.addr, align 8
  %this1 = load %"struct.std::_Vector_base<Y, std::allocator<Y> >::_Vector_impl"** %this.addr
  %0 = bitcast %"struct.std::_Vector_base<Y, std::allocator<Y> >::_Vector_impl"* %this1 to %"class.std::allocator"*
  %1 = load %"class.std::allocator"** %__a.addr, align 8
  call void @_ZNSaI1YEC2ERKS0_(%"class.std::allocator"* %0, %"class.std::allocator"* %1) nounwind
  %_M_start = getelementptr inbounds %"struct.std::_Vector_base<Y, std::allocator<Y> >::_Vector_impl"* %this1, i32 0, i32 0
  store %class.Y* null, %class.Y** %_M_start, align 8
  %_M_finish = getelementptr inbounds %"struct.std::_Vector_base<Y, std::allocator<Y> >::_Vector_impl"* %this1, i32 0, i32 1
  store %class.Y* null, %class.Y** %_M_finish, align 8
  %_M_end_of_storage = getelementptr inbounds %"struct.std::_Vector_base<Y, std::allocator<Y> >::_Vector_impl"* %this1, i32 0, i32 2
  store %class.Y* null, %class.Y** %_M_end_of_storage, align 8
  ret void
}

define linkonce_odr void @_ZNSaI1YEC2ERKS0_(%"class.std::allocator"* %this, %"class.std::allocator"* %__a) unnamed_addr nounwind uwtable align 2 {
entry:
  %this.addr = alloca %"class.std::allocator"*, align 8
  %__a.addr = alloca %"class.std::allocator"*, align 8
  store %"class.std::allocator"* %this, %"class.std::allocator"** %this.addr, align 8
  store %"class.std::allocator"* %__a, %"class.std::allocator"** %__a.addr, align 8
  %this1 = load %"class.std::allocator"** %this.addr
  %0 = bitcast %"class.std::allocator"* %this1 to %"class.__gnu_cxx::new_allocator"*
  %1 = load %"class.std::allocator"** %__a.addr, align 8
  %2 = bitcast %"class.std::allocator"* %1 to %"class.__gnu_cxx::new_allocator"*
  call void @_ZN9__gnu_cxx13new_allocatorI1YEC2ERKS2_(%"class.__gnu_cxx::new_allocator"* %0, %"class.__gnu_cxx::new_allocator"* %2) nounwind
  ret void
}

define linkonce_odr void @_ZN9__gnu_cxx13new_allocatorI1YEC2ERKS2_(%"class.__gnu_cxx::new_allocator"* %this, %"class.__gnu_cxx::new_allocator"*) unnamed_addr nounwind uwtable align 2 {
entry:
  %this.addr = alloca %"class.__gnu_cxx::new_allocator"*, align 8
  %.addr = alloca %"class.__gnu_cxx::new_allocator"*, align 8
  store %"class.__gnu_cxx::new_allocator"* %this, %"class.__gnu_cxx::new_allocator"** %this.addr, align 8
  store %"class.__gnu_cxx::new_allocator"* %0, %"class.__gnu_cxx::new_allocator"** %.addr, align 8
  %this1 = load %"class.__gnu_cxx::new_allocator"** %this.addr
  ret void
}

define linkonce_odr void @_ZNSt6vectorI1YSaIS0_EED2Ev(%"class.std::vector"* %this) unnamed_addr nounwind uwtable align 2 {
entry:
  %this.addr = alloca %"class.std::vector"*, align 8
  store %"class.std::vector"* %this, %"class.std::vector"** %this.addr, align 8
  %this1 = load %"class.std::vector"** %this.addr
  %0 = bitcast %"class.std::vector"* %this1 to %"struct.std::_Vector_base"*
  %_M_impl = getelementptr inbounds %"struct.std::_Vector_base"* %0, i32 0, i32 0
  %_M_start = getelementptr inbounds %"struct.std::_Vector_base<Y, std::allocator<Y> >::_Vector_impl"* %_M_impl, i32 0, i32 0
  %1 = load %class.Y** %_M_start, align 8
  %2 = bitcast %"class.std::vector"* %this1 to %"struct.std::_Vector_base"*
  %_M_impl2 = getelementptr inbounds %"struct.std::_Vector_base"* %2, i32 0, i32 0
  %_M_finish = getelementptr inbounds %"struct.std::_Vector_base<Y, std::allocator<Y> >::_Vector_impl"* %_M_impl2, i32 0, i32 1
  %3 = load %class.Y** %_M_finish, align 8
  %4 = bitcast %"class.std::vector"* %this1 to %"struct.std::_Vector_base"*
  %call = call %"class.std::allocator"* @_ZNSt12_Vector_baseI1YSaIS0_EE19_M_get_Tp_allocatorEv(%"struct.std::_Vector_base"* %4)
  call void @_ZSt8_DestroyIP1YS0_EvT_S2_RSaIT0_E(%class.Y* %1, %class.Y* %3, %"class.std::allocator"* %call)
  %5 = bitcast %"class.std::vector"* %this1 to %"struct.std::_Vector_base"*
  call void @_ZNSt12_Vector_baseI1YSaIS0_EED2Ev(%"struct.std::_Vector_base"* %5)
  ret void
}

define linkonce_odr void @_ZSt8_DestroyIP1YS0_EvT_S2_RSaIT0_E(%class.Y* %__first, %class.Y* %__last, %"class.std::allocator"*) nounwind uwtable inlinehint {
entry:
  %__first.addr = alloca %class.Y*, align 8
  %__last.addr = alloca %class.Y*, align 8
  %.addr = alloca %"class.std::allocator"*, align 8
  store %class.Y* %__first, %class.Y** %__first.addr, align 8
  store %class.Y* %__last, %class.Y** %__last.addr, align 8
  store %"class.std::allocator"* %0, %"class.std::allocator"** %.addr, align 8
  %1 = load %class.Y** %__first.addr, align 8
  %2 = load %class.Y** %__last.addr, align 8
  call void @_ZSt8_DestroyIP1YEvT_S2_(%class.Y* %1, %class.Y* %2)
  ret void
}

define linkonce_odr void @_ZNSt12_Vector_baseI1YSaIS0_EED2Ev(%"struct.std::_Vector_base"* %this) unnamed_addr nounwind uwtable align 2 {
entry:
  %this.addr = alloca %"struct.std::_Vector_base"*, align 8
  store %"struct.std::_Vector_base"* %this, %"struct.std::_Vector_base"** %this.addr, align 8
  %this1 = load %"struct.std::_Vector_base"** %this.addr
  %_M_impl = getelementptr inbounds %"struct.std::_Vector_base"* %this1, i32 0, i32 0
  %_M_start = getelementptr inbounds %"struct.std::_Vector_base<Y, std::allocator<Y> >::_Vector_impl"* %_M_impl, i32 0, i32 0
  %0 = load %class.Y** %_M_start, align 8
  %_M_impl2 = getelementptr inbounds %"struct.std::_Vector_base"* %this1, i32 0, i32 0
  %_M_end_of_storage = getelementptr inbounds %"struct.std::_Vector_base<Y, std::allocator<Y> >::_Vector_impl"* %_M_impl2, i32 0, i32 2
  %1 = load %class.Y** %_M_end_of_storage, align 8
  %_M_impl3 = getelementptr inbounds %"struct.std::_Vector_base"* %this1, i32 0, i32 0
  %_M_start4 = getelementptr inbounds %"struct.std::_Vector_base<Y, std::allocator<Y> >::_Vector_impl"* %_M_impl3, i32 0, i32 0
  %2 = load %class.Y** %_M_start4, align 8
  %sub.ptr.lhs.cast = ptrtoint %class.Y* %1 to i64
  %sub.ptr.rhs.cast = ptrtoint %class.Y* %2 to i64
  %sub.ptr.sub = sub i64 %sub.ptr.lhs.cast, %sub.ptr.rhs.cast
  %sub.ptr.div = sdiv exact i64 %sub.ptr.sub, 16
  call void @_ZNSt12_Vector_baseI1YSaIS0_EE13_M_deallocateEPS0_m(%"struct.std::_Vector_base"* %this1, %class.Y* %0, i64 %sub.ptr.div)
  %_M_impl5 = getelementptr inbounds %"struct.std::_Vector_base"* %this1, i32 0, i32 0
  call void @_ZNSt12_Vector_baseI1YSaIS0_EE12_Vector_implD1Ev(%"struct.std::_Vector_base<Y, std::allocator<Y> >::_Vector_impl"* %_M_impl5) nounwind
  ret void
}

define linkonce_odr void @_ZNSt12_Vector_baseI1YSaIS0_EE13_M_deallocateEPS0_m(%"struct.std::_Vector_base"* %this, %class.Y* %__p, i64 %__n) nounwind uwtable align 2 {
entry:
  %this.addr = alloca %"struct.std::_Vector_base"*, align 8
  %__p.addr = alloca %class.Y*, align 8
  %__n.addr = alloca i64, align 8
  store %"struct.std::_Vector_base"* %this, %"struct.std::_Vector_base"** %this.addr, align 8
  store %class.Y* %__p, %class.Y** %__p.addr, align 8
  store i64 %__n, i64* %__n.addr, align 8
  %this1 = load %"struct.std::_Vector_base"** %this.addr
  %0 = load %class.Y** %__p.addr, align 8
  %tobool = icmp ne %class.Y* %0, null
  br i1 %tobool, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  %_M_impl = getelementptr inbounds %"struct.std::_Vector_base"* %this1, i32 0, i32 0
  %1 = bitcast %"struct.std::_Vector_base<Y, std::allocator<Y> >::_Vector_impl"* %_M_impl to %"class.__gnu_cxx::new_allocator"*
  %2 = load %class.Y** %__p.addr, align 8
  %3 = load i64* %__n.addr, align 8
  call void @_ZN9__gnu_cxx13new_allocatorI1YE10deallocateEPS1_m(%"class.__gnu_cxx::new_allocator"* %1, %class.Y* %2, i64 %3)
  br label %if.end

if.end:                                           ; preds = %if.then, %entry
  ret void
}

define linkonce_odr void @_ZNSt12_Vector_baseI1YSaIS0_EE12_Vector_implD1Ev(%"struct.std::_Vector_base<Y, std::allocator<Y> >::_Vector_impl"* %this) unnamed_addr nounwind uwtable inlinehint align 2 {
entry:
  %this.addr = alloca %"struct.std::_Vector_base<Y, std::allocator<Y> >::_Vector_impl"*, align 8
  store %"struct.std::_Vector_base<Y, std::allocator<Y> >::_Vector_impl"* %this, %"struct.std::_Vector_base<Y, std::allocator<Y> >::_Vector_impl"** %this.addr, align 8
  %this1 = load %"struct.std::_Vector_base<Y, std::allocator<Y> >::_Vector_impl"** %this.addr
  call void @_ZNSt12_Vector_baseI1YSaIS0_EE12_Vector_implD2Ev(%"struct.std::_Vector_base<Y, std::allocator<Y> >::_Vector_impl"* %this1) nounwind
  ret void
}

define linkonce_odr void @_ZNSt12_Vector_baseI1YSaIS0_EE12_Vector_implD2Ev(%"struct.std::_Vector_base<Y, std::allocator<Y> >::_Vector_impl"* %this) unnamed_addr nounwind uwtable inlinehint align 2 {
entry:
  %this.addr = alloca %"struct.std::_Vector_base<Y, std::allocator<Y> >::_Vector_impl"*, align 8
  store %"struct.std::_Vector_base<Y, std::allocator<Y> >::_Vector_impl"* %this, %"struct.std::_Vector_base<Y, std::allocator<Y> >::_Vector_impl"** %this.addr, align 8
  %this1 = load %"struct.std::_Vector_base<Y, std::allocator<Y> >::_Vector_impl"** %this.addr
  %0 = bitcast %"struct.std::_Vector_base<Y, std::allocator<Y> >::_Vector_impl"* %this1 to %"class.std::allocator"*
  call void @_ZNSaI1YED2Ev(%"class.std::allocator"* %0) nounwind
  ret void
}

define linkonce_odr void @_ZNSaI1YED2Ev(%"class.std::allocator"* %this) unnamed_addr nounwind uwtable align 2 {
entry:
  %this.addr = alloca %"class.std::allocator"*, align 8
  store %"class.std::allocator"* %this, %"class.std::allocator"** %this.addr, align 8
  %this1 = load %"class.std::allocator"** %this.addr
  %0 = bitcast %"class.std::allocator"* %this1 to %"class.__gnu_cxx::new_allocator"*
  call void @_ZN9__gnu_cxx13new_allocatorI1YED2Ev(%"class.__gnu_cxx::new_allocator"* %0) nounwind
  ret void
}

define linkonce_odr void @_ZN9__gnu_cxx13new_allocatorI1YED2Ev(%"class.__gnu_cxx::new_allocator"* %this) unnamed_addr nounwind uwtable align 2 {
entry:
  %this.addr = alloca %"class.__gnu_cxx::new_allocator"*, align 8
  store %"class.__gnu_cxx::new_allocator"* %this, %"class.__gnu_cxx::new_allocator"** %this.addr, align 8
  %this1 = load %"class.__gnu_cxx::new_allocator"** %this.addr
  ret void
}

define linkonce_odr void @_ZN9__gnu_cxx13new_allocatorI1YE10deallocateEPS1_m(%"class.__gnu_cxx::new_allocator"* %this, %class.Y* %__p, i64) nounwind uwtable align 2 {
entry:
  %this.addr = alloca %"class.__gnu_cxx::new_allocator"*, align 8
  %__p.addr = alloca %class.Y*, align 8
  %.addr = alloca i64, align 8
  store %"class.__gnu_cxx::new_allocator"* %this, %"class.__gnu_cxx::new_allocator"** %this.addr, align 8
  store %class.Y* %__p, %class.Y** %__p.addr, align 8
  store i64 %0, i64* %.addr, align 8
  %this1 = load %"class.__gnu_cxx::new_allocator"** %this.addr
  %1 = load %class.Y** %__p.addr, align 8
  %2 = bitcast %class.Y* %1 to i8*
  call void @_ZdlPv(i8* %2) nounwind
  ret void
}

declare void @_ZdlPv(i8*) nounwind

define linkonce_odr void @_ZSt8_DestroyIP1YEvT_S2_(%class.Y* %__first, %class.Y* %__last) nounwind uwtable inlinehint {
entry:
  %__first.addr = alloca %class.Y*, align 8
  %__last.addr = alloca %class.Y*, align 8
  store %class.Y* %__first, %class.Y** %__first.addr, align 8
  store %class.Y* %__last, %class.Y** %__last.addr, align 8
  %0 = load %class.Y** %__first.addr, align 8
  %1 = load %class.Y** %__last.addr, align 8
  call void @_ZNSt12_Destroy_auxILb1EE9__destroyIP1YEEvT_S4_(%class.Y* %0, %class.Y* %1)
  ret void
}

define linkonce_odr void @_ZNSt12_Destroy_auxILb1EE9__destroyIP1YEEvT_S4_(%class.Y*, %class.Y*) nounwind uwtable align 2 {
entry:
  %.addr = alloca %class.Y*, align 8
  %.addr1 = alloca %class.Y*, align 8
  store %class.Y* %0, %class.Y** %.addr, align 8
  store %class.Y* %1, %class.Y** %.addr1, align 8
  ret void
}

define linkonce_odr void @_ZNSt6vectorI1YSaIS0_EEC2EmRKS0_RKS1_(%"class.std::vector"* %this, i64 %__n, %class.Y* %__value, %"class.std::allocator"* %__a) unnamed_addr nounwind uwtable align 2 {
entry:
  %this.addr = alloca %"class.std::vector"*, align 8
  %__n.addr = alloca i64, align 8
  %__value.addr = alloca %class.Y*, align 8
  %__a.addr = alloca %"class.std::allocator"*, align 8
  store %"class.std::vector"* %this, %"class.std::vector"** %this.addr, align 8
  store i64 %__n, i64* %__n.addr, align 8
  store %class.Y* %__value, %class.Y** %__value.addr, align 8
  store %"class.std::allocator"* %__a, %"class.std::allocator"** %__a.addr, align 8
  %this1 = load %"class.std::vector"** %this.addr
  %0 = bitcast %"class.std::vector"* %this1 to %"struct.std::_Vector_base"*
  %1 = load i64* %__n.addr, align 8
  %2 = load %"class.std::allocator"** %__a.addr, align 8
  call void @_ZNSt12_Vector_baseI1YSaIS0_EEC2EmRKS1_(%"struct.std::_Vector_base"* %0, i64 %1, %"class.std::allocator"* %2)
  %3 = load i64* %__n.addr, align 8
  %4 = load %class.Y** %__value.addr, align 8
  call void @_ZNSt6vectorI1YSaIS0_EE18_M_fill_initializeEmRKS0_(%"class.std::vector"* %this1, i64 %3, %class.Y* %4)
  ret void
}

define linkonce_odr void @_ZNSt6vectorI1YSaIS0_EE18_M_fill_initializeEmRKS0_(%"class.std::vector"* %this, i64 %__n, %class.Y* %__value) nounwind uwtable align 2 {
entry:
  %this.addr = alloca %"class.std::vector"*, align 8
  %__n.addr = alloca i64, align 8
  %__value.addr = alloca %class.Y*, align 8
  store %"class.std::vector"* %this, %"class.std::vector"** %this.addr, align 8
  store i64 %__n, i64* %__n.addr, align 8
  store %class.Y* %__value, %class.Y** %__value.addr, align 8
  %this1 = load %"class.std::vector"** %this.addr
  %0 = bitcast %"class.std::vector"* %this1 to %"struct.std::_Vector_base"*
  %_M_impl = getelementptr inbounds %"struct.std::_Vector_base"* %0, i32 0, i32 0
  %_M_start = getelementptr inbounds %"struct.std::_Vector_base<Y, std::allocator<Y> >::_Vector_impl"* %_M_impl, i32 0, i32 0
  %1 = load %class.Y** %_M_start, align 8
  %2 = load i64* %__n.addr, align 8
  %3 = load %class.Y** %__value.addr, align 8
  %4 = bitcast %"class.std::vector"* %this1 to %"struct.std::_Vector_base"*
  %call = call %"class.std::allocator"* @_ZNSt12_Vector_baseI1YSaIS0_EE19_M_get_Tp_allocatorEv(%"struct.std::_Vector_base"* %4)
  call void @_ZSt24__uninitialized_fill_n_aIP1YmS0_S0_EvT_T0_RKT1_RSaIT2_E(%class.Y* %1, i64 %2, %class.Y* %3, %"class.std::allocator"* %call)
  %5 = bitcast %"class.std::vector"* %this1 to %"struct.std::_Vector_base"*
  %_M_impl2 = getelementptr inbounds %"struct.std::_Vector_base"* %5, i32 0, i32 0
  %_M_end_of_storage = getelementptr inbounds %"struct.std::_Vector_base<Y, std::allocator<Y> >::_Vector_impl"* %_M_impl2, i32 0, i32 2
  %6 = load %class.Y** %_M_end_of_storage, align 8
  %7 = bitcast %"class.std::vector"* %this1 to %"struct.std::_Vector_base"*
  %_M_impl3 = getelementptr inbounds %"struct.std::_Vector_base"* %7, i32 0, i32 0
  %_M_finish = getelementptr inbounds %"struct.std::_Vector_base<Y, std::allocator<Y> >::_Vector_impl"* %_M_impl3, i32 0, i32 1
  store %class.Y* %6, %class.Y** %_M_finish, align 8
  ret void
}

define linkonce_odr void @_ZSt24__uninitialized_fill_n_aIP1YmS0_S0_EvT_T0_RKT1_RSaIT2_E(%class.Y* %__first, i64 %__n, %class.Y* %__x, %"class.std::allocator"*) nounwind uwtable inlinehint {
entry:
  %__first.addr = alloca %class.Y*, align 8
  %__n.addr = alloca i64, align 8
  %__x.addr = alloca %class.Y*, align 8
  %.addr = alloca %"class.std::allocator"*, align 8
  store %class.Y* %__first, %class.Y** %__first.addr, align 8
  store i64 %__n, i64* %__n.addr, align 8
  store %class.Y* %__x, %class.Y** %__x.addr, align 8
  store %"class.std::allocator"* %0, %"class.std::allocator"** %.addr, align 8
  %1 = load %class.Y** %__first.addr, align 8
  %2 = load i64* %__n.addr, align 8
  %3 = load %class.Y** %__x.addr, align 8
  call void @_ZSt20uninitialized_fill_nIP1YmS0_EvT_T0_RKT1_(%class.Y* %1, i64 %2, %class.Y* %3)
  ret void
}

define linkonce_odr void @_ZSt20uninitialized_fill_nIP1YmS0_EvT_T0_RKT1_(%class.Y* %__first, i64 %__n, %class.Y* %__x) nounwind uwtable inlinehint {
entry:
  %__first.addr = alloca %class.Y*, align 8
  %__n.addr = alloca i64, align 8
  %__x.addr = alloca %class.Y*, align 8
  store %class.Y* %__first, %class.Y** %__first.addr, align 8
  store i64 %__n, i64* %__n.addr, align 8
  store %class.Y* %__x, %class.Y** %__x.addr, align 8
  %0 = load %class.Y** %__first.addr, align 8
  %1 = load i64* %__n.addr, align 8
  %2 = load %class.Y** %__x.addr, align 8
  call void @_ZNSt22__uninitialized_fill_nILb1EE15__uninit_fill_nIP1YmS2_EEvT_T0_RKT1_(%class.Y* %0, i64 %1, %class.Y* %2)
  ret void
}

define linkonce_odr void @_ZNSt22__uninitialized_fill_nILb1EE15__uninit_fill_nIP1YmS2_EEvT_T0_RKT1_(%class.Y* %__first, i64 %__n, %class.Y* %__x) nounwind uwtable align 2 {
entry:
  %__first.addr = alloca %class.Y*, align 8
  %__n.addr = alloca i64, align 8
  %__x.addr = alloca %class.Y*, align 8
  store %class.Y* %__first, %class.Y** %__first.addr, align 8
  store i64 %__n, i64* %__n.addr, align 8
  store %class.Y* %__x, %class.Y** %__x.addr, align 8
  %0 = load %class.Y** %__first.addr, align 8
  %1 = load i64* %__n.addr, align 8
  %2 = load %class.Y** %__x.addr, align 8
  %call = call %class.Y* @_ZSt6fill_nIP1YmS0_ET_S2_T0_RKT1_(%class.Y* %0, i64 %1, %class.Y* %2)
  ret void
}

define linkonce_odr %class.Y* @_ZSt6fill_nIP1YmS0_ET_S2_T0_RKT1_(%class.Y* %__first, i64 %__n, %class.Y* %__value) nounwind uwtable inlinehint {
entry:
  %__first.addr = alloca %class.Y*, align 8
  %__n.addr = alloca i64, align 8
  %__value.addr = alloca %class.Y*, align 8
  store %class.Y* %__first, %class.Y** %__first.addr, align 8
  store i64 %__n, i64* %__n.addr, align 8
  store %class.Y* %__value, %class.Y** %__value.addr, align 8
  %0 = load %class.Y** %__first.addr, align 8
  %call = call %class.Y* @_ZSt12__niter_baseIP1YENSt11_Niter_baseIT_E13iterator_typeES3_(%class.Y* %0)
  %1 = load i64* %__n.addr, align 8
  %2 = load %class.Y** %__value.addr, align 8
  %call1 = call %class.Y* @_ZSt10__fill_n_aIP1YmS0_EN9__gnu_cxx11__enable_ifIXntsr11__is_scalarIT1_EE7__valueET_E6__typeES5_T0_RKS4_(%class.Y* %call, i64 %1, %class.Y* %2)
  ret %class.Y* %call1
}

define linkonce_odr %class.Y* @_ZSt10__fill_n_aIP1YmS0_EN9__gnu_cxx11__enable_ifIXntsr11__is_scalarIT1_EE7__valueET_E6__typeES5_T0_RKS4_(%class.Y* %__first, i64 %__n, %class.Y* %__value) nounwind uwtable inlinehint {
entry:
  %__first.addr = alloca %class.Y*, align 8
  %__n.addr = alloca i64, align 8
  %__value.addr = alloca %class.Y*, align 8
  %__niter = alloca i64, align 8
  store %class.Y* %__first, %class.Y** %__first.addr, align 8
  store i64 %__n, i64* %__n.addr, align 8
  store %class.Y* %__value, %class.Y** %__value.addr, align 8
  %0 = load i64* %__n.addr, align 8
  store i64 %0, i64* %__niter, align 8
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %1 = load i64* %__niter, align 8
  %cmp = icmp ugt i64 %1, 0
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %2 = load %class.Y** %__first.addr, align 8
  %3 = load %class.Y** %__value.addr, align 8
  %4 = bitcast %class.Y* %2 to i8*
  %5 = bitcast %class.Y* %3 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %4, i8* %5, i64 16, i32 8, i1 false)
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %6 = load i64* %__niter, align 8
  %dec = add i64 %6, -1
  store i64 %dec, i64* %__niter, align 8
  %7 = load %class.Y** %__first.addr, align 8
  %incdec.ptr = getelementptr inbounds %class.Y* %7, i32 1
  store %class.Y* %incdec.ptr, %class.Y** %__first.addr, align 8
  br label %for.cond

for.end:                                          ; preds = %for.cond
  %8 = load %class.Y** %__first.addr, align 8
  ret %class.Y* %8
}

define linkonce_odr void @_ZNSaI1YEC2Ev(%"class.std::allocator"* %this) unnamed_addr nounwind uwtable align 2 {
entry:
  %this.addr = alloca %"class.std::allocator"*, align 8
  store %"class.std::allocator"* %this, %"class.std::allocator"** %this.addr, align 8
  %this1 = load %"class.std::allocator"** %this.addr
  %0 = bitcast %"class.std::allocator"* %this1 to %"class.__gnu_cxx::new_allocator"*
  call void @_ZN9__gnu_cxx13new_allocatorI1YEC2Ev(%"class.__gnu_cxx::new_allocator"* %0) nounwind
  ret void
}

define linkonce_odr void @_ZN9__gnu_cxx13new_allocatorI1YEC2Ev(%"class.__gnu_cxx::new_allocator"* %this) unnamed_addr nounwind uwtable align 2 {
entry:
  %this.addr = alloca %"class.__gnu_cxx::new_allocator"*, align 8
  store %"class.__gnu_cxx::new_allocator"* %this, %"class.__gnu_cxx::new_allocator"** %this.addr, align 8
  %this1 = load %"class.__gnu_cxx::new_allocator"** %this.addr
  ret void
}

define linkonce_odr %class.Y** @_ZNK9__gnu_cxx17__normal_iteratorIP1YSt6vectorIS1_SaIS1_EEE4baseEv(%"class.__gnu_cxx::__normal_iterator"* %this) nounwind uwtable align 2 {
entry:
  %this.addr = alloca %"class.__gnu_cxx::__normal_iterator"*, align 8
  store %"class.__gnu_cxx::__normal_iterator"* %this, %"class.__gnu_cxx::__normal_iterator"** %this.addr, align 8
  %this1 = load %"class.__gnu_cxx::__normal_iterator"** %this.addr
  %_M_current = getelementptr inbounds %"class.__gnu_cxx::__normal_iterator"* %this1, i32 0, i32 0
  ret %class.Y** %_M_current
}

define linkonce_odr void @_ZN9__gnu_cxx17__normal_iteratorIP1YSt6vectorIS1_SaIS1_EEEC1ERKS2_(%"class.__gnu_cxx::__normal_iterator"* %this, %class.Y** %__i) unnamed_addr nounwind uwtable align 2 {
entry:
  %this.addr = alloca %"class.__gnu_cxx::__normal_iterator"*, align 8
  %__i.addr = alloca %class.Y**, align 8
  store %"class.__gnu_cxx::__normal_iterator"* %this, %"class.__gnu_cxx::__normal_iterator"** %this.addr, align 8
  store %class.Y** %__i, %class.Y*** %__i.addr, align 8
  %this1 = load %"class.__gnu_cxx::__normal_iterator"** %this.addr
  %0 = load %class.Y*** %__i.addr
  call void @_ZN9__gnu_cxx17__normal_iteratorIP1YSt6vectorIS1_SaIS1_EEEC2ERKS2_(%"class.__gnu_cxx::__normal_iterator"* %this1, %class.Y** %0)
  ret void
}

define linkonce_odr void @_ZN9__gnu_cxx17__normal_iteratorIP1YSt6vectorIS1_SaIS1_EEEC2ERKS2_(%"class.__gnu_cxx::__normal_iterator"* %this, %class.Y** %__i) unnamed_addr nounwind uwtable align 2 {
entry:
  %this.addr = alloca %"class.__gnu_cxx::__normal_iterator"*, align 8
  %__i.addr = alloca %class.Y**, align 8
  store %"class.__gnu_cxx::__normal_iterator"* %this, %"class.__gnu_cxx::__normal_iterator"** %this.addr, align 8
  store %class.Y** %__i, %class.Y*** %__i.addr, align 8
  %this1 = load %"class.__gnu_cxx::__normal_iterator"** %this.addr
  %_M_current = getelementptr inbounds %"class.__gnu_cxx::__normal_iterator"* %this1, i32 0, i32 0
  %0 = load %class.Y*** %__i.addr, align 8
  %1 = load %class.Y** %0, align 8
  store %class.Y* %1, %class.Y** %_M_current, align 8
  ret void
}

define internal void @_GLOBAL__I_a() nounwind section ".text.startup" {
entry:
  call void @__cxx_global_var_init()
  ret void
}
