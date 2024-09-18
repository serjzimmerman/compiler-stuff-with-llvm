; ModuleID = 'day_night.2a6c25ac55b8f206-cgu.0'
source_filename = "day_night.2a6c25ac55b8f206-cgu.0"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@vtable.0 = private unnamed_addr constant <{ [24 x i8], ptr, ptr, ptr }> <{ [24 x i8] c"\00\00\00\00\00\00\00\00\08\00\00\00\00\00\00\00\08\00\00\00\00\00\00\00", ptr @"_ZN4core3ops8function6FnOnce40call_once$u7b$$u7b$vtable.shim$u7d$$u7d$17h008cc6e2b42e2dc0E", ptr @"_ZN3std2rt10lang_start28_$u7b$$u7b$closure$u7d$$u7d$17h96d0021b3f1d3e68E", ptr @"_ZN3std2rt10lang_start28_$u7b$$u7b$closure$u7d$$u7d$17h96d0021b3f1d3e68E" }>, align 8
@alloc_b71d8f19709663b7581a3cab0ce04cff = private unnamed_addr constant <{ [20 x i8] }> <{ [20 x i8] c"src/bin/day_night.rs" }>, align 1
@alloc_ab2ea280c03751c3a9cf53a9ca2a17c0 = private unnamed_addr constant <{ ptr, [16 x i8] }> <{ ptr @alloc_b71d8f19709663b7581a3cab0ce04cff, [16 x i8] c"\14\00\00\00\00\00\00\00/\00\00\00\0C\00\00\00" }>, align 8

; std::rt::lang_start
; Function Attrs: nonlazybind uwtable
define hidden noundef i64 @_ZN3std2rt10lang_start17h5365486a3ee83af4E(ptr noundef nonnull %main, i64 noundef %argc, ptr noundef %argv, i8 noundef %sigpipe) unnamed_addr #0 {
start:
  %_8 = alloca [8 x i8], align 8
  call void @llvm.lifetime.start.p0(i64 8, ptr nonnull %_8)
  store ptr %main, ptr %_8, align 8
; call std::rt::lang_start_internal
  %0 = call noundef i64 @_ZN3std2rt19lang_start_internal17h5e7c81cecd7f0954E(ptr noundef nonnull align 1 %_8, ptr noalias noundef nonnull readonly align 8 dereferenceable(48) @vtable.0, i64 noundef %argc, ptr noundef %argv, i8 noundef %sigpipe)
  call void @llvm.lifetime.end.p0(i64 8, ptr nonnull %_8)
  ret i64 %0
}

; std::rt::lang_start::{{closure}}
; Function Attrs: inlinehint nonlazybind uwtable
define internal noundef i32 @"_ZN3std2rt10lang_start28_$u7b$$u7b$closure$u7d$$u7d$17h96d0021b3f1d3e68E"(ptr noalias nocapture noundef readonly align 8 dereferenceable(8) %_1) unnamed_addr #1 {
start:
  %_4 = load ptr, ptr %_1, align 8, !nonnull !4, !noundef !4
; call std::sys::backtrace::__rust_begin_short_backtrace
  tail call fastcc void @_ZN3std3sys9backtrace28__rust_begin_short_backtrace17h2bd1679097cec21cE(ptr noundef nonnull %_4)
  ret i32 0
}

; std::sys::backtrace::__rust_begin_short_backtrace
; Function Attrs: noinline nonlazybind uwtable
define internal fastcc void @_ZN3std3sys9backtrace28__rust_begin_short_backtrace17h2bd1679097cec21cE(ptr nocapture noundef nonnull readonly %f) unnamed_addr #2 {
start:
  tail call void %f()
  tail call void asm sideeffect "", "~{memory}"() #11, !srcloc !5
  ret void
}

; core::ops::function::FnOnce::call_once{{vtable.shim}}
; Function Attrs: inlinehint nonlazybind uwtable
define internal noundef i32 @"_ZN4core3ops8function6FnOnce40call_once$u7b$$u7b$vtable.shim$u7d$$u7d$17h008cc6e2b42e2dc0E"(ptr nocapture noundef readonly %_1) unnamed_addr #1 personality ptr @rust_eh_personality {
start:
  %0 = load ptr, ptr %_1, align 8, !nonnull !4, !noundef !4
; call std::sys::backtrace::__rust_begin_short_backtrace
  tail call fastcc void @_ZN3std3sys9backtrace28__rust_begin_short_backtrace17h2bd1679097cec21cE(ptr noundef nonnull %0), !noalias !6
  ret i32 0
}

; Function Attrs: nofree norecurse nosync nounwind nonlazybind memory(argmem: readwrite) uwtable
define dso_local void @new(ptr dead_on_unwind noalias nocapture noundef writable writeonly sret([131072 x i8]) align 1 dereferenceable(131072) %_0) unnamed_addr #3 {
start:
  %_1 = alloca [131072 x i8], align 1
  call void @llvm.lifetime.start.p0(i64 131072, ptr nonnull %_1)
  br label %repeat_loop_body

repeat_loop_body:                                 ; preds = %repeat_loop_body, %start
  %0 = phi i64 [ 0, %start ], [ %16, %repeat_loop_body ]
  %1 = getelementptr inbounds [256 x [512 x i8]], ptr %_1, i64 0, i64 %0
  call void @llvm.memset.p0.i64(ptr noundef nonnull align 1 dereferenceable(512) %1, i8 0, i64 512, i1 false)
  %2 = or disjoint i64 %0, 1
  %3 = getelementptr inbounds [256 x [512 x i8]], ptr %_1, i64 0, i64 %2
  call void @llvm.memset.p0.i64(ptr noundef nonnull align 1 dereferenceable(512) %3, i8 0, i64 512, i1 false)
  %4 = or disjoint i64 %0, 2
  %5 = getelementptr inbounds [256 x [512 x i8]], ptr %_1, i64 0, i64 %4
  call void @llvm.memset.p0.i64(ptr noundef nonnull align 1 dereferenceable(512) %5, i8 0, i64 512, i1 false)
  %6 = or disjoint i64 %0, 3
  %7 = getelementptr inbounds [256 x [512 x i8]], ptr %_1, i64 0, i64 %6
  call void @llvm.memset.p0.i64(ptr noundef nonnull align 1 dereferenceable(512) %7, i8 0, i64 512, i1 false)
  %8 = or disjoint i64 %0, 4
  %9 = getelementptr inbounds [256 x [512 x i8]], ptr %_1, i64 0, i64 %8
  call void @llvm.memset.p0.i64(ptr noundef nonnull align 1 dereferenceable(512) %9, i8 0, i64 512, i1 false)
  %10 = or disjoint i64 %0, 5
  %11 = getelementptr inbounds [256 x [512 x i8]], ptr %_1, i64 0, i64 %10
  call void @llvm.memset.p0.i64(ptr noundef nonnull align 1 dereferenceable(512) %11, i8 0, i64 512, i1 false)
  %12 = or disjoint i64 %0, 6
  %13 = getelementptr inbounds [256 x [512 x i8]], ptr %_1, i64 0, i64 %12
  call void @llvm.memset.p0.i64(ptr noundef nonnull align 1 dereferenceable(512) %13, i8 0, i64 512, i1 false)
  %14 = or disjoint i64 %0, 7
  %15 = getelementptr inbounds [256 x [512 x i8]], ptr %_1, i64 0, i64 %14
  call void @llvm.memset.p0.i64(ptr noundef nonnull align 1 dereferenceable(512) %15, i8 0, i64 512, i1 false)
  %16 = add nuw nsw i64 %0, 8
  %exitcond.not.7 = icmp eq i64 %16, 256
  br i1 %exitcond.not.7, label %repeat_loop_next, label %repeat_loop_body

repeat_loop_next:                                 ; preds = %repeat_loop_body
  call void @llvm.memcpy.p0.p0.i64(ptr noundef nonnull align 1 dereferenceable(131072) %_0, ptr noundef nonnull align 1 dereferenceable(131072) %_1, i64 131072, i1 false)
  call void @llvm.lifetime.end.p0(i64 131072, ptr nonnull %_1)
  ret void
}

; Function Attrs: nounwind nonlazybind uwtable
define dso_local void @randomly_fill(ptr noalias nocapture noundef writeonly align 1 dereferenceable(131072) %self) unnamed_addr #4 personality ptr @rust_eh_personality {
start:
  br label %bb2

bb2:                                              ; preds = %bb9, %start
  %iter.sroa.0.0 = phi i64 [ 2, %start ], [ %iter.sroa.0.113, %bb9 ]
  %iter.sroa.5.0 = phi i64 [ undef, %start ], [ %iter.sroa.5.114, %bb9 ]
  %iter.sroa.8.0 = phi i64 [ 0, %start ], [ %iter.sroa.8.215, %bb9 ]
  %iter.sroa.15.0 = phi i64 [ 0, %start ], [ %iter.sroa.15.316, %bb9 ]
  %_0.i.i.i.i = icmp ult i64 %iter.sroa.15.0, 256
  br i1 %_0.i.i.i.i, label %bb18.i, label %bb9.i

bb9.i:                                            ; preds = %bb2
  %_0.i.i.i20.i = icmp ult i64 %iter.sroa.8.0, 512
  br i1 %_0.i.i.i20.i, label %"_ZN4core6option15Option$LT$T$GT$18get_or_insert_with17ha12f119ccbbcb787E.exit.i.thread18", label %bb6

bb18.i:                                           ; preds = %bb2
  %_0.i1.i.i.i = add nuw nsw i64 %iter.sroa.15.0, 1
  %cond = icmp eq i64 %iter.sroa.0.0, 2
  br i1 %cond, label %bb1.i.i, label %bb9

bb1.i.i:                                          ; preds = %bb18.i
  %_0.i.i.i.i.i.i = icmp ult i64 %iter.sroa.8.0, 512
  br i1 %_0.i.i.i.i.i.i, label %"_ZN4core6option15Option$LT$T$GT$18get_or_insert_with17ha12f119ccbbcb787E.exit.i.thread18", label %bb6

"_ZN4core6option15Option$LT$T$GT$18get_or_insert_with17ha12f119ccbbcb787E.exit.i.thread18": ; preds = %bb9.i, %bb1.i.i
  %iter.sroa.15.1 = phi i64 [ %_0.i1.i.i.i, %bb1.i.i ], [ 1, %bb9.i ]
  %elt_b.sroa.0.031.ph.ph.i = phi i64 [ %iter.sroa.15.0, %bb1.i.i ], [ 0, %bb9.i ]
  %_0.i1.i.i.i.i.i = add nuw nsw i64 %iter.sroa.8.0, 1
  br label %bb9

bb6:                                              ; preds = %bb1.i.i, %bb9.i
  ret void

bb9:                                              ; preds = %bb18.i, %"_ZN4core6option15Option$LT$T$GT$18get_or_insert_with17ha12f119ccbbcb787E.exit.i.thread18"
  %iter.sroa.15.316 = phi i64 [ %iter.sroa.15.1, %"_ZN4core6option15Option$LT$T$GT$18get_or_insert_with17ha12f119ccbbcb787E.exit.i.thread18" ], [ %_0.i1.i.i.i, %bb18.i ]
  %iter.sroa.8.215 = phi i64 [ %_0.i1.i.i.i.i.i, %"_ZN4core6option15Option$LT$T$GT$18get_or_insert_with17ha12f119ccbbcb787E.exit.i.thread18" ], [ %iter.sroa.8.0, %bb18.i ]
  %iter.sroa.5.114 = phi i64 [ %iter.sroa.8.0, %"_ZN4core6option15Option$LT$T$GT$18get_or_insert_with17ha12f119ccbbcb787E.exit.i.thread18" ], [ %iter.sroa.5.0, %bb18.i ]
  %iter.sroa.0.113 = phi i64 [ 1, %"_ZN4core6option15Option$LT$T$GT$18get_or_insert_with17ha12f119ccbbcb787E.exit.i.thread18" ], [ %iter.sroa.0.0, %bb18.i ]
  %_4.sroa.7.1 = phi i64 [ %elt_b.sroa.0.031.ph.ph.i, %"_ZN4core6option15Option$LT$T$GT$18get_or_insert_with17ha12f119ccbbcb787E.exit.i.thread18" ], [ %iter.sroa.15.0, %bb18.i ]
  %_10 = tail call noundef i32 @sim_rand() #11
  %0 = getelementptr inbounds [256 x [512 x i8]], ptr %self, i64 0, i64 %_4.sroa.7.1, i64 %iter.sroa.5.114
  %1 = trunc i32 %_10 to i8
  %2 = and i8 %1, 1
  store i8 %2, ptr %0, align 1
  br label %bb2
}

