define i32 @main() {
0:
  %1 = alloca { [512 x [256 x i8]] }, align 8
  %2 = alloca { [512 x [256 x i8]] }, align 8
  call void @sim_init()
  br label %3

3:                                              ; preds = %5, %0
  %4 = phi i64 [ 0, %0 ], [ %6, %5 ]
  br label %8

5:                                              ; preds = %8
  %6 = add nuw i64 %4, 1
  %7 = icmp eq i64 %6, 512
  br i1 %7, label %17, label %3

8:                                              ; preds = %8, %3
  %9 = phi i64 [ 0, %3 ], [ %14, %8 ]
  %10 = call i32 @sim_rand()
  %11 = getelementptr [512 x [256 x i8]], ptr %1, i64 0, i64 %4, i64 %9
  %12 = trunc i32 %10 to i8
  %13 = and i8 %12, 1
  store i8 %13, ptr %11, align 1
  %14 = add nuw i64 %9, 1
  %15 = icmp eq i64 %14, 256
  br i1 %15, label %5, label %8

16:                                             ; preds = %53
  call void @sim_exit()
  ret i32 0

17:                                             ; preds = %53, %5
  %18 = phi i32 [ %56, %53 ], [ 0, %5 ]
  %19 = phi i32 [ %55, %53 ], [ 0, %5 ]
  %20 = icmp eq i32 %19, 0
  br i1 %20, label %21, label %37

21:                                             ; preds = %24, %17
  %22 = phi i64 [ %25, %24 ], [ 0, %17 ]
  %23 = trunc i64 %22 to i32
  br label %27

24:                                             ; preds = %27
  %25 = add nuw i64 %22, 1
  %26 = icmp eq i64 %25, 512
  br i1 %26, label %36, label %21

27:                                             ; preds = %27, %21
  %28 = phi i64 [ 0, %21 ], [ %34, %27 ]
  %29 = getelementptr [512 x [256 x i8]], ptr %1, i64 0, i64 %22, i64 %28
  %30 = load i8, ptr %29, align 1
  %31 = icmp eq i8 %30, 0
  %32 = select i1 %31, i32 -65536, i32 -16711936
  %33 = trunc i64 %28 to i32
  call void @sim_put_pixel(i32 %23, i32 %33, i32 %32)
  %34 = add nuw i64 %28, 1
  %35 = icmp eq i64 %34, 256
  br i1 %35, label %24, label %27

36:                                             ; preds = %24
  call void @get_next_state(ptr %1, ptr %2)
  br label %53

37:                                             ; preds = %40, %17
  %38 = phi i64 [ %41, %40 ], [ 0, %17 ]
  %39 = trunc i64 %38 to i32
  br label %43

40:                                             ; preds = %43
  %41 = add nuw i64 %38, 1
  %42 = icmp eq i64 %41, 512
  br i1 %42, label %52, label %37

43:                                             ; preds = %43, %37
  %44 = phi i64 [ 0, %37 ], [ %50, %43 ]
  %45 = getelementptr [512 x [256 x i8]], ptr %2, i64 0, i64 %38, i64 %44
  %46 = load i8, ptr %45, align 1
  %47 = icmp eq i8 %46, 0
  %48 = select i1 %47, i32 -65536, i32 -16711936
  %49 = trunc i64 %44 to i32
  call void @sim_put_pixel(i32 %39, i32 %49, i32 %48)
  %50 = add nuw i64 %44, 1
  %51 = icmp eq i64 %50, 256
  br i1 %51, label %40, label %43

52:                                             ; preds = %40
  call void @get_next_state(ptr %2, ptr %1)
  br label %53

53:                                             ; preds = %52, %36
  call void @sim_flush()
  %54 = add i32 %19, 1
  %55 = xor i32 %54, 1
  %56 = add nuw i32 %18, 1
  %57 = icmp eq i32 %56, 10000
  br i1 %57, label %16, label %17
}
