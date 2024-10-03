; ModuleID = 'src/day_night.c'
source_filename = "src/day_night.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%struct.board_s = type { [512 x [256 x i8]] }

; Function Attrs: nounwind sspstrong uwtable
define noundef i32 @main() local_unnamed_addr #0 {
  %1 = alloca %struct.board_s, align 1
  %2 = alloca %struct.board_s, align 1
  call void @llvm.lifetime.start.p0(i64 131072, ptr nonnull %1) #5
  call void @llvm.memset.p0.i64(ptr noundef nonnull align 1 dereferenceable(131072) %1, i8 0, i64 131072, i1 false)
  call void @llvm.lifetime.start.p0(i64 131072, ptr nonnull %2) #5
  call void @llvm.memset.p0.i64(ptr noundef nonnull align 1 dereferenceable(131072) %2, i8 0, i64 131072, i1 false)
  tail call void @sim_init() #5
  br label %3

3:                                                ; preds = %5, %0
  %4 = phi i64 [ 0, %0 ], [ %6, %5 ]
  br label %8

5:                                                ; preds = %8
  %6 = add nuw nsw i64 %4, 1
  %7 = icmp eq i64 %6, 512
  br i1 %7, label %17, label %3, !llvm.loop !4

8:                                                ; preds = %8, %3
  %9 = phi i64 [ 0, %3 ], [ %14, %8 ]
  %10 = tail call i32 @sim_rand() #5
  %11 = getelementptr [512 x [256 x i8]], ptr %1, i64 0, i64 %4, i64 %9
  %12 = trunc i32 %10 to i8
  %13 = and i8 %12, 1
  store i8 %13, ptr %11, align 1, !tbaa !6
  %14 = add nuw nsw i64 %9, 1
  %15 = icmp eq i64 %14, 256
  br i1 %15, label %5, label %8, !llvm.loop !10

16:                                               ; preds = %53
  tail call void @sim_exit() #5
  call void @llvm.lifetime.end.p0(i64 131072, ptr nonnull %2) #5
  call void @llvm.lifetime.end.p0(i64 131072, ptr nonnull %1) #5
  ret i32 0

17:                                               ; preds = %5, %53
  %18 = phi i32 [ %56, %53 ], [ 0, %5 ]
  %19 = phi i32 [ %55, %53 ], [ 0, %5 ]
  %20 = icmp eq i32 %19, 0
  br i1 %20, label %21, label %37

21:                                               ; preds = %17, %24
  %22 = phi i64 [ %25, %24 ], [ 0, %17 ]
  %23 = trunc i64 %22 to i32
  br label %27

24:                                               ; preds = %27
  %25 = add nuw nsw i64 %22, 1
  %26 = icmp eq i64 %25, 512
  br i1 %26, label %36, label %21, !llvm.loop !11

27:                                               ; preds = %27, %21
  %28 = phi i64 [ 0, %21 ], [ %34, %27 ]
  %29 = getelementptr [512 x [256 x i8]], ptr %1, i64 0, i64 %22, i64 %28
  %30 = load i8, ptr %29, align 1, !tbaa !6, !range !12, !noundef !13
  %31 = icmp eq i8 %30, 0
  %32 = select i1 %31, i32 -65536, i32 -16711936
  %33 = trunc i64 %28 to i32
  tail call void @sim_put_pixel(i32 noundef %23, i32 noundef %33, i32 noundef %32) #5
  %34 = add nuw nsw i64 %28, 1
  %35 = icmp eq i64 %34, 256
  br i1 %35, label %24, label %27, !llvm.loop !14

36:                                               ; preds = %24
  call fastcc void @get_next_state(ptr noundef nonnull %1, ptr noundef nonnull %2)
  br label %53

37:                                               ; preds = %17, %40
  %38 = phi i64 [ %41, %40 ], [ 0, %17 ]
  %39 = trunc i64 %38 to i32
  br label %43

40:                                               ; preds = %43
  %41 = add nuw nsw i64 %38, 1
  %42 = icmp eq i64 %41, 512
  br i1 %42, label %52, label %37, !llvm.loop !11