; Function Attrs: nonlazybind uwtable
define dso_local noundef zeroext i1 @get_next_cell_state(ptr noalias nocapture noundef readonly align 1 dereferenceable(131072) %self, i64 noundef %x_center, i64 noundef %y_center) unnamed_addr #0 personality ptr @rust_eh_personality {
start:
  %_14 = trunc i64 %x_center to i32
  %_16 = trunc i64 %y_center to i32
  br label %bb2.outer

bb2.outer:                                        ; preds = %bb9, %start
  %iter.sroa.0.0.ph = phi i32 [ %iter.sroa.0.135, %bb9 ], [ 2, %start ]
  %iter.sroa.5.0.ph = phi i32 [ %iter.sroa.5.136, %bb9 ], [ undef, %start ]
  %iter.sroa.8.0.ph = phi i32 [ %iter.sroa.8.237, %bb9 ], [ -1, %start ]
  %iter.sroa.14.0.ph = phi i8 [ %iter.sroa.14.238, %bb9 ], [ 0, %start ]
  %iter.sroa.1825.0.ph = phi i32 [ %iter.sroa.1825.439, %bb9 ], [ -1, %start ]
  %iter.sroa.26.0.shrunk.ph = phi i1 [ %iter.sroa.26.441.shrunk, %bb9 ], [ false, %start ]
  %alive_neighbours.sroa.0.0.ph = phi i32 [ %11, %bb9 ], [ 0, %start ]
  br label %bb2

bb2:                                              ; preds = %bb2.outer, %bb5
  %iter.sroa.0.0 = phi i32 [ %iter.sroa.0.135, %bb5 ], [ %iter.sroa.0.0.ph, %bb2.outer ]
  %iter.sroa.5.0 = phi i32 [ %iter.sroa.5.136, %bb5 ], [ %iter.sroa.5.0.ph, %bb2.outer ]
  %iter.sroa.8.0 = phi i32 [ %iter.sroa.8.237, %bb5 ], [ %iter.sroa.8.0.ph, %bb2.outer ]
  %iter.sroa.14.0 = phi i8 [ %iter.sroa.14.238, %bb5 ], [ %iter.sroa.14.0.ph, %bb2.outer ]
  %iter.sroa.1825.0 = phi i32 [ %iter.sroa.1825.439, %bb5 ], [ %iter.sroa.1825.0.ph, %bb2.outer ]
  %iter.sroa.26.0.shrunk = phi i1 [ %iter.sroa.26.441.shrunk, %bb5 ], [ %iter.sroa.26.0.shrunk.ph, %bb2.outer ]
  %_0.i.not.i.i.i = icmp sgt i32 %iter.sroa.1825.0, 1
  %or.cond76 = select i1 %iter.sroa.26.0.shrunk, i1 true, i1 %_0.i.not.i.i.i
  br i1 %or.cond76, label %bb9.i, label %bb2.i.i.i

bb2.i.i.i:                                        ; preds = %bb2
  %_0.i3.i.i.i = icmp slt i32 %iter.sroa.1825.0, 1
  %0 = zext i1 %_0.i3.i.i.i to i32
  %spec.select = add nsw i32 %iter.sroa.1825.0, %0
  %not._0.i3.i.i.i = xor i1 %_0.i3.i.i.i, true
  %cond = icmp eq i32 %iter.sroa.0.0, 2
  br i1 %cond, label %bb1.i.i, label %bb5

bb9.i:                                            ; preds = %bb2
  %_10.not.i.i21.i = icmp ne i8 %iter.sroa.14.0, 0
  %_0.i.not.i.i28.i = icmp sgt i32 %iter.sroa.8.0, 1
  %or.cond78 = select i1 %_10.not.i.i21.i, i1 true, i1 %_0.i.not.i.i28.i
  br i1 %or.cond78, label %bb6, label %bb2.i.i29.i

bb2.i.i29.i:                                      ; preds = %bb9.i
  %_0.i3.i.i30.i = icmp slt i32 %iter.sroa.8.0, 1
  br i1 %_0.i3.i.i30.i, label %bb4.i.i32.i, label %bb5

bb4.i.i32.i:                                      ; preds = %bb2.i.i29.i
  %1 = add nsw i32 %iter.sroa.8.0, 1
  br label %bb5

bb1.i.i:                                          ; preds = %bb2.i.i.i
  %_10.not.i.i.i.i.i = icmp ne i8 %iter.sroa.14.0, 0
  %_0.i.not.i.i.i.i.i = icmp sgt i32 %iter.sroa.8.0, 1
  %or.cond79 = select i1 %_10.not.i.i.i.i.i, i1 true, i1 %_0.i.not.i.i.i.i.i
  br i1 %or.cond79, label %bb6, label %bb2.i.i.i.i.i

bb2.i.i.i.i.i:                                    ; preds = %bb1.i.i
  %_0.i3.i.i.i.i.i = icmp slt i32 %iter.sroa.8.0, 1
  br i1 %_0.i3.i.i.i.i.i, label %bb4.i.i.i.i.i, label %bb5

bb4.i.i.i.i.i:                                    ; preds = %bb2.i.i.i.i.i
  %2 = add nsw i32 %iter.sroa.8.0, 1
  br label %bb5

bb6:                                              ; preds = %bb1.i.i, %bb9.i
  %_22 = icmp ult i64 %y_center, 256
  br i1 %_22, label %bb15, label %panic

bb5:                                              ; preds = %bb2.i.i.i, %bb2.i.i.i.i.i, %bb2.i.i29.i, %bb4.i.i.i.i.i, %bb4.i.i32.i
  %iter.sroa.26.441.shrunk = phi i1 [ %not._0.i3.i.i.i, %bb2.i.i.i.i.i ], [ false, %bb2.i.i29.i ], [ %not._0.i3.i.i.i, %bb4.i.i.i.i.i ], [ false, %bb4.i.i32.i ], [ %not._0.i3.i.i.i, %bb2.i.i.i ]
  %iter.sroa.1825.439 = phi i32 [ %spec.select, %bb2.i.i.i.i.i ], [ 0, %bb2.i.i29.i ], [ %spec.select, %bb4.i.i.i.i.i ], [ 0, %bb4.i.i32.i ], [ %spec.select, %bb2.i.i.i ]
  %iter.sroa.14.238 = phi i8 [ 1, %bb2.i.i.i.i.i ], [ 1, %bb2.i.i29.i ], [ 0, %bb4.i.i.i.i.i ], [ 0, %bb4.i.i32.i ], [ %iter.sroa.14.0, %bb2.i.i.i ]
  %iter.sroa.8.237 = phi i32 [ 1, %bb2.i.i.i.i.i ], [ 1, %bb2.i.i29.i ], [ %2, %bb4.i.i.i.i.i ], [ %1, %bb4.i.i32.i ], [ %iter.sroa.8.0, %bb2.i.i.i ]
  %iter.sroa.5.136 = phi i32 [ 1, %bb2.i.i.i.i.i ], [ 1, %bb2.i.i29.i ], [ %iter.sroa.8.0, %bb4.i.i.i.i.i ], [ %iter.sroa.8.0, %bb4.i.i32.i ], [ %iter.sroa.5.0, %bb2.i.i.i ]
  %iter.sroa.0.135 = phi i32 [ 1, %bb2.i.i.i.i.i ], [ 1, %bb2.i.i29.i ], [ 1, %bb4.i.i.i.i.i ], [ 1, %bb4.i.i32.i ], [ %iter.sroa.0.0, %bb2.i.i.i ]
  %_8.sroa.8.1 = phi i32 [ %iter.sroa.1825.0, %bb2.i.i.i.i.i ], [ -1, %bb2.i.i29.i ], [ %iter.sroa.1825.0, %bb4.i.i.i.i.i ], [ -1, %bb4.i.i32.i ], [ %iter.sroa.1825.0, %bb2.i.i.i ]
  %3 = or i32 %_8.sroa.8.1, %iter.sroa.5.136
  %or.cond = icmp eq i32 %3, 0
  br i1 %or.cond, label %bb2, label %bb9

bb15:                                             ; preds = %bb6
  %_23 = icmp ult i64 %x_center, 512
  br i1 %_23, label %bb16, label %panic1

panic:                                            ; preds = %bb6
; call core::panicking::panic_bounds_check
  tail call void @_ZN4core9panicking18panic_bounds_check17h9397cb495d89a72dE(i64 noundef %y_center, i64 noundef 256, ptr noalias noundef nonnull readonly align 8 dereferenceable(24) @alloc_ab2ea280c03751c3a9cf53a9ca2a17c0) #12
  unreachable

bb16:                                             ; preds = %bb15
  %4 = getelementptr inbounds [256 x [512 x i8]], ptr %self, i64 0, i64 %y_center, i64 %x_center
  %5 = load i8, ptr %4, align 1, !range !9, !noundef !4
  %_21.not = icmp eq i8 %5, 0
  %6 = icmp ult i32 %alive_neighbours.sroa.0.0.ph, 9
  br i1 %_21.not, label %bb28, label %bb17

panic1:                                           ; preds = %bb15
; call core::panicking::panic_bounds_check
  tail call void @_ZN4core9panicking18panic_bounds_check17h9397cb495d89a72dE(i64 noundef %x_center, i64 noundef 512, ptr noalias noundef nonnull readonly align 8 dereferenceable(24) @alloc_ab2ea280c03751c3a9cf53a9ca2a17c0) #12
  unreachable

bb28:                                             ; preds = %bb16
  br i1 %6, label %switch.lookup, label %bb37

bb17:                                             ; preds = %bb16
  br i1 %6, label %switch.lookup80, label %bb37

switch.lookup:                                    ; preds = %bb28
  %switch.cast = trunc i32 %alive_neighbours.sroa.0.0.ph to i9
  %switch.downshift = lshr i9 -56, %switch.cast
  %7 = and i9 %switch.downshift, 1
  %switch.masked = icmp ne i9 %7, 0
  br label %bb37

switch.lookup80:                                  ; preds = %bb17
  %switch.cast81 = trunc i32 %alive_neighbours.sroa.0.0.ph to i9
  %switch.downshift83 = lshr i9 -40, %switch.cast81
  %8 = and i9 %switch.downshift83, 1
  %switch.masked84 = icmp ne i9 %8, 0
  br label %bb37

bb37:                                             ; preds = %bb17, %bb28, %switch.lookup80, %switch.lookup
  %_0.sroa.0.0.shrunk = phi i1 [ %switch.masked, %switch.lookup ], [ %switch.masked84, %switch.lookup80 ], [ false, %bb28 ], [ false, %bb17 ]
  ret i1 %_0.sroa.0.0.shrunk

bb9:                                              ; preds = %bb5
  %_34 = add i32 %iter.sroa.5.136, %_14
  %spec.select.i = and i32 %_34, 511
  %x = zext nneg i32 %spec.select.i to i64
  %_36 = add i32 %_8.sroa.8.1, %_16
  %spec.select.i22 = and i32 %_36, 255
  %y = zext nneg i32 %spec.select.i22 to i64
  %9 = getelementptr inbounds [256 x [512 x i8]], ptr %self, i64 0, i64 %y, i64 %x
  %10 = load i8, ptr %9, align 1, !range !9, !noundef !4
  %. = zext nneg i8 %10 to i32
  %11 = add i32 %alive_neighbours.sroa.0.0.ph, %.
  br label %bb2.outer
}

