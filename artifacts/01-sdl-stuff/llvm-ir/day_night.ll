; ModuleID = 'src/c/day_night.c'
source_filename = "src/c/day_night.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%struct.board_s = type { [512 x [256 x i8]] }

; Function Attrs: nounwind sspstrong uwtable
define void @randomly_fill_board(ptr nocapture noundef writeonly %0) local_unnamed_addr #0 {
  br label %2

2:                                                ; preds = %1, %5
  %3 = phi i64 [ 0, %1 ], [ %6, %5 ]
  br label %8

4:                                                ; preds = %5
  ret void

5:                                                ; preds = %8
  %6 = add nuw nsw i64 %3, 1
  %7 = icmp eq i64 %6, 512
  br i1 %7, label %4, label %2, !llvm.loop !4

8:                                                ; preds = %2, %8
  %9 = phi i64 [ 0, %2 ], [ %14, %8 ]
  %10 = tail call i32 @sim_rand() #8
  %11 = getelementptr [512 x [256 x i8]], ptr %0, i64 0, i64 %3, i64 %9
  %12 = trunc i32 %10 to i8
  %13 = and i8 %12, 1
  store i8 %13, ptr %11, align 1, !tbaa !6
  %14 = add nuw nsw i64 %9, 1
  %15 = icmp eq i64 %14, 256
  br i1 %15, label %5, label %8, !llvm.loop !10
}

; Function Attrs: mustprogress nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @llvm.lifetime.start.p0(i64 immarg, ptr nocapture) #1

declare i32 @sim_rand() local_unnamed_addr #2

; Function Attrs: mustprogress nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @llvm.lifetime.end.p0(i64 immarg, ptr nocapture) #1

; Function Attrs: mustprogress nofree norecurse nosync nounwind sspstrong willreturn memory(none) uwtable
define noundef i32 @wrap_coordinate(i32 noundef %0, i32 noundef %1, i32 noundef %2) local_unnamed_addr #3 {
  %4 = icmp eq i32 %1, 0
  br i1 %4, label %16, label %5

5:                                                ; preds = %3
  %6 = icmp eq i32 %0, 0
  %7 = icmp eq i32 %1, -1
  %8 = and i1 %6, %7
  %9 = add i32 %2, -1
  br i1 %8, label %16, label %10

10:                                               ; preds = %5
  %11 = icmp eq i32 %9, %0
  %12 = icmp eq i32 %1, 1
  %13 = and i1 %12, %11
  %14 = add i32 %1, %0
  %15 = select i1 %13, i32 0, i32 %14
  br label %16

