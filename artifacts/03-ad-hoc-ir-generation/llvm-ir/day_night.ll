; ModuleID = 'day_night_irgen'
source_filename = "day_night_irgen"

define internal void @get_next_state(ptr %0, ptr %1) {
bb2:
  br label %bb3

bb3:                                              ; preds = %bb58, %bb2
  %v4 = phi i64 [ 0, %bb2 ], [ %v11, %bb58 ]
  %v5 = icmp eq i64 %v4, 0
  %v6 = icmp eq i64 %v4, 511
  %v7 = shl i64 %v4, 32
  %v8 = add i64 %v7, -4294967296
  %v9 = ashr i64 %v8, 32
  %v10 = select i1 %v5, i64 511, i64 %v9
  %v11 = add nuw i64 %v4, 1
  %v12 = and i64 %v11, 4294967295
  %v13 = select i1 %v6, i64 0, i64 %v12
  %v14 = getelementptr [512 x [256 x i8]], ptr %0, i64 0, i64 %v10, i64 255
  %v15 = load i8, ptr %v14, align 1
  %v16 = getelementptr [512 x [256 x i8]], ptr %0, i64 0, i64 %v10, i64 0
  %v17 = load i8, ptr %v16, align 1
  %v18 = add nuw i8 %v17, %v15
  %v19 = getelementptr [512 x [256 x i8]], ptr %0, i64 0, i64 %v10, i64 1
  %v20 = load i8, ptr %v19, align 1
  %v21 = add nuw i8 %v18, %v20
  %v22 = getelementptr [512 x [256 x i8]], ptr %0, i64 0, i64 %v4, i64 255
  %v23 = load i8, ptr %v22, align 1
  %v24 = add nuw i8 %v21, %v23
  %v25 = getelementptr [512 x [256 x i8]], ptr %0, i64 0, i64 %v4, i64 1
  %v26 = load i8, ptr %v25, align 1
  %v27 = add nuw i8 %v24, %v26
  %v28 = getelementptr [512 x [256 x i8]], ptr %0, i64 0, i64 %v13, i64 255
  %v29 = load i8, ptr %v28, align 1
  %v30 = add nuw i8 %v27, %v29
  %v31 = getelementptr [512 x [256 x i8]], ptr %0, i64 0, i64 %v13, i64 0
  %v32 = load i8, ptr %v31, align 1
  %v33 = add nuw i8 %v30, %v32
  %v34 = getelementptr [512 x [256 x i8]], ptr %0, i64 0, i64 %v13, i64 1
  %v35 = load i8, ptr %v34, align 1
  %v36 = add i8 %v33, %v35
  %v37 = getelementptr [512 x [256 x i8]], ptr %0, i64 0, i64 %v4, i64 0
  %v38 = load i8, ptr %v37, align 1
  %v39 = icmp eq i8 %v38, 0
  %v40 = add i8 %v36, -3
  %v41 = icmp ult i8 %v40, 6
  br i1 %v39, label %bb43, label %bb42

bb42:                                             ; preds = %bb3
  br i1 %v41, label %bb44, label %bb54

bb43:                                             ; preds = %bb3
  br i1 %v41, label %bb49, label %bb54

bb44:                                             ; preds = %bb42
  %v45 = shl i8 %v40, 3
  %v46 = zext i8 %v45 to i48
  %v47 = lshr i48 1103823372545, %v46
  %v48 = trunc i48 %v47 to i8
  br label %bb54

bb49:                                             ; preds = %bb43
  %v50 = shl i8 %v40, 3
  %v51 = zext i8 %v50 to i48
  %v52 = lshr i48 1103823372289, %v51
  %v53 = trunc i48 %v52 to i8
  br label %bb54

bb54:                                             ; preds = %bb49, %bb44, %bb43, %bb42
  %v55 = phi i8 [ %v48, %bb44 ], [ 0, %bb42 ], [ %v53, %bb49 ], [ 0, %bb43 ]
  %v56 = getelementptr [512 x [256 x i8]], ptr %1, i64 0, i64 %v4, i64 0
  store i8 %v55, ptr %v56, align 1
  br label %bb60

bb57:                                             ; preds = %bb58
  ret void

bb58:                                             ; preds = %bb109
  %v59 = icmp eq i64 %v11, 512
  br i1 %v59, label %bb57, label %bb3

bb60:                                             ; preds = %bb109, %bb54
  %v61 = phi i64 [ 1, %bb54 ], [ %v71, %bb109 ]
  %v62 = icmp eq i64 %v61, 255
  %v63 = shl i64 %v61, 32
  %v631 = add i64 %v63, -4294967296
  %v65 = ashr i64 %v631, 32
  %v66 = getelementptr [512 x [256 x i8]], ptr %0, i64 0, i64 %v10, i64 %v65
  %v67 = load i8, ptr %v66, align 1
  %v68 = getelementptr [512 x [256 x i8]], ptr %0, i64 0, i64 %v10, i64 %v61
  %v69 = load i8, ptr %v68, align 1
  %v70 = add nuw i8 %v69, %v67
  %v71 = add nuw i64 %v61, 1
  %v72 = and i64 %v71, 4294967295
  %v73 = select i1 %v62, i64 0, i64 %v72
  %v74 = getelementptr [512 x [256 x i8]], ptr %0, i64 0, i64 %v10, i64 %v73
  %v75 = load i8, ptr %v74, align 1
  %v76 = add nuw i8 %v70, %v75
  %v77 = getelementptr [512 x [256 x i8]], ptr %0, i64 0, i64 %v4, i64 %v65
  %v78 = load i8, ptr %v77, align 1
  %v79 = add nuw i8 %v76, %v78
  %v80 = getelementptr [512 x [256 x i8]], ptr %0, i64 0, i64 %v4, i64 %v73
  %v81 = load i8, ptr %v80, align 1
  %v82 = add nuw i8 %v79, %v81
  %v83 = getelementptr [512 x [256 x i8]], ptr %0, i64 0, i64 %v13, i64 %v65
  %v84 = load i8, ptr %v83, align 1
  %v85 = add nuw i8 %v82, %v84
  %v86 = getelementptr [512 x [256 x i8]], ptr %0, i64 0, i64 %v13, i64 %v61
  %v87 = load i8, ptr %v86, align 1
  %v88 = add nuw i8 %v85, %v87
  %v89 = getelementptr [512 x [256 x i8]], ptr %0, i64 0, i64 %v13, i64 %v73
  %v90 = load i8, ptr %v89, align 1
  %v91 = add i8 %v88, %v90
  %v92 = getelementptr [512 x [256 x i8]], ptr %0, i64 0, i64 %v4, i64 %v61
  %v93 = load i8, ptr %v92, align 1
  %v94 = icmp eq i8 %v93, 0
  %v95 = add i8 %v91, -3
  %v96 = icmp ult i8 %v95, 6
  br i1 %v94, label %bb98, label %bb97

bb97:                                             ; preds = %bb60
  br i1 %v96, label %bb99, label %bb109

bb98:                                             ; preds = %bb60
  br i1 %v96, label %bb104, label %bb109

bb99:                                             ; preds = %bb97
  %v100 = shl i8 %v95, 3
  %v101 = zext i8 %v100 to i48
  %v102 = lshr i48 1103823372545, %v101
  %v103 = trunc i48 %v102 to i8
  br label %bb109

bb104:                                            ; preds = %bb98
  %v105 = shl i8 %v95, 3
  %v106 = zext i8 %v105 to i48
  %v107 = lshr i48 1103823372289, %v106
  %v108 = trunc i48 %v107 to i8
  br label %bb109

bb109:                                            ; preds = %bb104, %bb99, %bb98, %bb97
  %v110 = phi i8 [ %v103, %bb99 ], [ 0, %bb97 ], [ %v108, %bb104 ], [ 0, %bb98 ]
  %v111 = getelementptr [512 x [256 x i8]], ptr %1, i64 0, i64 %v4, i64 %v61
  store i8 %v110, ptr %v111, align 1
  %v112 = icmp eq i64 %v71, 256
  br i1 %v112, label %bb58, label %bb60
}