; Function Attrs: nonlazybind uwtable
define dso_local void @iterate(ptr noalias nocapture noundef writeonly align 1 dereferenceable(131072) %self, ptr noalias nocapture noundef readonly align 1 dereferenceable(131072) %other) unnamed_addr #0 personality ptr @rust_eh_personality {
start:
  br label %bb2

bb2:                                              ; preds = %bb9, %start
  %iter.sroa.0.0 = phi i64 [ 2, %start ], [ %iter.sroa.0.113, %bb9 ]
  %iter.sroa.5.0 = phi i64 [ undef, %start ], [ %iter.sroa.5.114, %bb9 ]
  %iter.sroa.8.0 = phi i64 [ 0, %start ], [ %iter.sroa.8.215, %bb9 ]
  %iter.sroa.15.0 = phi i64 [ 0, %start ], [ %iter.sroa.15.316, %bb9 ]
  %_0.i.i.i.i = icmp ult i64 %iter.sroa.15.0, 256
  br i1 %_0.i.i.i.i, label %bb18.i, label %bb9.i

bb9.i:                                            ; preds = %bb2
  %_0.i.i.i20.i = icmp ult i64 %iter.sroa.8.0, 512
  br i1 %_0.i.i.i20.i, label %"_ZN4core6option15Option$LT$T$GT$18get_or_insert_with17ha12f119ccbbcb787E.exit.i.thread18", label %bb6

bb18.i:                                           ; preds = %bb2
  %_0.i1.i.i.i = add nuw nsw i64 %iter.sroa.15.0, 1
  %cond = icmp eq i64 %iter.sroa.0.0, 2
  br i1 %cond, label %bb1.i.i, label %bb9

bb1.i.i:                                          ; preds = %bb18.i
  %_0.i.i.i.i.i.i = icmp ult i64 %iter.sroa.8.0, 512
  br i1 %_0.i.i.i.i.i.i, label %"_ZN4core6option15Option$LT$T$GT$18get_or_insert_with17ha12f119ccbbcb787E.exit.i.thread18", label %bb6

"_ZN4core6option15Option$LT$T$GT$18get_or_insert_with17ha12f119ccbbcb787E.exit.i.thread18": ; preds = %bb9.i, %bb1.i.i
  %iter.sroa.15.1 = phi i64 [ %_0.i1.i.i.i, %bb1.i.i ], [ 1, %bb9.i ]
  %elt_b.sroa.0.031.ph.ph.i = phi i64 [ %iter.sroa.15.0, %bb1.i.i ], [ 0, %bb9.i ]
  %_0.i1.i.i.i.i.i = add nuw nsw i64 %iter.sroa.8.0, 1
  br label %bb9

bb6:                                              ; preds = %bb1.i.i, %bb9.i
  ret void

bb9:                                              ; preds = %bb18.i, %"_ZN4core6option15Option$LT$T$GT$18get_or_insert_with17ha12f119ccbbcb787E.exit.i.thread18"
  %iter.sroa.15.316 = phi i64 [ %iter.sroa.15.1, %"_ZN4core6option15Option$LT$T$GT$18get_or_insert_with17ha12f119ccbbcb787E.exit.i.thread18" ], [ %_0.i1.i.i.i, %bb18.i ]
  %iter.sroa.8.215 = phi i64 [ %_0.i1.i.i.i.i.i, %"_ZN4core6option15Option$LT$T$GT$18get_or_insert_with17ha12f119ccbbcb787E.exit.i.thread18" ], [ %iter.sroa.8.0, %bb18.i ]
  %iter.sroa.5.114 = phi i64 [ %iter.sroa.8.0, %"_ZN4core6option15Option$LT$T$GT$18get_or_insert_with17ha12f119ccbbcb787E.exit.i.thread18" ], [ %iter.sroa.5.0, %bb18.i ]
  %iter.sroa.0.113 = phi i64 [ 1, %"_ZN4core6option15Option$LT$T$GT$18get_or_insert_with17ha12f119ccbbcb787E.exit.i.thread18" ], [ %iter.sroa.0.0, %bb18.i ]
  %_5.sroa.7.1 = phi i64 [ %elt_b.sroa.0.031.ph.ph.i, %"_ZN4core6option15Option$LT$T$GT$18get_or_insert_with17ha12f119ccbbcb787E.exit.i.thread18" ], [ %iter.sroa.15.0, %bb18.i ]
  %_10 = tail call noundef zeroext i1 @get_next_cell_state(ptr noalias noundef nonnull readonly align 1 dereferenceable(131072) %other, i64 noundef %iter.sroa.5.114, i64 noundef %_5.sroa.7.1)
  %0 = getelementptr inbounds [256 x [512 x i8]], ptr %self, i64 0, i64 %_5.sroa.7.1, i64 %iter.sroa.5.114
  %1 = zext i1 %_10 to i8
  store i8 %1, ptr %0, align 1
  br label %bb2
}

; Function Attrs: nounwind nonlazybind uwtable
define dso_local void @draw(ptr noalias nocapture noundef readonly align 1 dereferenceable(131072) %self) unnamed_addr #4 personality ptr @rust_eh_personality {
start:
  br label %bb2

bb2:                                              ; preds = %bb8, %start
  %iter.sroa.0.0 = phi i64 [ 2, %start ], [ %iter.sroa.0.113, %bb8 ]
  %iter.sroa.5.0 = phi i64 [ undef, %start ], [ %iter.sroa.5.114, %bb8 ]
  %iter.sroa.8.0 = phi i64 [ 0, %start ], [ %iter.sroa.8.215, %bb8 ]
  %iter.sroa.15.0 = phi i64 [ 0, %start ], [ %iter.sroa.15.316, %bb8 ]
  %_0.i.i.i.i = icmp ult i64 %iter.sroa.15.0, 256
  br i1 %_0.i.i.i.i, label %bb18.i, label %bb9.i

bb9.i:                                            ; preds = %bb2
  %_0.i.i.i20.i = icmp ult i64 %iter.sroa.8.0, 512
  br i1 %_0.i.i.i20.i, label %"_ZN4core6option15Option$LT$T$GT$18get_or_insert_with17ha12f119ccbbcb787E.exit.i.thread18", label %bb6

bb18.i:                                           ; preds = %bb2
  %_0.i1.i.i.i = add nuw nsw i64 %iter.sroa.15.0, 1
  %cond = icmp eq i64 %iter.sroa.0.0, 2
  br i1 %cond, label %bb1.i.i, label %bb8

bb1.i.i:                                          ; preds = %bb18.i
  %_0.i.i.i.i.i.i = icmp ult i64 %iter.sroa.8.0, 512
  br i1 %_0.i.i.i.i.i.i, label %"_ZN4core6option15Option$LT$T$GT$18get_or_insert_with17ha12f119ccbbcb787E.exit.i.thread18", label %bb6

"_ZN4core6option15Option$LT$T$GT$18get_or_insert_with17ha12f119ccbbcb787E.exit.i.thread18": ; preds = %bb9.i, %bb1.i.i
  %iter.sroa.15.1 = phi i64 [ %_0.i1.i.i.i, %bb1.i.i ], [ 1, %bb9.i ]
  %elt_b.sroa.0.031.ph.ph.i = phi i64 [ %iter.sroa.15.0, %bb1.i.i ], [ 0, %bb9.i ]
  %_0.i1.i.i.i.i.i = add nuw nsw i64 %iter.sroa.8.0, 1
  br label %bb8

bb6:                                              ; preds = %bb1.i.i, %bb9.i
  ret void

bb8:                                              ; preds = %bb18.i, %"_ZN4core6option15Option$LT$T$GT$18get_or_insert_with17ha12f119ccbbcb787E.exit.i.thread18"
  %iter.sroa.15.316 = phi i64 [ %iter.sroa.15.1, %"_ZN4core6option15Option$LT$T$GT$18get_or_insert_with17ha12f119ccbbcb787E.exit.i.thread18" ], [ %_0.i1.i.i.i, %bb18.i ]
  %iter.sroa.8.215 = phi i64 [ %_0.i1.i.i.i.i.i, %"_ZN4core6option15Option$LT$T$GT$18get_or_insert_with17ha12f119ccbbcb787E.exit.i.thread18" ], [ %iter.sroa.8.0, %bb18.i ]
  %iter.sroa.5.114 = phi i64 [ %iter.sroa.8.0, %"_ZN4core6option15Option$LT$T$GT$18get_or_insert_with17ha12f119ccbbcb787E.exit.i.thread18" ], [ %iter.sroa.5.0, %bb18.i ]
  %iter.sroa.0.113 = phi i64 [ 1, %"_ZN4core6option15Option$LT$T$GT$18get_or_insert_with17ha12f119ccbbcb787E.exit.i.thread18" ], [ %iter.sroa.0.0, %bb18.i ]
  %_5.sroa.7.1 = phi i64 [ %elt_b.sroa.0.031.ph.ph.i, %"_ZN4core6option15Option$LT$T$GT$18get_or_insert_with17ha12f119ccbbcb787E.exit.i.thread18" ], [ %iter.sroa.15.0, %bb18.i ]
  %_10 = trunc i64 %iter.sroa.5.114 to i16
  %_11 = trunc i64 %_5.sroa.7.1 to i16
  %0 = getelementptr inbounds [256 x [512 x i8]], ptr %self, i64 0, i64 %_5.sroa.7.1, i64 %iter.sroa.5.114
  %1 = load i8, ptr %0, align 1, !range !9, !noundef !4
  %_13.not = icmp eq i8 %1, 0
  %. = select i1 %_13.not, i32 -65536, i32 -16711936
  tail call void @sim_put_pixel(i16 noundef zeroext %_10, i16 noundef zeroext %_11, i32 noundef %.) #11
  br label %bb2
}

; day_night::main
; Function Attrs: nonlazybind uwtable
define internal void @_ZN9day_night4main17ha0c38ec631b4fa75E() unnamed_addr #0 personality ptr @rust_eh_personality {
start:
  %board2 = alloca [131072 x i8], align 1
  %board1 = alloca [131072 x i8], align 1
  tail call void @sim_init() #11
  call void @llvm.lifetime.start.p0(i64 131072, ptr nonnull %board1)
  call void @llvm.memset.p0.i64(ptr noundef nonnull align 1 dereferenceable(131072) %board1, i8 0, i64 131072, i1 false)
  call void @llvm.lifetime.start.p0(i64 131072, ptr nonnull %board2)
  call void @llvm.memset.p0.i64(ptr noundef nonnull align 1 dereferenceable(131072) %board2, i8 0, i64 131072, i1 false)
  tail call void @llvm.experimental.noalias.scope.decl(metadata !10)
  br label %bb2.i

bb2.i:                                            ; preds = %bb9.i, %start
  %iter.sroa.0.0.i = phi i64 [ 2, %start ], [ %iter.sroa.0.113.i, %bb9.i ]
  %iter.sroa.5.0.i = phi i64 [ undef, %start ], [ %iter.sroa.5.114.i, %bb9.i ]
  %iter.sroa.8.0.i = phi i64 [ 0, %start ], [ %iter.sroa.8.215.i, %bb9.i ]
  %iter.sroa.15.0.i = phi i64 [ 0, %start ], [ %iter.sroa.15.316.i, %bb9.i ]
  %_0.i.i.i.i.i = icmp ult i64 %iter.sroa.15.0.i, 256
  br i1 %_0.i.i.i.i.i, label %bb18.i.i, label %bb9.i.i

bb9.i.i:                                          ; preds = %bb2.i
  %_0.i.i.i20.i.i = icmp ult i64 %iter.sroa.8.0.i, 512
  br i1 %_0.i.i.i20.i.i, label %"_ZN4core6option15Option$LT$T$GT$18get_or_insert_with17ha12f119ccbbcb787E.exit.i.thread18.i", label %bb16.preheader