16:                                               ; preds = %5, %10, %3
  %17 = phi i32 [ %0, %3 ], [ %15, %10 ], [ %9, %5 ]
  ret i32 %17
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind sspstrong willreturn memory(argmem: read) uwtable
define noundef zeroext i1 @get_next_cell_state(ptr nocapture noundef readonly %0, i32 noundef %1, i32 noundef %2) local_unnamed_addr #4 {
  %4 = icmp eq i32 %1, 0
  %5 = icmp eq i32 %1, 511
  %6 = icmp eq i32 %2, 0
  %7 = icmp eq i32 %2, 255
  %8 = add i32 %1, -1
  %9 = select i1 %4, i32 511, i32 %8
  %10 = add i32 %2, -1
  %11 = select i1 %6, i32 255, i32 %10
  %12 = sext i32 %9 to i64
  %13 = sext i32 %11 to i64
  %14 = getelementptr [512 x [256 x i8]], ptr %0, i64 0, i64 %12, i64 %13
  %15 = load i8, ptr %14, align 1, !tbaa !6, !range !11, !noundef !12
  %16 = select i1 %4, i32 511, i32 %8
  %17 = sext i32 %16 to i64
  %18 = sext i32 %2 to i64
  %19 = getelementptr [512 x [256 x i8]], ptr %0, i64 0, i64 %17, i64 %18
  %20 = load i8, ptr %19, align 1, !tbaa !6, !range !11, !noundef !12
  %21 = add nuw nsw i8 %15, %20
  %22 = select i1 %4, i32 511, i32 %8
  %23 = add i32 %2, 1
  %24 = select i1 %7, i32 0, i32 %23
  %25 = sext i32 %22 to i64
  %26 = sext i32 %24 to i64
  %27 = getelementptr [512 x [256 x i8]], ptr %0, i64 0, i64 %25, i64 %26
  %28 = load i8, ptr %27, align 1, !tbaa !6, !range !11, !noundef !12
  %29 = add nuw nsw i8 %21, %28
  %30 = add i32 %2, -1
  %31 = select i1 %6, i32 255, i32 %30
  %32 = sext i32 %31 to i64
  %33 = sext i32 %1 to i64
  %34 = getelementptr [512 x [256 x i8]], ptr %0, i64 0, i64 %33, i64 %32
  %35 = load i8, ptr %34, align 1, !tbaa !6, !range !11, !noundef !12
  %36 = add nuw nsw i8 %29, %35
  %37 = add i32 %2, 1
  %38 = select i1 %7, i32 0, i32 %37
  %39 = sext i32 %1 to i64
  %40 = sext i32 %38 to i64
  %41 = getelementptr [512 x [256 x i8]], ptr %0, i64 0, i64 %39, i64 %40
  %42 = load i8, ptr %41, align 1, !tbaa !6, !range !11, !noundef !12
  %43 = add nuw nsw i8 %36, %42
  %44 = add i32 %1, 1
  %45 = select i1 %5, i32 0, i32 %44
  %46 = add i32 %2, -1
  %47 = select i1 %6, i32 255, i32 %46
  %48 = sext i32 %47 to i64
  %49 = sext i32 %45 to i64
  %50 = getelementptr [512 x [256 x i8]], ptr %0, i64 0, i64 %49, i64 %48
  %51 = load i8, ptr %50, align 1, !tbaa !6, !range !11, !noundef !12
  %52 = add nuw nsw i8 %43, %51
  %53 = sext i32 %45 to i64
  %54 = sext i32 %2 to i64
  %55 = getelementptr [512 x [256 x i8]], ptr %0, i64 0, i64 %53, i64 %54
  %56 = load i8, ptr %55, align 1, !tbaa !6, !range !11, !noundef !12
  %57 = add nuw nsw i8 %52, %56
  %58 = add i32 %2, 1
  %59 = select i1 %7, i32 0, i32 %58
  %60 = sext i32 %45 to i64
  %61 = sext i32 %59 to i64
  %62 = getelementptr [512 x [256 x i8]], ptr %0, i64 0, i64 %60, i64 %61
  %63 = load i8, ptr %62, align 1, !tbaa !6, !range !11, !noundef !12
  %64 = add i8 %57, %63
  %65 = sext i32 %1 to i64
  %66 = sext i32 %2 to i64
  %67 = getelementptr [512 x [256 x i8]], ptr %0, i64 0, i64 %65, i64 %66
  %68 = load i8, ptr %67, align 1, !tbaa !6, !range !11, !noundef !12
  %69 = icmp eq i8 %68, 0
  %70 = icmp ult i8 %64, 9
  br i1 %69, label %72, label %71

71:                                               ; preds = %3
  br i1 %70, label %73, label %83

72:                                               ; preds = %3
  br i1 %70, label %78, label %83

73:                                               ; preds = %71
  %74 = zext nneg i8 %64 to i9
  %75 = lshr i9 -40, %74
  %76 = and i9 %75, 1
  %77 = icmp ne i9 %76, 0
  br label %83

78:                                               ; preds = %72
  %79 = zext nneg i8 %64 to i9
  %80 = lshr i9 -56, %79
  %81 = and i9 %80, 1
  %82 = icmp ne i9 %81, 0
  br label %83

83:                                               ; preds = %72, %78, %71, %73
  %84 = phi i1 [ %77, %73 ], [ false, %71 ], [ %82, %78 ], [ false, %72 ]
  ret i1 %84
}

; Function Attrs: nofree norecurse nosync nounwind sspstrong memory(argmem: readwrite) uwtable
define void @get_next_state(ptr nocapture noundef readonly %0, ptr nocapture noundef writeonly %1) local_unnamed_addr #5 {
  br label %3

3:                                                ; preds = %2, %7
  %4 = phi i64 [ 0, %2 ], [ %8, %7 ]
  %5 = trunc i64 %4 to i32
  br label %10

6:                                                ; preds = %7
  ret void

7:                                                ; preds = %10
  %8 = add nuw nsw i64 %4, 1
  %9 = icmp eq i64 %8, 512
  br i1 %9, label %6, label %3, !llvm.loop !13

10:                                               ; preds = %3, %10
  %11 = phi i64 [ 0, %3 ], [ %16, %10 ]
  %12 = trunc i64 %11 to i32
  %13 = tail call zeroext i1 @get_next_cell_state(ptr noundef %0, i32 noundef %5, i32 noundef %12)
  %14 = getelementptr [512 x [256 x i8]], ptr %1, i64 0, i64 %4, i64 %11
  %15 = zext i1 %13 to i8
  store i8 %15, ptr %14, align 1, !tbaa !6
  %16 = add nuw nsw i64 %11, 1
  %17 = icmp eq i64 %16, 256
  br i1 %17, label %7, label %10, !llvm.loop !14
}

; Function Attrs: nounwind sspstrong uwtable
define void @draw_board(ptr nocapture noundef readonly %0) local_unnamed_addr #0 {
  br label %2