define i32 @main() {
bb0:
  %v1 = alloca { [512 x [256 x i8]] }, align 8
  %v2 = alloca { [512 x [256 x i8]] }, align 8
  call void @sim_init()
  br label %bb3

bb3:                                              ; preds = %bb5, %bb0
  %v4 = phi i64 [ 0, %bb0 ], [ %v6, %bb5 ]
  br label %bb8

bb5:                                              ; preds = %bb8
  %v6 = add nuw i64 %v4, 1
  %v7 = icmp eq i64 %v6, 512
  br i1 %v7, label %bb17, label %bb3

bb8:                                              ; preds = %bb8, %bb3
  %v9 = phi i64 [ 0, %bb3 ], [ %v14, %bb8 ]
  %v10 = call i32 @sim_rand()
  %v11 = getelementptr [512 x [256 x i8]], ptr %v1, i64 0, i64 %v4, i64 %v9
  %v12 = trunc i32 %v10 to i8
  %v13 = and i8 %v12, 1
  store i8 %v13, ptr %v11, align 1
  %v14 = add nuw i64 %v9, 1
  %v15 = icmp eq i64 %v14, 256
  br i1 %v15, label %bb5, label %bb8

bb16:                                             ; preds = %bb53
  call void @sim_exit()
  ret i32 0

bb17:                                             ; preds = %bb53, %bb5
  %v18 = phi i32 [ %v56, %bb53 ], [ 0, %bb5 ]
  %v19 = phi i32 [ %v55, %bb53 ], [ 0, %bb5 ]
  %v20 = icmp eq i32 %v19, 0
  br i1 %v20, label %bb21, label %bb37

bb21:                                             ; preds = %bb24, %bb17
  %v22 = phi i64 [ %v25, %bb24 ], [ 0, %bb17 ]
  %v23 = trunc i64 %v22 to i32
  br label %bb27

bb24:                                             ; preds = %bb27
  %v25 = add nuw i64 %v22, 1
  %v26 = icmp eq i64 %v25, 512
  br i1 %v26, label %bb36, label %bb21

bb27:                                             ; preds = %bb27, %bb21
  %v28 = phi i64 [ 0, %bb21 ], [ %v34, %bb27 ]
  %v29 = getelementptr [512 x [256 x i8]], ptr %v1, i64 0, i64 %v22, i64 %v28
  %v30 = load i8, ptr %v29, align 1
  %v31 = icmp eq i8 %v30, 0
  %v32 = select i1 %v31, i32 -65536, i32 -16711936
  %v33 = trunc i64 %v28 to i32
  call void @sim_put_pixel(i32 %v23, i32 %v33, i32 %v32)
  %v34 = add nuw i64 %v28, 1
  %v35 = icmp eq i64 %v34, 256
  br i1 %v35, label %bb24, label %bb27

bb36:                                             ; preds = %bb24
  call void @get_next_state(ptr %v1, ptr %v2)
  br label %bb53

bb37:                                             ; preds = %bb40, %bb17
  %v38 = phi i64 [ %v41, %bb40 ], [ 0, %bb17 ]
  %v39 = trunc i64 %v38 to i32
  br label %bb43

bb40:                                             ; preds = %bb43
  %v41 = add nuw i64 %v38, 1
  %v42 = icmp eq i64 %v41, 512
  br i1 %v42, label %bb52, label %bb37

bb43:                                             ; preds = %bb43, %bb37
  %v44 = phi i64 [ 0, %bb37 ], [ %v50, %bb43 ]
  %v45 = getelementptr [512 x [256 x i8]], ptr %v2, i64 0, i64 %v38, i64 %v44
  %v46 = load i8, ptr %v45, align 1
  %v47 = icmp eq i8 %v46, 0
  %v48 = select i1 %v47, i32 -65536, i32 -16711936
  %v49 = trunc i64 %v44 to i32
  call void @sim_put_pixel(i32 %v39, i32 %v49, i32 %v48)
  %v50 = add nuw i64 %v44, 1
  %v51 = icmp eq i64 %v50, 256
  br i1 %v51, label %bb40, label %bb43

bb52:                                             ; preds = %bb40
  call void @get_next_state(ptr %v2, ptr %v1)
  br label %bb53

bb53:                                             ; preds = %bb52, %bb36
  call void @sim_flush()
  %v54 = and i32 %v19, 1
  %v55 = xor i32 %v54, 1
  %v56 = add nuw i32 %v18, 1
  %v57 = icmp eq i32 %v56, 10000
  br i1 %v57, label %bb16, label %bb17
}

declare void @sim_init()

declare i32 @sim_rand()

declare void @sim_exit()

declare void @sim_put_pixel(i32, i32, i32)

declare void @sim_flush()