bb18.i.i:                                         ; preds = %bb2.i
  %_0.i1.i.i.i.i = add nuw nsw i64 %iter.sroa.15.0.i, 1
  %cond.i = icmp eq i64 %iter.sroa.0.0.i, 2
  br i1 %cond.i, label %bb1.i.i.i, label %bb9.i

bb1.i.i.i:                                        ; preds = %bb18.i.i
  %_0.i.i.i.i.i.i.i = icmp ult i64 %iter.sroa.8.0.i, 512
  br i1 %_0.i.i.i.i.i.i.i, label %"_ZN4core6option15Option$LT$T$GT$18get_or_insert_with17ha12f119ccbbcb787E.exit.i.thread18.i", label %bb16.preheader

bb16.preheader:                                   ; preds = %bb9.i.i, %bb1.i.i.i
  br label %bb16

"_ZN4core6option15Option$LT$T$GT$18get_or_insert_with17ha12f119ccbbcb787E.exit.i.thread18.i": ; preds = %bb1.i.i.i, %bb9.i.i
  %iter.sroa.15.1.i = phi i64 [ %_0.i1.i.i.i.i, %bb1.i.i.i ], [ 1, %bb9.i.i ]
  %elt_b.sroa.0.031.ph.ph.i.i = phi i64 [ %iter.sroa.15.0.i, %bb1.i.i.i ], [ 0, %bb9.i.i ]
  %_0.i1.i.i.i.i.i.i = add nuw nsw i64 %iter.sroa.8.0.i, 1
  br label %bb9.i

bb9.i:                                            ; preds = %"_ZN4core6option15Option$LT$T$GT$18get_or_insert_with17ha12f119ccbbcb787E.exit.i.thread18.i", %bb18.i.i
  %iter.sroa.15.316.i = phi i64 [ %iter.sroa.15.1.i, %"_ZN4core6option15Option$LT$T$GT$18get_or_insert_with17ha12f119ccbbcb787E.exit.i.thread18.i" ], [ %_0.i1.i.i.i.i, %bb18.i.i ]
  %iter.sroa.8.215.i = phi i64 [ %_0.i1.i.i.i.i.i.i, %"_ZN4core6option15Option$LT$T$GT$18get_or_insert_with17ha12f119ccbbcb787E.exit.i.thread18.i" ], [ %iter.sroa.8.0.i, %bb18.i.i ]
  %iter.sroa.5.114.i = phi i64 [ %iter.sroa.8.0.i, %"_ZN4core6option15Option$LT$T$GT$18get_or_insert_with17ha12f119ccbbcb787E.exit.i.thread18.i" ], [ %iter.sroa.5.0.i, %bb18.i.i ]
  %iter.sroa.0.113.i = phi i64 [ 1, %"_ZN4core6option15Option$LT$T$GT$18get_or_insert_with17ha12f119ccbbcb787E.exit.i.thread18.i" ], [ %iter.sroa.0.0.i, %bb18.i.i ]
  %_4.sroa.7.1.i = phi i64 [ %elt_b.sroa.0.031.ph.ph.i.i, %"_ZN4core6option15Option$LT$T$GT$18get_or_insert_with17ha12f119ccbbcb787E.exit.i.thread18.i" ], [ %iter.sroa.15.0.i, %bb18.i.i ]
  %_10.i = tail call noundef i32 @sim_rand() #11, !noalias !10
  %0 = getelementptr inbounds [256 x [512 x i8]], ptr %board1, i64 0, i64 %_4.sroa.7.1.i, i64 %iter.sroa.5.114.i
  %1 = trunc i32 %_10.i to i8
  %2 = and i8 %1, 1
  store i8 %2, ptr %0, align 1, !alias.scope !10
  br label %bb2.i

bb18:                                             ; preds = %bb14
  tail call void @sim_exit() #11
  call void @llvm.lifetime.end.p0(i64 131072, ptr nonnull %board2)
  call void @llvm.lifetime.end.p0(i64 131072, ptr nonnull %board1)
  ret void

bb16:                                             ; preds = %bb16.preheader, %bb14
  %board_index.sroa.0.0190 = phi i32 [ %36, %bb14 ], [ 0, %bb16.preheader ]
  %iter.sroa.0.0189 = phi i32 [ %3, %bb14 ], [ 0, %bb16.preheader ]
  %3 = add nuw nsw i32 %iter.sroa.0.0189, 1
  %4 = icmp eq i32 %board_index.sroa.0.0190, 0
  br i1 %4, label %bb6, label %bb10

bb6:                                              ; preds = %bb16
  tail call void @llvm.experimental.noalias.scope.decl(metadata !13)
  br label %bb2.i7

bb2.i7:                                           ; preds = %bb8.i, %bb6
  %iter.sroa.0.0.i8 = phi i64 [ 2, %bb6 ], [ %iter.sroa.0.113.i22, %bb8.i ]
  %iter.sroa.5.0.i9 = phi i64 [ undef, %bb6 ], [ %iter.sroa.5.114.i21, %bb8.i ]
  %iter.sroa.8.0.i10 = phi i64 [ 0, %bb6 ], [ %iter.sroa.8.215.i20, %bb8.i ]
  %iter.sroa.15.0.i11 = phi i64 [ 0, %bb6 ], [ %iter.sroa.15.316.i19, %bb8.i ]
  %_0.i.i.i.i.i12 = icmp ult i64 %iter.sroa.15.0.i11, 256
  br i1 %_0.i.i.i.i.i12, label %bb18.i.i24, label %bb9.i.i13

bb9.i.i13:                                        ; preds = %bb2.i7
  %_0.i.i.i20.i.i14 = icmp ult i64 %iter.sroa.8.0.i10, 512
  br i1 %_0.i.i.i20.i.i14, label %"_ZN4core6option15Option$LT$T$GT$18get_or_insert_with17ha12f119ccbbcb787E.exit.i.thread18.i15", label %draw.exit

bb18.i.i24:                                       ; preds = %bb2.i7
  %_0.i1.i.i.i.i25 = add nuw nsw i64 %iter.sroa.15.0.i11, 1
  %cond.i26 = icmp eq i64 %iter.sroa.0.0.i8, 2
  br i1 %cond.i26, label %bb1.i.i.i27, label %bb8.i

bb1.i.i.i27:                                      ; preds = %bb18.i.i24
  %_0.i.i.i.i.i.i.i28 = icmp ult i64 %iter.sroa.8.0.i10, 512
  br i1 %_0.i.i.i.i.i.i.i28, label %"_ZN4core6option15Option$LT$T$GT$18get_or_insert_with17ha12f119ccbbcb787E.exit.i.thread18.i15", label %draw.exit

"_ZN4core6option15Option$LT$T$GT$18get_or_insert_with17ha12f119ccbbcb787E.exit.i.thread18.i15": ; preds = %bb1.i.i.i27, %bb9.i.i13
  %iter.sroa.15.1.i16 = phi i64 [ %_0.i1.i.i.i.i25, %bb1.i.i.i27 ], [ 1, %bb9.i.i13 ]
  %elt_b.sroa.0.031.ph.ph.i.i17 = phi i64 [ %iter.sroa.15.0.i11, %bb1.i.i.i27 ], [ 0, %bb9.i.i13 ]
  %_0.i1.i.i.i.i.i.i18 = add nuw nsw i64 %iter.sroa.8.0.i10, 1
  br label %bb8.i

bb8.i:                                            ; preds = %"_ZN4core6option15Option$LT$T$GT$18get_or_insert_with17ha12f119ccbbcb787E.exit.i.thread18.i15", %bb18.i.i24
  %iter.sroa.15.316.i19 = phi i64 [ %iter.sroa.15.1.i16, %"_ZN4core6option15Option$LT$T$GT$18get_or_insert_with17ha12f119ccbbcb787E.exit.i.thread18.i15" ], [ %_0.i1.i.i.i.i25, %bb18.i.i24 ]
  %iter.sroa.8.215.i20 = phi i64 [ %_0.i1.i.i.i.i.i.i18, %"_ZN4core6option15Option$LT$T$GT$18get_or_insert_with17ha12f119ccbbcb787E.exit.i.thread18.i15" ], [ %iter.sroa.8.0.i10, %bb18.i.i24 ]
  %iter.sroa.5.114.i21 = phi i64 [ %iter.sroa.8.0.i10, %"_ZN4core6option15Option$LT$T$GT$18get_or_insert_with17ha12f119ccbbcb787E.exit.i.thread18.i15" ], [ %iter.sroa.5.0.i9, %bb18.i.i24 ]
  %iter.sroa.0.113.i22 = phi i64 [ 1, %"_ZN4core6option15Option$LT$T$GT$18get_or_insert_with17ha12f119ccbbcb787E.exit.i.thread18.i15" ], [ %iter.sroa.0.0.i8, %bb18.i.i24 ]
  %_5.sroa.7.1.i = phi i64 [ %elt_b.sroa.0.031.ph.ph.i.i17, %"_ZN4core6option15Option$LT$T$GT$18get_or_insert_with17ha12f119ccbbcb787E.exit.i.thread18.i15" ], [ %iter.sroa.15.0.i11, %bb18.i.i24 ]
  %_10.i23 = trunc i64 %iter.sroa.5.114.i21 to i16
  %_11.i = trunc i64 %_5.sroa.7.1.i to i16
  %5 = getelementptr inbounds [256 x [512 x i8]], ptr %board1, i64 0, i64 %_5.sroa.7.1.i, i64 %iter.sroa.5.114.i21
  %6 = load i8, ptr %5, align 1, !range !9, !alias.scope !13, !noundef !4
  %_13.not.i = icmp eq i8 %6, 0
  %..i = select i1 %_13.not.i, i32 -65536, i32 -16711936
  tail call void @sim_put_pixel(i16 noundef zeroext %_10.i23, i16 noundef zeroext %_11.i, i32 noundef %..i) #11, !noalias !13
  br label %bb2.i7

draw.exit:                                        ; preds = %bb9.i.i13, %bb1.i.i.i27
  tail call void @sim_flush() #11
  tail call void @llvm.experimental.noalias.scope.decl(metadata !16)
  br label %bb2.i29

bb2.i29:                                          ; preds = %get_next_cell_state.exit, %draw.exit
  %iter.sroa.0.0.i30 = phi i64 [ 2, %draw.exit ], [ %iter.sroa.0.113.i45, %get_next_cell_state.exit ]
  %iter.sroa.5.0.i31 = phi i64 [ undef, %draw.exit ], [ %iter.sroa.5.114.i44, %get_next_cell_state.exit ]
  %iter.sroa.8.0.i32 = phi i64 [ 0, %draw.exit ], [ %iter.sroa.8.215.i43, %get_next_cell_state.exit ]
  %iter.sroa.15.0.i33 = phi i64 [ 0, %draw.exit ], [ %iter.sroa.15.316.i42, %get_next_cell_state.exit ]
  %_0.i.i.i.i.i34 = icmp ult i64 %iter.sroa.15.0.i33, 256
  br i1 %_0.i.i.i.i.i34, label %bb18.i.i48, label %bb9.i.i35

bb9.i.i35:                                        ; preds = %bb2.i29
  %_0.i.i.i20.i.i36 = icmp ult i64 %iter.sroa.8.0.i32, 512
  br i1 %_0.i.i.i20.i.i36, label %"_ZN4core6option15Option$LT$T$GT$18get_or_insert_with17ha12f119ccbbcb787E.exit.i.thread18.i37", label %bb14

bb18.i.i48:                                       ; preds = %bb2.i29
  %_0.i1.i.i.i.i49 = add nuw nsw i64 %iter.sroa.15.0.i33, 1
  %cond.i50 = icmp eq i64 %iter.sroa.0.0.i30, 2
  br i1 %cond.i50, label %bb1.i.i.i51, label %bb9.i41