2:                                                ; preds = %1, %6
  %3 = phi i64 [ 0, %1 ], [ %7, %6 ]
  %4 = trunc i64 %3 to i32
  br label %9

5:                                                ; preds = %6
  ret void

6:                                                ; preds = %9
  %7 = add nuw nsw i64 %3, 1
  %8 = icmp eq i64 %7, 512
  br i1 %8, label %5, label %2, !llvm.loop !15

9:                                                ; preds = %2, %9
  %10 = phi i64 [ 0, %2 ], [ %16, %9 ]
  %11 = getelementptr [512 x [256 x i8]], ptr %0, i64 0, i64 %3, i64 %10
  %12 = load i8, ptr %11, align 1, !tbaa !6, !range !11, !noundef !12
  %13 = icmp eq i8 %12, 0
  %14 = select i1 %13, i32 -65536, i32 -16711936
  %15 = trunc i64 %10 to i32
  tail call void @sim_put_pixel(i32 noundef %4, i32 noundef %15, i32 noundef %14) #8
  %16 = add nuw nsw i64 %10, 1
  %17 = icmp eq i64 %16, 256
  br i1 %17, label %6, label %9, !llvm.loop !16
}

declare void @sim_put_pixel(i32 noundef, i32 noundef, i32 noundef) local_unnamed_addr #2

; Function Attrs: nounwind sspstrong uwtable
define noundef i32 @main() local_unnamed_addr #6 {
  %1 = alloca %struct.board_s, align 1
  %2 = alloca %struct.board_s, align 1
  call void @llvm.lifetime.start.p0(i64 131072, ptr nonnull %1) #8
  call void @llvm.memset.p0.i64(ptr noundef nonnull align 1 dereferenceable(131072) %1, i8 0, i64 131072, i1 false)
  call void @llvm.lifetime.start.p0(i64 131072, ptr nonnull %2) #8
  call void @llvm.memset.p0.i64(ptr noundef nonnull align 1 dereferenceable(131072) %2, i8 0, i64 131072, i1 false)
  tail call void @sim_init() #8
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
  %10 = tail call i32 @sim_rand() #8
  %11 = getelementptr [512 x [256 x i8]], ptr %1, i64 0, i64 %4, i64 %9
  %12 = trunc i32 %10 to i8
  %13 = and i8 %12, 1
  store i8 %13, ptr %11, align 1, !tbaa !6
  %14 = add nuw nsw i64 %9, 1
  %15 = icmp eq i64 %14, 256
  br i1 %15, label %5, label %8, !llvm.loop !10

16:                                               ; preds = %79
  tail call void @sim_exit() #8
  call void @llvm.lifetime.end.p0(i64 131072, ptr nonnull %2) #8
  call void @llvm.lifetime.end.p0(i64 131072, ptr nonnull %1) #8
  ret i32 0

17:                                               ; preds = %5, %79
  %18 = phi i32 [ %82, %79 ], [ 0, %5 ]
  %19 = phi i32 [ %81, %79 ], [ 0, %5 ]
  %20 = icmp eq i32 %19, 0
  br i1 %20, label %21, label %50

21:                                               ; preds = %17, %24
  %22 = phi i64 [ %25, %24 ], [ 0, %17 ]
  %23 = trunc i64 %22 to i32
  br label %27

24:                                               ; preds = %27
  %25 = add nuw nsw i64 %22, 1
  %26 = icmp eq i64 %25, 512
  br i1 %26, label %36, label %21, !llvm.loop !15

27:                                               ; preds = %27, %21
  %28 = phi i64 [ 0, %21 ], [ %34, %27 ]
  %29 = getelementptr [512 x [256 x i8]], ptr %1, i64 0, i64 %22, i64 %28
  %30 = load i8, ptr %29, align 1, !tbaa !6, !range !11, !noundef !12
  %31 = icmp eq i8 %30, 0
  %32 = select i1 %31, i32 -65536, i32 -16711936
  %33 = trunc i64 %28 to i32
  tail call void @sim_put_pixel(i32 noundef %23, i32 noundef %33, i32 noundef %32) #8
  %34 = add nuw nsw i64 %28, 1
  %35 = icmp eq i64 %34, 256
  br i1 %35, label %24, label %27, !llvm.loop !16

36:                                               ; preds = %24, %39
  %37 = phi i64 [ %40, %39 ], [ 0, %24 ]
  %38 = trunc i64 %37 to i32
  br label %42

39:                                               ; preds = %42
  %40 = add nuw nsw i64 %37, 1
  %41 = icmp eq i64 %40, 512
  br i1 %41, label %79, label %36, !llvm.loop !13