43:                                               ; preds = %43, %37
  %44 = phi i64 [ 0, %37 ], [ %50, %43 ]
  %45 = getelementptr [512 x [256 x i8]], ptr %2, i64 0, i64 %38, i64 %44
  %46 = load i8, ptr %45, align 1, !tbaa !6, !range !12, !noundef !13
  %47 = icmp eq i8 %46, 0
  %48 = select i1 %47, i32 -65536, i32 -16711936
  %49 = trunc i64 %44 to i32
  tail call void @sim_put_pixel(i32 noundef %39, i32 noundef %49, i32 noundef %48) #5
  %50 = add nuw nsw i64 %44, 1
  %51 = icmp eq i64 %50, 256
  br i1 %51, label %40, label %43, !llvm.loop !14

52:                                               ; preds = %40
  call fastcc void @get_next_state(ptr noundef nonnull %2, ptr noundef nonnull %1)
  br label %53

53:                                               ; preds = %52, %36
  tail call void @sim_flush() #5
  %54 = and i32 %19, 1
  %55 = xor i32 %54, 1
  %56 = add nuw nsw i32 %18, 1
  %57 = icmp eq i32 %56, 10000
  br i1 %57, label %16, label %17, !llvm.loop !15
}

; Function Attrs: mustprogress nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @llvm.lifetime.start.p0(i64 immarg, ptr nocapture) #1

; Function Attrs: mustprogress nocallback nofree nounwind willreturn memory(argmem: write)
declare void @llvm.memset.p0.i64(ptr nocapture writeonly, i8, i64, i1 immarg) #2

declare void @sim_init() local_unnamed_addr #3

; Function Attrs: nofree norecurse nosync nounwind sspstrong memory(argmem: readwrite) uwtable
define internal fastcc void @get_next_state(ptr nocapture noundef readonly %0, ptr nocapture noundef writeonly %1) unnamed_addr #4 {
  br label %3

3:                                                ; preds = %2, %58
  %4 = phi i64 [ 0, %2 ], [ %11, %58 ]
  %5 = icmp eq i64 %4, 0
  %6 = icmp eq i64 %4, 511
  %7 = shl i64 %4, 32
  %8 = add i64 %7, -4294967296
  %9 = ashr exact i64 %8, 32
  %10 = select i1 %5, i64 511, i64 %9
  %11 = add nuw nsw i64 %4, 1
  %12 = and i64 %11, 4294967295
  %13 = select i1 %6, i64 0, i64 %12
  %14 = getelementptr [512 x [256 x i8]], ptr %0, i64 0, i64 %10, i64 255
  %15 = load i8, ptr %14, align 1, !tbaa !6, !range !12, !noundef !13
  %16 = getelementptr [512 x [256 x i8]], ptr %0, i64 0, i64 %10, i64 0
  %17 = load i8, ptr %16, align 1, !tbaa !6, !range !12, !noundef !13
  %18 = add nuw nsw i8 %17, %15
  %19 = getelementptr [512 x [256 x i8]], ptr %0, i64 0, i64 %10, i64 1
  %20 = load i8, ptr %19, align 1, !tbaa !6, !range !12, !noundef !13
  %21 = add nuw nsw i8 %18, %20
  %22 = getelementptr [512 x [256 x i8]], ptr %0, i64 0, i64 %4, i64 255
  %23 = load i8, ptr %22, align 1, !tbaa !6, !range !12, !noundef !13
  %24 = add nuw nsw i8 %21, %23
  %25 = getelementptr [512 x [256 x i8]], ptr %0, i64 0, i64 %4, i64 1
  %26 = load i8, ptr %25, align 1, !tbaa !6, !range !12, !noundef !13
  %27 = add nuw nsw i8 %24, %26
  %28 = getelementptr [512 x [256 x i8]], ptr %0, i64 0, i64 %13, i64 255
  %29 = load i8, ptr %28, align 1, !tbaa !6, !range !12, !noundef !13
  %30 = add nuw nsw i8 %27, %29
  %31 = getelementptr [512 x [256 x i8]], ptr %0, i64 0, i64 %13, i64 0
  %32 = load i8, ptr %31, align 1, !tbaa !6, !range !12, !noundef !13
  %33 = add nuw nsw i8 %30, %32
  %34 = getelementptr [512 x [256 x i8]], ptr %0, i64 0, i64 %13, i64 1
  %35 = load i8, ptr %34, align 1, !tbaa !6, !range !12, !noundef !13
  %36 = add i8 %33, %35
  %37 = getelementptr [512 x [256 x i8]], ptr %0, i64 0, i64 %4, i64 0
  %38 = load i8, ptr %37, align 1, !tbaa !6, !range !12, !noundef !13
  %39 = icmp eq i8 %38, 0
  %40 = add i8 %36, -3
  %41 = icmp ult i8 %40, 6
  br i1 %39, label %43, label %42