bb1.i.i.i51:                                      ; preds = %bb18.i.i48
  %_0.i.i.i.i.i.i.i52 = icmp ult i64 %iter.sroa.8.0.i32, 512
  br i1 %_0.i.i.i.i.i.i.i52, label %"_ZN4core6option15Option$LT$T$GT$18get_or_insert_with17ha12f119ccbbcb787E.exit.i.thread18.i37", label %bb14

"_ZN4core6option15Option$LT$T$GT$18get_or_insert_with17ha12f119ccbbcb787E.exit.i.thread18.i37": ; preds = %bb1.i.i.i51, %bb9.i.i35
  %iter.sroa.15.1.i38 = phi i64 [ %_0.i1.i.i.i.i49, %bb1.i.i.i51 ], [ 1, %bb9.i.i35 ]
  %elt_b.sroa.0.031.ph.ph.i.i39 = phi i64 [ %iter.sroa.15.0.i33, %bb1.i.i.i51 ], [ 0, %bb9.i.i35 ]
  %_0.i1.i.i.i.i.i.i40 = add nuw nsw i64 %iter.sroa.8.0.i32, 1
  br label %bb9.i41

bb9.i41:                                          ; preds = %"_ZN4core6option15Option$LT$T$GT$18get_or_insert_with17ha12f119ccbbcb787E.exit.i.thread18.i37", %bb18.i.i48
  %iter.sroa.15.316.i42 = phi i64 [ %iter.sroa.15.1.i38, %"_ZN4core6option15Option$LT$T$GT$18get_or_insert_with17ha12f119ccbbcb787E.exit.i.thread18.i37" ], [ %_0.i1.i.i.i.i49, %bb18.i.i48 ]
  %iter.sroa.8.215.i43 = phi i64 [ %_0.i1.i.i.i.i.i.i40, %"_ZN4core6option15Option$LT$T$GT$18get_or_insert_with17ha12f119ccbbcb787E.exit.i.thread18.i37" ], [ %iter.sroa.8.0.i32, %bb18.i.i48 ]
  %iter.sroa.5.114.i44 = phi i64 [ %iter.sroa.8.0.i32, %"_ZN4core6option15Option$LT$T$GT$18get_or_insert_with17ha12f119ccbbcb787E.exit.i.thread18.i37" ], [ %iter.sroa.5.0.i31, %bb18.i.i48 ]
  %iter.sroa.0.113.i45 = phi i64 [ 1, %"_ZN4core6option15Option$LT$T$GT$18get_or_insert_with17ha12f119ccbbcb787E.exit.i.thread18.i37" ], [ %iter.sroa.0.0.i30, %bb18.i.i48 ]
  %_5.sroa.7.1.i46 = phi i64 [ %elt_b.sroa.0.031.ph.ph.i.i39, %"_ZN4core6option15Option$LT$T$GT$18get_or_insert_with17ha12f119ccbbcb787E.exit.i.thread18.i37" ], [ %iter.sroa.15.0.i33, %bb18.i.i48 ]
  tail call void @llvm.experimental.noalias.scope.decl(metadata !19)
  %_14.i = trunc i64 %iter.sroa.5.114.i44 to i32
  %_16.i = trunc i64 %_5.sroa.7.1.i46 to i32
  br label %bb2.outer.i

bb2.outer.i:                                      ; preds = %bb9.i111, %bb9.i41
  %iter.sroa.0.0.ph.i = phi i32 [ %iter.sroa.0.135.i, %bb9.i111 ], [ 2, %bb9.i41 ]
  %iter.sroa.5.0.ph.i = phi i32 [ %iter.sroa.5.136.i, %bb9.i111 ], [ undef, %bb9.i41 ]
  %iter.sroa.8.0.ph.i = phi i32 [ %iter.sroa.8.237.i, %bb9.i111 ], [ -1, %bb9.i41 ]
  %iter.sroa.14.0.ph.i = phi i8 [ %iter.sroa.14.238.i, %bb9.i111 ], [ 0, %bb9.i41 ]
  %iter.sroa.1825.0.ph.i = phi i32 [ %iter.sroa.1825.439.i, %bb9.i111 ], [ -1, %bb9.i41 ]
  %iter.sroa.26.0.shrunk.ph.i = phi i1 [ %iter.sroa.26.441.shrunk.i, %bb9.i111 ], [ false, %bb9.i41 ]
  %alive_neighbours.sroa.0.0.ph.i = phi i32 [ %16, %bb9.i111 ], [ 0, %bb9.i41 ]
  br label %bb2.i106

bb2.i106:                                         ; preds = %bb5.i, %bb2.outer.i
  %iter.sroa.0.0.i107 = phi i32 [ %iter.sroa.0.135.i, %bb5.i ], [ %iter.sroa.0.0.ph.i, %bb2.outer.i ]
  %iter.sroa.5.0.i108 = phi i32 [ %iter.sroa.5.136.i, %bb5.i ], [ %iter.sroa.5.0.ph.i, %bb2.outer.i ]
  %iter.sroa.8.0.i109 = phi i32 [ %iter.sroa.8.237.i, %bb5.i ], [ %iter.sroa.8.0.ph.i, %bb2.outer.i ]
  %iter.sroa.14.0.i = phi i8 [ %iter.sroa.14.238.i, %bb5.i ], [ %iter.sroa.14.0.ph.i, %bb2.outer.i ]
  %iter.sroa.1825.0.i = phi i32 [ %iter.sroa.1825.439.i, %bb5.i ], [ %iter.sroa.1825.0.ph.i, %bb2.outer.i ]
  %iter.sroa.26.0.shrunk.i = phi i1 [ %iter.sroa.26.441.shrunk.i, %bb5.i ], [ %iter.sroa.26.0.shrunk.ph.i, %bb2.outer.i ]
  %_0.i.not.i.i.i.i = icmp sgt i32 %iter.sroa.1825.0.i, 1
  %or.cond76.i = select i1 %iter.sroa.26.0.shrunk.i, i1 true, i1 %_0.i.not.i.i.i.i
  br i1 %or.cond76.i, label %bb9.i.i114, label %bb2.i.i.i.i

bb2.i.i.i.i:                                      ; preds = %bb2.i106
  %_0.i3.i.i.i.i = icmp slt i32 %iter.sroa.1825.0.i, 1
  %7 = zext i1 %_0.i3.i.i.i.i to i32
  %spec.select.i = add nsw i32 %iter.sroa.1825.0.i, %7
  %not._0.i3.i.i.i.i = xor i1 %_0.i3.i.i.i.i, true
  %cond.i110 = icmp eq i32 %iter.sroa.0.0.i107, 2
  br i1 %cond.i110, label %bb1.i.i.i113, label %bb5.i

bb9.i.i114:                                       ; preds = %bb2.i106
  %_10.not.i.i21.i.i = icmp ne i8 %iter.sroa.14.0.i, 0
  %_0.i.not.i.i28.i.i = icmp sgt i32 %iter.sroa.8.0.i109, 1
  %or.cond78.i = select i1 %_10.not.i.i21.i.i, i1 true, i1 %_0.i.not.i.i28.i.i
  br i1 %or.cond78.i, label %bb6.i, label %bb2.i.i29.i.i

bb2.i.i29.i.i:                                    ; preds = %bb9.i.i114
  %_0.i3.i.i30.i.i = icmp slt i32 %iter.sroa.8.0.i109, 1
  br i1 %_0.i3.i.i30.i.i, label %bb4.i.i32.i.i, label %bb5.i

bb4.i.i32.i.i:                                    ; preds = %bb2.i.i29.i.i
  %8 = add nsw i32 %iter.sroa.8.0.i109, 1
  br label %bb5.i

bb1.i.i.i113:                                     ; preds = %bb2.i.i.i.i
  %_10.not.i.i.i.i.i.i = icmp ne i8 %iter.sroa.14.0.i, 0
  %_0.i.not.i.i.i.i.i.i = icmp sgt i32 %iter.sroa.8.0.i109, 1
  %or.cond79.i = select i1 %_10.not.i.i.i.i.i.i, i1 true, i1 %_0.i.not.i.i.i.i.i.i
  br i1 %or.cond79.i, label %bb6.i, label %bb2.i.i.i.i.i.i

bb2.i.i.i.i.i.i:                                  ; preds = %bb1.i.i.i113
  %_0.i3.i.i.i.i.i.i = icmp slt i32 %iter.sroa.8.0.i109, 1
  br i1 %_0.i3.i.i.i.i.i.i, label %bb4.i.i.i.i.i.i, label %bb5.i

bb4.i.i.i.i.i.i:                                  ; preds = %bb2.i.i.i.i.i.i
  %9 = add nsw i32 %iter.sroa.8.0.i109, 1
  br label %bb5.i

bb6.i:                                            ; preds = %bb1.i.i.i113, %bb9.i.i114
  %_22.i = icmp ult i64 %_5.sroa.7.1.i46, 256
  br i1 %_22.i, label %bb15.i, label %panic.i

bb5.i:                                            ; preds = %bb4.i.i.i.i.i.i, %bb2.i.i.i.i.i.i, %bb4.i.i32.i.i, %bb2.i.i29.i.i, %bb2.i.i.i.i
  %iter.sroa.26.441.shrunk.i = phi i1 [ %not._0.i3.i.i.i.i, %bb2.i.i.i.i.i.i ], [ false, %bb2.i.i29.i.i ], [ %not._0.i3.i.i.i.i, %bb4.i.i.i.i.i.i ], [ false, %bb4.i.i32.i.i ], [ %not._0.i3.i.i.i.i, %bb2.i.i.i.i ]
  %iter.sroa.1825.439.i = phi i32 [ %spec.select.i, %bb2.i.i.i.i.i.i ], [ 0, %bb2.i.i29.i.i ], [ %spec.select.i, %bb4.i.i.i.i.i.i ], [ 0, %bb4.i.i32.i.i ], [ %spec.select.i, %bb2.i.i.i.i ]
  %iter.sroa.14.238.i = phi i8 [ 1, %bb2.i.i.i.i.i.i ], [ 1, %bb2.i.i29.i.i ], [ 0, %bb4.i.i.i.i.i.i ], [ 0, %bb4.i.i32.i.i ], [ %iter.sroa.14.0.i, %bb2.i.i.i.i ]
  %iter.sroa.8.237.i = phi i32 [ 1, %bb2.i.i.i.i.i.i ], [ 1, %bb2.i.i29.i.i ], [ %9, %bb4.i.i.i.i.i.i ], [ %8, %bb4.i.i32.i.i ], [ %iter.sroa.8.0.i109, %bb2.i.i.i.i ]
  %iter.sroa.5.136.i = phi i32 [ 1, %bb2.i.i.i.i.i.i ], [ 1, %bb2.i.i29.i.i ], [ %iter.sroa.8.0.i109, %bb4.i.i.i.i.i.i ], [ %iter.sroa.8.0.i109, %bb4.i.i32.i.i ], [ %iter.sroa.5.0.i108, %bb2.i.i.i.i ]
  %iter.sroa.0.135.i = phi i32 [ 1, %bb2.i.i.i.i.i.i ], [ 1, %bb2.i.i29.i.i ], [ 1, %bb4.i.i.i.i.i.i ], [ 1, %bb4.i.i32.i.i ], [ %iter.sroa.0.0.i107, %bb2.i.i.i.i ]
  %_8.sroa.8.1.i = phi i32 [ %iter.sroa.1825.0.i, %bb2.i.i.i.i.i.i ], [ -1, %bb2.i.i29.i.i ], [ %iter.sroa.1825.0.i, %bb4.i.i.i.i.i.i ], [ -1, %bb4.i.i32.i.i ], [ %iter.sroa.1825.0.i, %bb2.i.i.i.i ]
  %10 = or i32 %_8.sroa.8.1.i, %iter.sroa.5.136.i
  %or.cond.i = icmp eq i32 %10, 0
  br i1 %or.cond.i, label %bb2.i106, label %bb9.i111

