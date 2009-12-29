#ifndef COS_FUNCTOR_H
#define COS_FUNCTOR_H

/*
 o---------------------------------------------------------------------o
 |
 | COS Functor (closure and expression)
 |
 o---------------------------------------------------------------------o
 |
 | C Object System
 |
 | Copyright (c) 2006+ Laurent Deniau, laurent.deniau@cern.ch
 |
 | For more information, see:
 | http://cern.ch/laurent.deniau/cos.html
 |
 o---------------------------------------------------------------------o
 |
 | This file is part of the C Object System framework.
 |
 | The C Object System is free software; you can redistribute it and/or
 | modify it under the terms of the GNU Lesser General Public License
 | as published by the Free Software Foundation; either version 3 of
 | the License, or (at your option) any later version.
 |
 | The C Object System is distributed in the hope that it will be
 | useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 | of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 |
 | See <http://www.gnu.org/licenses> for more details.
 |
 o---------------------------------------------------------------------o
 |
 | $Id: Functor.h,v 1.20 2009/12/29 19:52:24 ldeniau Exp $
 |
*/

#include <cos/Object.h>

// ----- Functor class cluster

defclass(Functor) endclass

defclass(VarExpr, Functor)          endclass  // variables
defclass(FunExpr, Functor) STR str; endclass  // functors
defclass(MthExpr, Functor) STR str; endclass  // methods (private)

// ----- Functor variable

defclass(Argument, VarExpr) U32 idx; endclass  // placeholder
defclass(Variable, VarExpr) OBJ var; endclass  // variable

// ----- Functor function

defclass(Function0, FunExpr)
  FUN0 fct;
endclass

defclass(Function1, FunExpr)
  FUN1 fct;
endclass

defclass(Function2, FunExpr)
  FUN2 fct;
endclass

defclass(Function3, FunExpr)
  FUN3 fct;
endclass

defclass(Function4, FunExpr)
  FUN4 fct;
endclass

defclass(Function5, FunExpr)
  FUN5 fct;
endclass

defclass(Function6, FunExpr)
  FUN6 fct;
endclass

defclass(Function7, FunExpr)
  FUN7 fct;
endclass

defclass(Function8, FunExpr)
  FUN8 fct;
endclass

defclass(Function9, FunExpr)
  FUN9 fct;
endclass

// ----- Functor expression

defclass(FunExpr1, FunExpr)
  FUN1 fct;
  U32  msk;
  I32  ary;
  OBJ  arg[1];
endclass

defclass(FunExpr2, FunExpr)
  FUN2 fct;
  U32  msk;
  I32  ary;
  OBJ  arg[2];
endclass

defclass(FunExpr3, FunExpr)
  FUN3 fct;
  U32  msk;
  I32  ary;
  OBJ  arg[3];
endclass

defclass(FunExpr4, FunExpr)
  FUN4 fct;
  U32  msk;
  I32  ary;
  OBJ  arg[4];
endclass

defclass(FunExpr5, FunExpr)
  FUN5 fct;
  U32  msk;
  I32  ary;
  OBJ  arg[5];
endclass

defclass(FunExpr6, FunExpr)
  FUN6 fct;
  U32  msk;
  I32  ary;
  OBJ  arg[6];
endclass

defclass(FunExpr7, FunExpr)
  FUN7 fct;
  U32  msk;
  I32  ary;
  OBJ  arg[7];
endclass

defclass(FunExpr8, FunExpr)
  FUN8 fct;
  U32  msk;
  I32  ary;
  OBJ  arg[8];
endclass

defclass(FunExpr9, FunExpr)
  FUN9 fct;
  U32  msk;
  I32  ary;
  OBJ  arg[9];
endclass

// ----- Delayed functor

defclass(LazyFun, Functor)
  OBJ fun;
endclass

// ----- Composition of functors

defclass(ComposeFun, Functor)
  U32 size;
  OBJ fun[];
endclass

// ----- Iteration of functor

defclass(IterateFun, Functor)
  U32  num;
  OBJ  fun;
  FUN1 fct;
endclass

// ----- automatic constructor

#define aFunctor(...)  ( (OBJ)atFunctor (__VA_ARGS__) )
#define aLazyFun(...)  ( (OBJ)atLazyFun (__VA_ARGS__) )
#define aArgument(...) ( (OBJ)atArgument(__VA_ARGS__) )
#define aVariable(...) ( (OBJ)atVariable(__VA_ARGS__) )

// --- shortcuts

#ifndef COS_NOSHORTCUT
#define aFun(...)  aFunctor (__VA_ARGS__)
#define aLzy(...)  aLazyFun (__VA_ARGS__)
#define aArg(...)  aArgument(__VA_ARGS__)
#define aVar(...)  aVariable(__VA_ARGS__)
#endif

// --- placeholders

#ifndef COS_NOPLACEHOLDER
#define __1  aArgument(0)
#define __2  aArgument(1)
#define __3  aArgument(2)
#define __4  aArgument(3)
#define __5  aArgument(4)
#define __6  aArgument(5)
#define __7  aArgument(6)
#define __8  aArgument(7)
#define __9  aArgument(8)
#endif

/***********************************************************
 * Implementation (private)
 */

// ----- Functors

#define atFunctor(...) \
  COS_PP_IF(COS_PP_2ARGS(__VA_ARGS__)) \
    (atFunctorF(__VA_ARGS__),atFunctor0(__VA_ARGS__))

#define atFunctor0(F) \
  ( (struct Functor*)&(struct Function0) { \
    {{ cos_object_auto(Function0) }, COS_PP_STR(F) }, F })

#define atFunctorF(F,...) \
  atFunctorN(COS_PP_NARG(__VA_ARGS__), F, __VA_ARGS__)

#define atFunctorN(N,F,...) \
  COS_PP_CAT(FunExpr_init,N)( &(struct COS_PP_CAT(FunExpr,N)) { \
    {{ cos_object_auto(COS_PP_CAT(FunExpr,N)) }, COS_PP_STR(F) }, \
    F, 0, -1, {__VA_ARGS__} }, __FILE__, __LINE__)

// ----- Lazy modifier

#define atLazyFun(FUN) \
  ( &(struct LazyFun){ { cos_object_auto(LazyFun) }, (FUN) } )

// ----- Variables

#define atArgument(IDX) \
  ( &(struct Argument){ {{ cos_object_auto(Argument) }}, (IDX) } )

#define atVariable(VAR) \
  ( &(struct Variable){ {{ cos_object_auto(Variable) }}, (VAR) } )

// ----- initializers

struct Functor* FunExpr_init1(struct FunExpr1*, STR file, int line);
struct Functor* FunExpr_init2(struct FunExpr2*, STR file, int line);
struct Functor* FunExpr_init3(struct FunExpr3*, STR file, int line);
struct Functor* FunExpr_init4(struct FunExpr4*, STR file, int line);
struct Functor* FunExpr_init5(struct FunExpr5*, STR file, int line);
struct Functor* FunExpr_init6(struct FunExpr6*, STR file, int line);
struct Functor* FunExpr_init7(struct FunExpr7*, STR file, int line);
struct Functor* FunExpr_init8(struct FunExpr8*, STR file, int line);
struct Functor* FunExpr_init9(struct FunExpr9*, STR file, int line);

#endif // COS_FUNCTOR_H