42:                                               ; preds = %3
  br i1 %41, label %44, label %54

43:                                               ; preds = %3
  br i1 %41, label %49, label %54

44:                                               ; preds = %42
  %45 = shl nuw nsw i8 %40, 3
  %46 = zext nneg i8 %45 to i48
  %47 = lshr i48 1103823372545, %46
  %48 = trunc i48 %47 to i8
  br label %54

49:                                               ; preds = %43
  %50 = shl nuw nsw i8 %40, 3
  %51 = zext nneg i8 %50 to i48
  %52 = lshr i48 1103823372289, %51
  %53 = trunc i48 %52 to i8
  br label %54

54:                                               ; preds = %43, %49, %42, %44
  %55 = phi i8 [ %48, %44 ], [ 0, %42 ], [ %53, %49 ], [ 0, %43 ]
  %56 = getelementptr [512 x [256 x i8]], ptr %1, i64 0, i64 %4, i64 0
  store i8 %55, ptr %56, align 1, !tbaa !6
  br label %60

57:                                               ; preds = %58
  ret void

58:                                               ; preds = %109
  %59 = icmp eq i64 %11, 512
  br i1 %59, label %57, label %3, !llvm.loop !16

60:                                               ; preds = %54, %109
  %61 = phi i64 [ 1, %54 ], [ %71, %109 ]
  %62 = icmp eq i64 %61, 255
  %63 = shl i64 %61, 32
  %64 = add i64 %63, -4294967296
  %65 = ashr exact i64 %64, 32
  %66 = getelementptr [512 x [256 x i8]], ptr %0, i64 0, i64 %10, i64 %65
  %67 = load i8, ptr %66, align 1, !tbaa !6, !range !12, !noundef !13
  %68 = getelementptr [512 x [256 x i8]], ptr %0, i64 0, i64 %10, i64 %61
  %69 = load i8, ptr %68, align 1, !tbaa !6, !range !12, !noundef !13
  %70 = add nuw nsw i8 %69, %67
  %71 = add nuw nsw i64 %61, 1
  %72 = and i64 %71, 4294967295
  %73 = select i1 %62, i64 0, i64 %72
  %74 = getelementptr [512 x [256 x i8]], ptr %0, i64 0, i64 %10, i64 %73
  %75 = load i8, ptr %74, align 1, !tbaa !6, !range !12, !noundef !13
  %76 = add nuw nsw i8 %70, %75
  %77 = getelementptr [512 x [256 x i8]], ptr %0, i64 0, i64 %4, i64 %65
  %78 = load i8, ptr %77, align 1, !tbaa !6, !range !12, !noundef !13
  %79 = add nuw nsw i8 %76, %78
  %80 = getelementptr [512 x [256 x i8]], ptr %0, i64 0, i64 %4, i64 %73
  %81 = load i8, ptr %80, align 1, !tbaa !6, !range !12, !noundef !13
  %82 = add nuw nsw i8 %79, %81
  %83 = getelementptr [512 x [256 x i8]], ptr %0, i64 0, i64 %13, i64 %65
  %84 = load i8, ptr %83, align 1, !tbaa !6, !range !12, !noundef !13
  %85 = add nuw nsw i8 %82, %84
  %86 = getelementptr [512 x [256 x i8]], ptr %0, i64 0, i64 %13, i64 %61
  %87 = load i8, ptr %86, align 1, !tbaa !6, !range !12, !noundef !13
  %88 = add nuw nsw i8 %85, %87
  %89 = getelementptr [512 x [256 x i8]], ptr %0, i64 0, i64 %13, i64 %73
  %90 = load i8, ptr %89, align 1, !tbaa !6, !range !12, !noundef !13
  %91 = add i8 %88, %90
  %92 = getelementptr [512 x [256 x i8]], ptr %0, i64 0, i64 %4, i64 %61
  %93 = load i8, ptr %92, align 1, !tbaa !6, !range !12, !noundef !13
  %94 = icmp eq i8 %93, 0
  %95 = add i8 %91, -3
  %96 = icmp ult i8 %95, 6
  br i1 %94, label %98, label %97