bb15.i:                                           ; preds = %bb6.i
  %_23.i = icmp ult i64 %iter.sroa.5.114.i44, 512
  br i1 %_23.i, label %bb16.i, label %panic1.i

panic.i:                                          ; preds = %bb6.i
; call core::panicking::panic_bounds_check
  tail call void @_ZN4core9panicking18panic_bounds_check17h9397cb495d89a72dE(i64 noundef %_5.sroa.7.1.i46, i64 noundef 256, ptr noalias noundef nonnull readonly align 8 dereferenceable(24) @alloc_ab2ea280c03751c3a9cf53a9ca2a17c0) #12, !noalias !22
  unreachable

bb16.i:                                           ; preds = %bb15.i
  %11 = getelementptr inbounds [256 x [512 x i8]], ptr %board1, i64 0, i64 %_5.sroa.7.1.i46, i64 %iter.sroa.5.114.i44
  %12 = load i8, ptr %11, align 1, !range !9, !alias.scope !19, !noalias !16, !noundef !4
  %_21.not.i = icmp eq i8 %12, 0
  %switch.tableidx = add i32 %alive_neighbours.sroa.0.0.ph.i, -3
  %13 = icmp ult i32 %switch.tableidx, 6
  br i1 %_21.not.i, label %bb28.i, label %bb17.i

panic1.i:                                         ; preds = %bb15.i
; call core::panicking::panic_bounds_check
  tail call void @_ZN4core9panicking18panic_bounds_check17h9397cb495d89a72dE(i64 noundef %iter.sroa.5.114.i44, i64 noundef 512, ptr noalias noundef nonnull readonly align 8 dereferenceable(24) @alloc_ab2ea280c03751c3a9cf53a9ca2a17c0) #12, !noalias !22
  unreachable

bb28.i:                                           ; preds = %bb16.i
  br i1 %13, label %switch.lookup, label %get_next_cell_state.exit

bb17.i:                                           ; preds = %bb16.i
  br i1 %13, label %switch.lookup206, label %get_next_cell_state.exit

bb9.i111:                                         ; preds = %bb5.i
  %_34.i = add i32 %iter.sroa.5.136.i, %_14.i
  %spec.select.i.i = and i32 %_34.i, 511
  %x.i = zext nneg i32 %spec.select.i.i to i64
  %_36.i = add i32 %_8.sroa.8.1.i, %_16.i
  %spec.select.i22.i = and i32 %_36.i, 255
  %y.i = zext nneg i32 %spec.select.i22.i to i64
  %14 = getelementptr inbounds [256 x [512 x i8]], ptr %board1, i64 0, i64 %y.i, i64 %x.i
  %15 = load i8, ptr %14, align 1, !range !9, !alias.scope !19, !noalias !16, !noundef !4
  %..i112 = zext nneg i8 %15 to i32
  %16 = add i32 %alive_neighbours.sroa.0.0.ph.i, %..i112
  br label %bb2.outer.i

switch.lookup:                                    ; preds = %bb28.i
  %17 = shl nuw nsw i32 %switch.tableidx, 3
  %switch.shiftamt = zext nneg i32 %17 to i48
  %switch.downshift = lshr i48 1103823372289, %switch.shiftamt
  %switch.masked = trunc i48 %switch.downshift to i8
  br label %get_next_cell_state.exit

switch.lookup206:                                 ; preds = %bb17.i
  %18 = shl nuw nsw i32 %switch.tableidx, 3
  %switch.shiftamt209 = zext nneg i32 %18 to i48
  %switch.downshift210 = lshr i48 1103823372545, %switch.shiftamt209
  %switch.masked211 = trunc i48 %switch.downshift210 to i8
  br label %get_next_cell_state.exit

get_next_cell_state.exit:                         ; preds = %bb17.i, %bb28.i, %switch.lookup206, %switch.lookup
  %_0.sroa.0.0.shrunk.i = phi i8 [ %switch.masked, %switch.lookup ], [ %switch.masked211, %switch.lookup206 ], [ 0, %bb28.i ], [ 0, %bb17.i ]
  %19 = getelementptr inbounds [256 x [512 x i8]], ptr %board2, i64 0, i64 %_5.sroa.7.1.i46, i64 %iter.sroa.5.114.i44
  store i8 %_0.sroa.0.0.shrunk.i, ptr %19, align 1, !alias.scope !16, !noalias !23
  br label %bb2.i29

bb10:                                             ; preds = %bb16
  tail call void @llvm.experimental.noalias.scope.decl(metadata !25)
  br label %bb2.i53

bb2.i53:                                          ; preds = %bb8.i65, %bb10
  %iter.sroa.0.0.i54 = phi i64 [ 2, %bb10 ], [ %iter.sroa.0.113.i69, %bb8.i65 ]
  %iter.sroa.5.0.i55 = phi i64 [ undef, %bb10 ], [ %iter.sroa.5.114.i68, %bb8.i65 ]
  %iter.sroa.8.0.i56 = phi i64 [ 0, %bb10 ], [ %iter.sroa.8.215.i67, %bb8.i65 ]
  %iter.sroa.15.0.i57 = phi i64 [ 0, %bb10 ], [ %iter.sroa.15.316.i66, %bb8.i65 ]
  %_0.i.i.i.i.i58 = icmp ult i64 %iter.sroa.15.0.i57, 256
  br i1 %_0.i.i.i.i.i58, label %bb18.i.i75, label %bb9.i.i59

bb9.i.i59:                                        ; preds = %bb2.i53
  %_0.i.i.i20.i.i60 = icmp ult i64 %iter.sroa.8.0.i56, 512
  br i1 %_0.i.i.i20.i.i60, label %"_ZN4core6option15Option$LT$T$GT$18get_or_insert_with17ha12f119ccbbcb787E.exit.i.thread18.i61", label %draw.exit80

bb18.i.i75:                                       ; preds = %bb2.i53
  %_0.i1.i.i.i.i76 = add nuw nsw i64 %iter.sroa.15.0.i57, 1
  %cond.i77 = icmp eq i64 %iter.sroa.0.0.i54, 2
  br i1 %cond.i77, label %bb1.i.i.i78, label %bb8.i65

bb1.i.i.i78:                                      ; preds = %bb18.i.i75
  %_0.i.i.i.i.i.i.i79 = icmp ult i64 %iter.sroa.8.0.i56, 512
  br i1 %_0.i.i.i.i.i.i.i79, label %"_ZN4core6option15Option$LT$T$GT$18get_or_insert_with17ha12f119ccbbcb787E.exit.i.thread18.i61", label %draw.exit80

"_ZN4core6option15Option$LT$T$GT$18get_or_insert_with17ha12f119ccbbcb787E.exit.i.thread18.i61": ; preds = %bb1.i.i.i78, %bb9.i.i59
  %iter.sroa.15.1.i62 = phi i64 [ %_0.i1.i.i.i.i76, %bb1.i.i.i78 ], [ 1, %bb9.i.i59 ]
  %elt_b.sroa.0.031.ph.ph.i.i63 = phi i64 [ %iter.sroa.15.0.i57, %bb1.i.i.i78 ], [ 0, %bb9.i.i59 ]
  %_0.i1.i.i.i.i.i.i64 = add nuw nsw i64 %iter.sroa.8.0.i56, 1
  br label %bb8.i65

bb8.i65:                                          ; preds = %"_ZN4core6option15Option$LT$T$GT$18get_or_insert_with17ha12f119ccbbcb787E.exit.i.thread18.i61", %bb18.i.i75
  %iter.sroa.15.316.i66 = phi i64 [ %iter.sroa.15.1.i62, %"_ZN4core6option15Option$LT$T$GT$18get_or_insert_with17ha12f119ccbbcb787E.exit.i.thread18.i61" ], [ %_0.i1.i.i.i.i76, %bb18.i.i75 ]
  %iter.sroa.8.215.i67 = phi i64 [ %_0.i1.i.i.i.i.i.i64, %"_ZN4core6option15Option$LT$T$GT$18get_or_insert_with17ha12f119ccbbcb787E.exit.i.thread18.i61" ], [ %iter.sroa.8.0.i56, %bb18.i.i75 ]
  %iter.sroa.5.114.i68 = phi i64 [ %iter.sroa.8.0.i56, %"_ZN4core6option15Option$LT$T$GT$18get_or_insert_with17ha12f119ccbbcb787E.exit.i.thread18.i61" ], [ %iter.sroa.5.0.i55, %bb18.i.i75 ]
  %iter.sroa.0.113.i69 = phi i64 [ 1, %"_ZN4core6option15Option$LT$T$GT$18get_or_insert_with17ha12f119ccbbcb787E.exit.i.thread18.i61" ], [ %iter.sroa.0.0.i54, %bb18.i.i75 ]
  %_5.sroa.7.1.i70 = phi i64 [ %elt_b.sroa.0.031.ph.ph.i.i63, %"_ZN4core6option15Option$LT$T$GT$18get_or_insert_with17ha12f119ccbbcb787E.exit.i.thread18.i61" ], [ %iter.sroa.15.0.i57, %bb18.i.i75 ]
  %_10.i71 = trunc i64 %iter.sroa.5.114.i68 to i16
  %_11.i72 = trunc i64 %_5.sroa.7.1.i70 to i16
  %20 = getelementptr inbounds [256 x [512 x i8]], ptr %board2, i64 0, i64 %_5.sroa.7.1.i70, i64 %iter.sroa.5.114.i68
  %21 = load i8, ptr %20, align 1, !range !9, !alias.scope !25, !noundef !4
  %_13.not.i73 = icmp eq i8 %21, 0
  %..i74 = select i1 %_13.not.i73, i32 -65536, i32 -16711936
  tail call void @sim_put_pixel(i16 noundef zeroext %_10.i71, i16 noundef zeroext %_11.i72, i32 noundef %..i74) #11, !noalias !25
  br label %bb2.i53

draw.exit80:                                      ; preds = %bb9.i.i59, %bb1.i.i.i78
  tail call void @sim_flush() #11
  tail call void @llvm.experimental.noalias.scope.decl(metadata !28)
  br label %bb2.i81

bb2.i81:                                          ; preds = %get_next_cell_state.exit183, %draw.exit80
  %iter.sroa.0.0.i82 = phi i64 [ 2, %draw.exit80 ], [ %iter.sroa.0.113.i97, %get_next_cell_state.exit183 ]
  %iter.sroa.5.0.i83 = phi i64 [ undef, %draw.exit80 ], [ %iter.sroa.5.114.i96, %get_next_cell_state.exit183 ]
  %iter.sroa.8.0.i84 = phi i64 [ 0, %draw.exit80 ], [ %iter.sroa.8.215.i95, %get_next_cell_state.exit183 ]
  %iter.sroa.15.0.i85 = phi i64 [ 0, %draw.exit80 ], [ %iter.sroa.15.316.i94, %get_next_cell_state.exit183 ]
  %_0.i.i.i.i.i86 = icmp ult i64 %iter.sroa.15.0.i85, 256
  br i1 %_0.i.i.i.i.i86, label %bb18.i.i100, label %bb9.i.i87

bb9.i.i87:                                        ; preds = %bb2.i81
  %_0.i.i.i20.i.i88 = icmp ult i64 %iter.sroa.8.0.i84, 512
  br i1 %_0.i.i.i20.i.i88, label %"_ZN4core6option15Option$LT$T$GT$18get_or_insert_with17ha12f119ccbbcb787E.exit.i.thread18.i89", label %bb14

bb18.i.i100:                                      ; preds = %bb2.i81
  %_0.i1.i.i.i.i101 = add nuw nsw i64 %iter.sroa.15.0.i85, 1
  %cond.i102 = icmp eq i64 %iter.sroa.0.0.i82, 2
  br i1 %cond.i102, label %bb1.i.i.i103, label %bb9.i93