42:                                               ; preds = %42, %36
  %43 = phi i64 [ 0, %36 ], [ %48, %42 ]
  %44 = trunc i64 %43 to i32
  %45 = call zeroext i1 @get_next_cell_state(ptr noundef nonnull %1, i32 noundef %38, i32 noundef %44)
  %46 = getelementptr [512 x [256 x i8]], ptr %2, i64 0, i64 %37, i64 %43
  %47 = zext i1 %45 to i8
  store i8 %47, ptr %46, align 1, !tbaa !6
  %48 = add nuw nsw i64 %43, 1
  %49 = icmp eq i64 %48, 256
  br i1 %49, label %39, label %42, !llvm.loop !14

50:                                               ; preds = %17, %53
  %51 = phi i64 [ %54, %53 ], [ 0, %17 ]
  %52 = trunc i64 %51 to i32
  br label %56

53:                                               ; preds = %56
  %54 = add nuw nsw i64 %51, 1
  %55 = icmp eq i64 %54, 512
  br i1 %55, label %65, label %50, !llvm.loop !15

56:                                               ; preds = %56, %50
  %57 = phi i64 [ 0, %50 ], [ %63, %56 ]
  %58 = getelementptr [512 x [256 x i8]], ptr %2, i64 0, i64 %51, i64 %57
  %59 = load i8, ptr %58, align 1, !tbaa !6, !range !11, !noundef !12
  %60 = icmp eq i8 %59, 0
  %61 = select i1 %60, i32 -65536, i32 -16711936
  %62 = trunc i64 %57 to i32
  tail call void @sim_put_pixel(i32 noundef %52, i32 noundef %62, i32 noundef %61) #8
  %63 = add nuw nsw i64 %57, 1
  %64 = icmp eq i64 %63, 256
  br i1 %64, label %53, label %56, !llvm.loop !16

65:                                               ; preds = %53, %68
  %66 = phi i64 [ %69, %68 ], [ 0, %53 ]
  %67 = trunc i64 %66 to i32
  br label %71

68:                                               ; preds = %71
  %69 = add nuw nsw i64 %66, 1
  %70 = icmp eq i64 %69, 512
  br i1 %70, label %79, label %65, !llvm.loop !13

71:                                               ; preds = %71, %65
  %72 = phi i64 [ 0, %65 ], [ %77, %71 ]
  %73 = trunc i64 %72 to i32
  %74 = call zeroext i1 @get_next_cell_state(ptr noundef nonnull %2, i32 noundef %67, i32 noundef %73)
  %75 = getelementptr [512 x [256 x i8]], ptr %1, i64 0, i64 %66, i64 %72
  %76 = zext i1 %74 to i8
  store i8 %76, ptr %75, align 1, !tbaa !6
  %77 = add nuw nsw i64 %72, 1
  %78 = icmp eq i64 %77, 256
  br i1 %78, label %68, label %71, !llvm.loop !14

79:                                               ; preds = %68, %39
  tail call void @sim_flush() #8
  %80 = and i32 %19, 1
  %81 = xor i32 %80, 1
  %82 = add nuw nsw i32 %18, 1
  %83 = icmp eq i32 %82, 10000
  br i1 %83, label %16, label %17, !llvm.loop !17
}

; Function Attrs: mustprogress nocallback nofree nounwind willreturn memory(argmem: write)
declare void @llvm.memset.p0.i64(ptr nocapture writeonly, i8, i64, i1 immarg) #7

declare void @sim_init() local_unnamed_addr #2

declare void @sim_flush() local_unnamed_addr #2

declare void @sim_exit() local_unnamed_addr #2

attributes #0 = { nounwind sspstrong uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="4" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" "zero-call-used-regs"="used-gpr" }
attributes #1 = { mustprogress nocallback nofree nosync nounwind willreturn memory(argmem: readwrite) }
attributes #2 = { "no-trapping-math"="true" "stack-protector-buffer-size"="4" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" "zero-call-used-regs"="used-gpr" }
attributes #3 = { mustprogress nofree norecurse nosync nounwind sspstrong willreturn memory(none) uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="4" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" "zero-call-used-regs"="used-gpr" }
attributes #4 = { mustprogress nofree norecurse nosync nounwind sspstrong willreturn memory(argmem: read) uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="4" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" "zero-call-used-regs"="used-gpr" }
attributes #5 = { nofree norecurse nosync nounwind sspstrong memory(argmem: readwrite) uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="4" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" "zero-call-used-regs"="used-gpr" }
attributes #6 = { nounwind sspstrong uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="4" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #7 = { mustprogress nocallback nofree nounwind willreturn memory(argmem: write) }
attributes #8 = { nounwind }

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
!11 = !{i8 0, i8 2}
!12 = !{}
!13 = distinct !{!13, !5}
!14 = distinct !{!14, !5}
!15 = distinct !{!15, !5}
!16 = distinct !{!16, !5}
!17 = distinct !{!17, !5}