97:                                               ; preds = %60
  br i1 %96, label %99, label %109

98:                                               ; preds = %60
  br i1 %96, label %104, label %109

99:                                               ; preds = %97
  %100 = shl nuw nsw i8 %95, 3
  %101 = zext nneg i8 %100 to i48
  %102 = lshr i48 1103823372545, %101
  %103 = trunc i48 %102 to i8
  br label %109

104:                                              ; preds = %98
  %105 = shl nuw nsw i8 %95, 3
  %106 = zext nneg i8 %105 to i48
  %107 = lshr i48 1103823372289, %106
  %108 = trunc i48 %107 to i8
  br label %109

109:                                              ; preds = %98, %104, %97, %99
  %110 = phi i8 [ %103, %99 ], [ 0, %97 ], [ %108, %104 ], [ 0, %98 ]
  %111 = getelementptr [512 x [256 x i8]], ptr %1, i64 0, i64 %4, i64 %61
  store i8 %110, ptr %111, align 1, !tbaa !6
  %112 = icmp eq i64 %71, 256
  br i1 %112, label %58, label %60, !llvm.loop !17
}

declare void @sim_flush() local_unnamed_addr #3

; Function Attrs: mustprogress nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @llvm.lifetime.end.p0(i64 immarg, ptr nocapture) #1

declare void @sim_exit() local_unnamed_addr #3

declare i32 @sim_rand() local_unnamed_addr #3

declare void @sim_put_pixel(i32 noundef, i32 noundef, i32 noundef) local_unnamed_addr #3

attributes #0 = { nounwind sspstrong uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="4" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { mustprogress nocallback nofree nosync nounwind willreturn memory(argmem: readwrite) }
attributes #2 = { mustprogress nocallback nofree nounwind willreturn memory(argmem: write) }
attributes #3 = { "no-trapping-math"="true" "stack-protector-buffer-size"="4" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" "zero-call-used-regs"="used-gpr" }
attributes #4 = { nofree norecurse nosync nounwind sspstrong memory(argmem: readwrite) uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="4" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" "zero-call-used-regs"="used-gpr" }
attributes #5 = { nounwind }

!llvm.module.flags = !{!0, !1, !2}
!llvm.ident = !{!3}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{!"clang version 18.1.8"}
!4 = distinct !{!4, !5}
!5 = !{!"llvm.loop.mustprogress"}
!6 = !{!7, !7, i64 0}
!7 = !{!"_Bool", !8, i64 0}
!8 = !{!"omnipotent char", !9, i64 0}
!9 = !{!"Simple C/C++ TBAA"}
!10 = distinct !{!10, !5}
!11 = distinct !{!11, !5}
!12 = !{i8 0, i8 2}
!13 = !{}
!14 = distinct !{!14, !5}
!15 = distinct !{!15, !5}
!16 = distinct !{!16, !5}
!17 = distinct !{!17, !5, !18}
!18 = !{!"llvm.loop.peeled.count", i32 1}