bb1.i.i.i103:                                     ; preds = %bb18.i.i100
  %_0.i.i.i.i.i.i.i104 = icmp ult i64 %iter.sroa.8.0.i84, 512
  br i1 %_0.i.i.i.i.i.i.i104, label %"_ZN4core6option15Option$LT$T$GT$18get_or_insert_with17ha12f119ccbbcb787E.exit.i.thread18.i89", label %bb14

"_ZN4core6option15Option$LT$T$GT$18get_or_insert_with17ha12f119ccbbcb787E.exit.i.thread18.i89": ; preds = %bb1.i.i.i103, %bb9.i.i87
  %iter.sroa.15.1.i90 = phi i64 [ %_0.i1.i.i.i.i101, %bb1.i.i.i103 ], [ 1, %bb9.i.i87 ]
  %elt_b.sroa.0.031.ph.ph.i.i91 = phi i64 [ %iter.sroa.15.0.i85, %bb1.i.i.i103 ], [ 0, %bb9.i.i87 ]
  %_0.i1.i.i.i.i.i.i92 = add nuw nsw i64 %iter.sroa.8.0.i84, 1
  br label %bb9.i93

bb9.i93:                                          ; preds = %"_ZN4core6option15Option$LT$T$GT$18get_or_insert_with17ha12f119ccbbcb787E.exit.i.thread18.i89", %bb18.i.i100
  %iter.sroa.15.316.i94 = phi i64 [ %iter.sroa.15.1.i90, %"_ZN4core6option15Option$LT$T$GT$18get_or_insert_with17ha12f119ccbbcb787E.exit.i.thread18.i89" ], [ %_0.i1.i.i.i.i101, %bb18.i.i100 ]
  %iter.sroa.8.215.i95 = phi i64 [ %_0.i1.i.i.i.i.i.i92, %"_ZN4core6option15Option$LT$T$GT$18get_or_insert_with17ha12f119ccbbcb787E.exit.i.thread18.i89" ], [ %iter.sroa.8.0.i84, %bb18.i.i100 ]
  %iter.sroa.5.114.i96 = phi i64 [ %iter.sroa.8.0.i84, %"_ZN4core6option15Option$LT$T$GT$18get_or_insert_with17ha12f119ccbbcb787E.exit.i.thread18.i89" ], [ %iter.sroa.5.0.i83, %bb18.i.i100 ]
  %iter.sroa.0.113.i97 = phi i64 [ 1, %"_ZN4core6option15Option$LT$T$GT$18get_or_insert_with17ha12f119ccbbcb787E.exit.i.thread18.i89" ], [ %iter.sroa.0.0.i82, %bb18.i.i100 ]
  %_5.sroa.7.1.i98 = phi i64 [ %elt_b.sroa.0.031.ph.ph.i.i91, %"_ZN4core6option15Option$LT$T$GT$18get_or_insert_with17ha12f119ccbbcb787E.exit.i.thread18.i89" ], [ %iter.sroa.15.0.i85, %bb18.i.i100 ]
  tail call void @llvm.experimental.noalias.scope.decl(metadata !31)
  %_14.i115 = trunc i64 %iter.sroa.5.114.i96 to i32
  %_16.i116 = trunc i64 %_5.sroa.7.1.i98 to i32
  br label %bb2.outer.i117

bb2.outer.i117:                                   ; preds = %bb9.i148, %bb9.i93
  %iter.sroa.0.0.ph.i118 = phi i32 [ %iter.sroa.0.135.i145, %bb9.i148 ], [ 2, %bb9.i93 ]
  %iter.sroa.5.0.ph.i119 = phi i32 [ %iter.sroa.5.136.i144, %bb9.i148 ], [ undef, %bb9.i93 ]
  %iter.sroa.8.0.ph.i120 = phi i32 [ %iter.sroa.8.237.i143, %bb9.i148 ], [ -1, %bb9.i93 ]
  %iter.sroa.14.0.ph.i121 = phi i8 [ %iter.sroa.14.238.i142, %bb9.i148 ], [ 0, %bb9.i93 ]
  %iter.sroa.1825.0.ph.i122 = phi i32 [ %iter.sroa.1825.439.i141, %bb9.i148 ], [ -1, %bb9.i93 ]
  %iter.sroa.26.0.shrunk.ph.i123 = phi i1 [ %iter.sroa.26.441.shrunk.i140, %bb9.i148 ], [ false, %bb9.i93 ]
  %alive_neighbours.sroa.0.0.ph.i124 = phi i32 [ %31, %bb9.i148 ], [ 0, %bb9.i93 ]
  br label %bb2.i125

bb2.i125:                                         ; preds = %bb5.i139, %bb2.outer.i117
  %iter.sroa.0.0.i126 = phi i32 [ %iter.sroa.0.135.i145, %bb5.i139 ], [ %iter.sroa.0.0.ph.i118, %bb2.outer.i117 ]
  %iter.sroa.5.0.i127 = phi i32 [ %iter.sroa.5.136.i144, %bb5.i139 ], [ %iter.sroa.5.0.ph.i119, %bb2.outer.i117 ]
  %iter.sroa.8.0.i128 = phi i32 [ %iter.sroa.8.237.i143, %bb5.i139 ], [ %iter.sroa.8.0.ph.i120, %bb2.outer.i117 ]
  %iter.sroa.14.0.i129 = phi i8 [ %iter.sroa.14.238.i142, %bb5.i139 ], [ %iter.sroa.14.0.ph.i121, %bb2.outer.i117 ]
  %iter.sroa.1825.0.i130 = phi i32 [ %iter.sroa.1825.439.i141, %bb5.i139 ], [ %iter.sroa.1825.0.ph.i122, %bb2.outer.i117 ]
  %iter.sroa.26.0.shrunk.i131 = phi i1 [ %iter.sroa.26.441.shrunk.i140, %bb5.i139 ], [ %iter.sroa.26.0.shrunk.ph.i123, %bb2.outer.i117 ]
  %_0.i.not.i.i.i.i132 = icmp sgt i32 %iter.sroa.1825.0.i130, 1
  %or.cond76.i133 = select i1 %iter.sroa.26.0.shrunk.i131, i1 true, i1 %_0.i.not.i.i.i.i132
  br i1 %or.cond76.i133, label %bb9.i.i176, label %bb2.i.i.i.i134

bb2.i.i.i.i134:                                   ; preds = %bb2.i125
  %_0.i3.i.i.i.i135 = icmp slt i32 %iter.sroa.1825.0.i130, 1
  %22 = zext i1 %_0.i3.i.i.i.i135 to i32
  %spec.select.i136 = add nsw i32 %iter.sroa.1825.0.i130, %22
  %not._0.i3.i.i.i.i137 = xor i1 %_0.i3.i.i.i.i135, true
  %cond.i138 = icmp eq i32 %iter.sroa.0.0.i126, 2
  br i1 %cond.i138, label %bb1.i.i.i156, label %bb5.i139

bb9.i.i176:                                       ; preds = %bb2.i125
  %_10.not.i.i21.i.i177 = icmp ne i8 %iter.sroa.14.0.i129, 0
  %_0.i.not.i.i28.i.i178 = icmp sgt i32 %iter.sroa.8.0.i128, 1
  %or.cond78.i179 = select i1 %_10.not.i.i21.i.i177, i1 true, i1 %_0.i.not.i.i28.i.i178
  br i1 %or.cond78.i179, label %bb6.i163, label %bb2.i.i29.i.i180

bb2.i.i29.i.i180:                                 ; preds = %bb9.i.i176
  %_0.i3.i.i30.i.i181 = icmp slt i32 %iter.sroa.8.0.i128, 1
  br i1 %_0.i3.i.i30.i.i181, label %bb4.i.i32.i.i182, label %bb5.i139

bb4.i.i32.i.i182:                                 ; preds = %bb2.i.i29.i.i180
  %23 = add nsw i32 %iter.sroa.8.0.i128, 1
  br label %bb5.i139

bb1.i.i.i156:                                     ; preds = %bb2.i.i.i.i134
  %_10.not.i.i.i.i.i.i157 = icmp ne i8 %iter.sroa.14.0.i129, 0
  %_0.i.not.i.i.i.i.i.i158 = icmp sgt i32 %iter.sroa.8.0.i128, 1
  %or.cond79.i159 = select i1 %_10.not.i.i.i.i.i.i157, i1 true, i1 %_0.i.not.i.i.i.i.i.i158
  br i1 %or.cond79.i159, label %bb6.i163, label %bb2.i.i.i.i.i.i160

bb2.i.i.i.i.i.i160:                               ; preds = %bb1.i.i.i156
  %_0.i3.i.i.i.i.i.i161 = icmp slt i32 %iter.sroa.8.0.i128, 1
  br i1 %_0.i3.i.i.i.i.i.i161, label %bb4.i.i.i.i.i.i162, label %bb5.i139

bb4.i.i.i.i.i.i162:                               ; preds = %bb2.i.i.i.i.i.i160
  %24 = add nsw i32 %iter.sroa.8.0.i128, 1
  br label %bb5.i139

bb6.i163:                                         ; preds = %bb1.i.i.i156, %bb9.i.i176
  %_22.i164 = icmp ult i64 %_5.sroa.7.1.i98, 256
  br i1 %_22.i164, label %bb15.i166, label %panic.i165

bb5.i139:                                         ; preds = %bb4.i.i.i.i.i.i162, %bb2.i.i.i.i.i.i160, %bb4.i.i32.i.i182, %bb2.i.i29.i.i180, %bb2.i.i.i.i134
  %iter.sroa.26.441.shrunk.i140 = phi i1 [ %not._0.i3.i.i.i.i137, %bb2.i.i.i.i.i.i160 ], [ false, %bb2.i.i29.i.i180 ], [ %not._0.i3.i.i.i.i137, %bb4.i.i.i.i.i.i162 ], [ false, %bb4.i.i32.i.i182 ], [ %not._0.i3.i.i.i.i137, %bb2.i.i.i.i134 ]
  %iter.sroa.1825.439.i141 = phi i32 [ %spec.select.i136, %bb2.i.i.i.i.i.i160 ], [ 0, %bb2.i.i29.i.i180 ], [ %spec.select.i136, %bb4.i.i.i.i.i.i162 ], [ 0, %bb4.i.i32.i.i182 ], [ %spec.select.i136, %bb2.i.i.i.i134 ]
  %iter.sroa.14.238.i142 = phi i8 [ 1, %bb2.i.i.i.i.i.i160 ], [ 1, %bb2.i.i29.i.i180 ], [ 0, %bb4.i.i.i.i.i.i162 ], [ 0, %bb4.i.i32.i.i182 ], [ %iter.sroa.14.0.i129, %bb2.i.i.i.i134 ]
  %iter.sroa.8.237.i143 = phi i32 [ 1, %bb2.i.i.i.i.i.i160 ], [ 1, %bb2.i.i29.i.i180 ], [ %24, %bb4.i.i.i.i.i.i162 ], [ %23, %bb4.i.i32.i.i182 ], [ %iter.sroa.8.0.i128, %bb2.i.i.i.i134 ]
  %iter.sroa.5.136.i144 = phi i32 [ 1, %bb2.i.i.i.i.i.i160 ], [ 1, %bb2.i.i29.i.i180 ], [ %iter.sroa.8.0.i128, %bb4.i.i.i.i.i.i162 ], [ %iter.sroa.8.0.i128, %bb4.i.i32.i.i182 ], [ %iter.sroa.5.0.i127, %bb2.i.i.i.i134 ]
  %iter.sroa.0.135.i145 = phi i32 [ 1, %bb2.i.i.i.i.i.i160 ], [ 1, %bb2.i.i29.i.i180 ], [ 1, %bb4.i.i.i.i.i.i162 ], [ 1, %bb4.i.i32.i.i182 ], [ %iter.sroa.0.0.i126, %bb2.i.i.i.i134 ]
  %_8.sroa.8.1.i146 = phi i32 [ %iter.sroa.1825.0.i130, %bb2.i.i.i.i.i.i160 ], [ -1, %bb2.i.i29.i.i180 ], [ %iter.sroa.1825.0.i130, %bb4.i.i.i.i.i.i162 ], [ -1, %bb4.i.i32.i.i182 ], [ %iter.sroa.1825.0.i130, %bb2.i.i.i.i134 ]
  %25 = or i32 %_8.sroa.8.1.i146, %iter.sroa.5.136.i144
  %or.cond.i147 = icmp eq i32 %25, 0
  br i1 %or.cond.i147, label %bb2.i125, label %bb9.i148

bb15.i166:                                        ; preds = %bb6.i163
  %_23.i167 = icmp ult i64 %iter.sroa.5.114.i96, 512
  br i1 %_23.i167, label %bb16.i169, label %panic1.i168

panic.i165:                                       ; preds = %bb6.i163
; call core::panicking::panic_bounds_check
  tail call void @_ZN4core9panicking18panic_bounds_check17h9397cb495d89a72dE(i64 noundef %_5.sroa.7.1.i98, i64 noundef 256, ptr noalias noundef nonnull readonly align 8 dereferenceable(24) @alloc_ab2ea280c03751c3a9cf53a9ca2a17c0) #12, !noalias !34
  unreachable

bb16.i169:                                        ; preds = %bb15.i166
  %26 = getelementptr inbounds [256 x [512 x i8]], ptr %board2, i64 0, i64 %_5.sroa.7.1.i98, i64 %iter.sroa.5.114.i96
  %27 = load i8, ptr %26, align 1, !range !9, !alias.scope !31, !noalias !28, !noundef !4
  %_21.not.i170 = icmp eq i8 %27, 0
  %switch.tableidx213 = add i32 %alive_neighbours.sroa.0.0.ph.i124, -3
  %28 = icmp ult i32 %switch.tableidx213, 6
  br i1 %_21.not.i170, label %bb28.i174, label %bb17.i171

panic1.i168:                                      ; preds = %bb15.i166
; call core::panicking::panic_bounds_check
  tail call void @_ZN4core9panicking18panic_bounds_check17h9397cb495d89a72dE(i64 noundef %iter.sroa.5.114.i96, i64 noundef 512, ptr noalias noundef nonnull readonly align 8 dereferenceable(24) @alloc_ab2ea280c03751c3a9cf53a9ca2a17c0) #12, !noalias !34
  unreachable

bb28.i174:                                        ; preds = %bb16.i169
  br i1 %28, label %switch.lookup212, label %get_next_cell_state.exit183

bb17.i171:                                        ; preds = %bb16.i169
  br i1 %28, label %switch.lookup218, label %get_next_cell_state.exit183

bb9.i148:                                         ; preds = %bb5.i139
  %_34.i149 = add i32 %iter.sroa.5.136.i144, %_14.i115
  %spec.select.i.i150 = and i32 %_34.i149, 511
  %x.i151 = zext nneg i32 %spec.select.i.i150 to i64
  %_36.i152 = add i32 %_8.sroa.8.1.i146, %_16.i116
  %spec.select.i22.i153 = and i32 %_36.i152, 255
  %y.i154 = zext nneg i32 %spec.select.i22.i153 to i64
  %29 = getelementptr inbounds [256 x [512 x i8]], ptr %board2, i64 0, i64 %y.i154, i64 %x.i151
  %30 = load i8, ptr %29, align 1, !range !9, !alias.scope !31, !noalias !28, !noundef !4
  %..i155 = zext nneg i8 %30 to i32
  %31 = add i32 %alive_neighbours.sroa.0.0.ph.i124, %..i155
  br label %bb2.outer.i117

switch.lookup212:                                 ; preds = %bb28.i174
  %32 = shl nuw nsw i32 %switch.tableidx213, 3
  %switch.shiftamt215 = zext nneg i32 %32 to i48
  %switch.downshift216 = lshr i48 1103823372289, %switch.shiftamt215
  %switch.masked217 = trunc i48 %switch.downshift216 to i8
  br label %get_next_cell_state.exit183

switch.lookup218:                                 ; preds = %bb17.i171
  %33 = shl nuw nsw i32 %switch.tableidx213, 3
  %switch.shiftamt221 = zext nneg i32 %33 to i48
  %switch.downshift222 = lshr i48 1103823372545, %switch.shiftamt221
  %switch.masked223 = trunc i48 %switch.downshift222 to i8
  br label %get_next_cell_state.exit183

get_next_cell_state.exit183:                      ; preds = %bb17.i171, %bb28.i174, %switch.lookup218, %switch.lookup212
  %_0.sroa.0.0.shrunk.i172 = phi i8 [ %switch.masked217, %switch.lookup212 ], [ %switch.masked223, %switch.lookup218 ], [ 0, %bb28.i174 ], [ 0, %bb17.i171 ]
  %34 = getelementptr inbounds [256 x [512 x i8]], ptr %board1, i64 0, i64 %_5.sroa.7.1.i98, i64 %iter.sroa.5.114.i96
  store i8 %_0.sroa.0.0.shrunk.i172, ptr %34, align 1, !alias.scope !28, !noalias !35
  br label %bb2.i81

bb14:                                             ; preds = %bb1.i.i.i103, %bb9.i.i87, %bb1.i.i.i51, %bb9.i.i35
  %35 = and i32 %board_index.sroa.0.0190, 1
  %36 = xor i32 %35, 1
  %exitcond.not = icmp eq i32 %3, 1000
  br i1 %exitcond.not, label %bb18, label %bb16
}

; Function Attrs: nounwind nonlazybind uwtable
declare noundef i32 @rust_eh_personality(i32 noundef, i32 noundef, i64 noundef, ptr noundef, ptr noundef) unnamed_addr #4

; Function Attrs: mustprogress nocallback nofree nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p0.i64(ptr noalias nocapture writeonly, ptr noalias nocapture readonly, i64, i1 immarg) #5

; std::rt::lang_start_internal
; Function Attrs: nonlazybind uwtable
declare noundef i64 @_ZN3std2rt19lang_start_internal17h5e7c81cecd7f0954E(ptr noundef nonnull align 1, ptr noalias noundef readonly align 8 dereferenceable(48), i64 noundef, ptr noundef, i8 noundef) unnamed_addr #0

; Function Attrs: mustprogress nocallback nofree nounwind willreturn memory(argmem: write)
declare void @llvm.memset.p0.i64(ptr nocapture writeonly, i8, i64, i1 immarg) #6

; Function Attrs: nounwind nonlazybind uwtable
declare noundef i32 @sim_rand() unnamed_addr #4

; core::panicking::panic_bounds_check
; Function Attrs: cold noinline noreturn nonlazybind uwtable
declare void @_ZN4core9panicking18panic_bounds_check17h9397cb495d89a72dE(i64 noundef, i64 noundef, ptr noalias noundef readonly align 8 dereferenceable(24)) unnamed_addr #7

; Function Attrs: nounwind nonlazybind uwtable
declare void @sim_put_pixel(i16 noundef zeroext, i16 noundef zeroext, i32 noundef) unnamed_addr #4

; Function Attrs: nounwind nonlazybind uwtable
declare void @sim_init() unnamed_addr #4

; Function Attrs: nounwind nonlazybind uwtable
declare void @sim_exit() unnamed_addr #4

; Function Attrs: nounwind nonlazybind uwtable
declare void @sim_flush() unnamed_addr #4

; Function Attrs: nonlazybind
define noundef i32 @main(i32 %0, ptr %1) unnamed_addr #8 {
top:
  %_8.i = alloca [8 x i8], align 8
  %2 = sext i32 %0 to i64
  call void @llvm.lifetime.start.p0(i64 8, ptr nonnull %_8.i)
  store ptr @_ZN9day_night4main17ha0c38ec631b4fa75E, ptr %_8.i, align 8
; call std::rt::lang_start_internal
  %3 = call noundef i64 @_ZN3std2rt19lang_start_internal17h5e7c81cecd7f0954E(ptr noundef nonnull align 1 %_8.i, ptr noalias noundef nonnull readonly align 8 dereferenceable(48) @vtable.0, i64 noundef %2, ptr noundef %1, i8 noundef 0)
  call void @llvm.lifetime.end.p0(i64 8, ptr nonnull %_8.i)
  %4 = trunc i64 %3 to i32
  ret i32 %4
}

; Function Attrs: mustprogress nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @llvm.lifetime.start.p0(i64 immarg, ptr nocapture) #9

; Function Attrs: mustprogress nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @llvm.lifetime.end.p0(i64 immarg, ptr nocapture) #9

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(inaccessiblemem: readwrite)
declare void @llvm.experimental.noalias.scope.decl(metadata) #10

attributes #0 = { nonlazybind uwtable "probe-stack"="inline-asm" "target-cpu"="x86-64" }
attributes #1 = { inlinehint nonlazybind uwtable "probe-stack"="inline-asm" "target-cpu"="x86-64" }
attributes #2 = { noinline nonlazybind uwtable "probe-stack"="inline-asm" "target-cpu"="x86-64" }
attributes #3 = { nofree norecurse nosync nounwind nonlazybind memory(argmem: readwrite) uwtable "probe-stack"="inline-asm" "target-cpu"="x86-64" }
attributes #4 = { nounwind nonlazybind uwtable "probe-stack"="inline-asm" "target-cpu"="x86-64" }
attributes #5 = { mustprogress nocallback nofree nounwind willreturn memory(argmem: readwrite) }
attributes #6 = { mustprogress nocallback nofree nounwind willreturn memory(argmem: write) }
attributes #7 = { cold noinline noreturn nonlazybind uwtable "probe-stack"="inline-asm" "target-cpu"="x86-64" }
attributes #8 = { nonlazybind "probe-stack"="inline-asm" "target-cpu"="x86-64" }
attributes #9 = { mustprogress nocallback nofree nosync nounwind willreturn memory(argmem: readwrite) }
attributes #10 = { nocallback nofree nosync nounwind willreturn memory(inaccessiblemem: readwrite) }
attributes #11 = { nounwind }
attributes #12 = { noreturn }

!llvm.module.flags = !{!0, !1, !2}
!llvm.ident = !{!3}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 2, !"RtLibUseGOT", i32 1}
!3 = !{!"rustc version 1.81.0 (eeb90cda1 2024-09-04)"}
!4 = !{}
!5 = !{i32 2381884}
!6 = !{!7}
!7 = distinct !{!7, !8, !"_ZN3std2rt10lang_start28_$u7b$$u7b$closure$u7d$$u7d$17h96d0021b3f1d3e68E: %_1"}
!8 = distinct !{!8, !"_ZN3std2rt10lang_start28_$u7b$$u7b$closure$u7d$$u7d$17h96d0021b3f1d3e68E"}
!9 = !{i8 0, i8 2}
!10 = !{!11}
!11 = distinct !{!11, !12, !"randomly_fill: %self"}
!12 = distinct !{!12, !"randomly_fill"}
!13 = !{!14}
!14 = distinct !{!14, !15, !"draw: %self"}
!15 = distinct !{!15, !"draw"}
!16 = !{!17}
!17 = distinct !{!17, !18, !"iterate: %self"}
!18 = distinct !{!18, !"iterate"}
!19 = !{!20}
!20 = distinct !{!20, !21, !"get_next_cell_state: %self"}
!21 = distinct !{!21, !"get_next_cell_state"}
!22 = !{!20, !17}
!23 = !{!24}
!24 = distinct !{!24, !18, !"iterate: %other"}
!25 = !{!26}
!26 = distinct !{!26, !27, !"draw: %self"}
!27 = distinct !{!27, !"draw"}
!28 = !{!29}
!29 = distinct !{!29, !30, !"iterate: %self"}
!30 = distinct !{!30, !"iterate"}
!31 = !{!32}
!32 = distinct !{!32, !33, !"get_next_cell_state: %self"}
!33 = distinct !{!33, !"get_next_cell_state"}
!34 = !{!32, !29}
!35 = !{!36}
!36 = distinct !{!36, !30, !"iterate: %other"}
