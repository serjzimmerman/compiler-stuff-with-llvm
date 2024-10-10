use inkwell::{
    builder::{Builder, BuilderError},
    context::Context,
    module::{Linkage, Module},
    support::LLVMString,
    targets::{InitializationConfig, Target},
    types::StringRadix,
    values::{BasicValue, FunctionValue},
    AddressSpace, IntPredicate,
};

use cswl_sim::graphics::{sim_exit, sim_flush, sim_init, sim_put_pixel, sim_rand};

fn get_sim_init_function<'ctx>(module: &'ctx Module) -> FunctionValue<'ctx> {
    let ctx = module.get_context();
    const SIM_INIT_NAME: &str = "sim_init";
    match module.get_function(SIM_INIT_NAME) {
        Some(function) => function,
        None => {
            let func_type = ctx.void_type().fn_type(&[], false);
            module.add_function(SIM_INIT_NAME, func_type, Some(Linkage::External))
        }
    }
}

fn get_sim_exit_function<'ctx>(module: &'ctx Module) -> FunctionValue<'ctx> {
    let ctx = module.get_context();
    const SIM_EXIT_NAME: &str = "sim_exit";
    match module.get_function(SIM_EXIT_NAME) {
        Some(function) => function,
        None => {
            let func_type = ctx.void_type().fn_type(&[], false);
            module.add_function(SIM_EXIT_NAME, func_type, Some(Linkage::External))
        }
    }
}

fn get_sim_flush_function<'ctx>(module: &'ctx Module) -> FunctionValue<'ctx> {
    let ctx = module.get_context();
    const SIM_FLUSH_NAME: &str = "sim_flush";
    match module.get_function(SIM_FLUSH_NAME) {
        Some(function) => function,
        None => {
            let func_type = ctx.void_type().fn_type(&[], false);
            module.add_function(SIM_FLUSH_NAME, func_type, Some(Linkage::External))
        }
    }
}

fn get_sim_rand_function<'ctx>(module: &'ctx Module) -> FunctionValue<'ctx> {
    let ctx = module.get_context();
    const SIM_RAND_NAME: &str = "sim_rand";
    match module.get_function(SIM_RAND_NAME) {
        Some(function) => function,
        None => {
            let func_type = ctx.i32_type().fn_type(&[], false);
            module.add_function(SIM_RAND_NAME, func_type, Some(Linkage::External))
        }
    }
}

fn get_sim_put_pixel_function<'ctx>(module: &'ctx Module) -> FunctionValue<'ctx> {
    let ctx = module.get_context();
    const SIM_PUT_PIXEL: &str = "sim_put_pixel";
    match module.get_function(SIM_PUT_PIXEL) {
        Some(function) => function,
        None => {
            let i32_type = ctx.i32_type().into();
            let func_type = ctx
                .void_type()
                .fn_type(&[i32_type, i32_type, i32_type], false);
            module.add_function(SIM_PUT_PIXEL, func_type, Some(Linkage::External))
        }
    }
}

fn build_main(module: &Module, builder: &Builder) -> Result<(), BuilderError> {
    let ctx = module.get_context();

    // define noundef i32 @main() local_unnamed_addr #0 {
    let func_type = ctx.i32_type().fn_type(&[], false);
    let func_val = module.add_function("main", func_type, Some(Linkage::External));

    let get_next_state_func = module.get_function("get_next_state").unwrap();

    // %struct.board_s = type { [512 x [256 x i8]] }
    let board_s_type = ctx.struct_type(
        &[ctx.i8_type().array_type(256).array_type(512).into()],
        false,
    );

    let bb0 = ctx.append_basic_block(func_val, "bb0");
    let bb3 = ctx.append_basic_block(func_val, "bb3");
    let bb5 = ctx.append_basic_block(func_val, "bb5");
    let bb8 = ctx.append_basic_block(func_val, "bb8");
    let bb16 = ctx.append_basic_block(func_val, "bb16");
    let bb17 = ctx.append_basic_block(func_val, "bb17");
    let bb21 = ctx.append_basic_block(func_val, "bb21");
    let bb24 = ctx.append_basic_block(func_val, "bb24");
    let bb27 = ctx.append_basic_block(func_val, "bb27");
    let bb36 = ctx.append_basic_block(func_val, "bb36");
    let bb37 = ctx.append_basic_block(func_val, "bb37");
    let bb40 = ctx.append_basic_block(func_val, "bb40");
    let bb43 = ctx.append_basic_block(func_val, "bb43");
    let bb52 = ctx.append_basic_block(func_val, "bb52");
    let bb53 = ctx.append_basic_block(func_val, "bb53");

    let i64_0 = ctx.i64_type().const_int(0, false);
    let i32_0 = ctx.i32_type().const_int(0, false);

    builder.position_at_end(bb0);
    // %1 = alloca %struct.board_s, align 1
    let v1 = builder.build_alloca(board_s_type, "v1")?;
    // %2 = alloca %struct.board_s, align 1
    let v2 = builder.build_alloca(board_s_type, "v2")?;
    // tail call void @sim_init() #5
    builder.build_call(get_sim_init_function(&module), &[], "")?;
    // br label %3
    builder.build_unconditional_branch(bb3)?;

    // 3:; preds = %5, %0
    builder.position_at_end(bb3);
    // %4 = phi i64 [ 0, %0 ], [ %6, %5 ]
    let phi4 = builder.build_phi(ctx.i64_type(), "v4")?;
    let v4 = phi4.as_basic_value().into_int_value();
    // br label %8
    builder.build_unconditional_branch(bb8)?;

    // 5:; preds = %8
    builder.position_at_end(bb5);
    // %6 = add nuw nsw i64 %4, 1
    let v6 = builder.build_int_nuw_add(v4, ctx.i64_type().const_int(1, false), "v6")?;
    // %7 = icmp eq i64 %6, 512
    let v7 = builder.build_int_compare(
        IntPredicate::EQ,
        v6,
        ctx.i64_type().const_int(512, false),
        "v7",
    )?;
    // br i1 %7, label %17, label %3, !llvm.loop !4
    builder.build_conditional_branch(v7, bb17, bb3)?;

    // 8:; preds = %8, %3
    builder.position_at_end(bb8);
    // %9 = phi i64 [ 0, %3 ], [ %14, %8 ]
    let phi9 = builder.build_phi(ctx.i64_type(), "v9")?;
    let v9 = phi9.as_basic_value().into_int_value();
    // %10 = tail call i32 @sim_rand() #5
    let v10 = builder
        .build_call(get_sim_rand_function(&module), &[], "v10")?
        .try_as_basic_value()
        .left()
        .unwrap()
        .as_basic_value_enum()
        .into_int_value();

    // %11 = getelementptr [512 x [256 x i8]], ptr %1, i64 0, i64 %4, i64 %9
    let v11 = unsafe {
        builder.build_gep(
            board_s_type.get_field_type_at_index(0).unwrap(),
            v1,
            &[i64_0, v4, v9],
            "v11",
        )
    }?;
    // %12 = trunc i32 %10 to i8
    let v12 = builder.build_int_truncate(v10, ctx.i8_type(), "v12")?;
    // %13 = and i8 %12, 1
    let v13 = builder.build_and(v12, ctx.i8_type().const_int(1, false), "v13")?;
    // store i8 %13, ptr %11, align 1, !tbaa !6
    builder.build_store(v11, v13)?;
    // %14 = add nuw nsw i64 %9, 1
    let v14 = builder.build_int_nuw_add(v9, ctx.i64_type().const_int(1, false), "v14")?;
    // %15 = icmp eq i64 %14, 256
    let v15 = builder.build_int_compare(
        IntPredicate::EQ,
        v14,
        ctx.i64_type().const_int(256, false),
        "v15",
    )?;
    // br i1 %15, label %5, label %8, !llvm.loop !10
    builder.build_conditional_branch(v15, bb5, bb8)?;

    // 16: ; preds = %53
    builder.position_at_end(bb16);
    // tail call void @sim_exit() #5
    builder.build_call(get_sim_exit_function(&module), &[], "")?;
    // ret i32 0
    builder.build_return(Some(&i32_0))?;

    // 17: ; preds = %5, %53
    builder.position_at_end(bb17);
    // %18 = phi i32 [ %56, %53 ], [ 0, %5 ]
    let phi18 = builder.build_phi(ctx.i32_type(), "v18")?;
    let v18 = phi18.as_basic_value().into_int_value();
    // %19 = phi i32 [ %55, %53 ], [ 0, %5 ]
    let phi19 = builder.build_phi(ctx.i32_type(), "v19")?;
    let v19 = phi19.as_basic_value().into_int_value();
    // %20 = icmp eq i32 %19, 0
    let v20 = builder.build_int_compare(IntPredicate::EQ, v19, i32_0, "v20")?;
    // br i1 %20, label %21, label %37
    builder.build_conditional_branch(v20, bb21, bb37)?;

    // 21: ; preds = %17, %24
    builder.position_at_end(bb21);
    // %22 = phi i64 [ %25, %24 ], [ 0, %17 ]
    let phi22 = builder.build_phi(ctx.i64_type(), "v22")?;
    let v22 = phi22.as_basic_value().into_int_value();
    // %23 = trunc i64 %22 to i32
    let v23 = builder.build_int_truncate(v22, ctx.i32_type(), "v23")?;
    // br label %27
    builder.build_unconditional_branch(bb27)?;

    // 24: ; preds = %27
    builder.position_at_end(bb24);
    // %25 = add nuw nsw i64 %22, 1
    let v25 = builder.build_int_nuw_add(
        phi22.as_basic_value().into_int_value(),
        ctx.i64_type().const_int(1, false),
        "v25",
    )?;
    // %26 = icmp eq i64 %25, 512
    let v26 = builder.build_int_compare(
        IntPredicate::EQ,
        v25,
        ctx.i64_type().const_int(512, false),
        "v26",
    )?;
    // br i1 %26, label %36, label %21, !llvm.loop !11
    builder.build_conditional_branch(v26, bb36, bb21)?;

    // 27: ; preds = %27, %21
    builder.position_at_end(bb27);
    // %28 = phi i64 [ 0, %21 ], [ %34, %27 ]
    let phi28 = builder.build_phi(ctx.i64_type(), "v28")?;
    let v28 = phi28.as_basic_value().into_int_value();
    // %29 = getelementptr [512 x [256 x i8]], ptr %1, i64 0, i64 %22, i64 %28
    let v29 = unsafe {
        builder.build_gep(
            board_s_type.get_field_type_at_index(0).unwrap(),
            v1,
            &[i64_0, v22, v28],
            "v29",
        )
    }?;
    // %30 = load i8, ptr %29, align 1, !tbaa !6, !range !12, !noundef !13
    let v30 = builder
        .build_load(ctx.i8_type(), v29, "v30")?
        .into_int_value();
    // %31 = icmp eq i8 %30, 0
    let v31 = builder.build_int_compare(
        IntPredicate::EQ,
        v30,
        ctx.i8_type().const_int(0, false),
        "v31",
    )?;
    // %32 = select i1 %31, i32 -65536, i32 -16711936
    let v32 = builder.build_select(
        v31,
        ctx.i32_type().const_int(0xffff0000, false),
        ctx.i32_type().const_int(0xff00ff00, false),
        "v32",
    )?;
    // %33 = trunc i64 %28 to i32
    let v33 = builder.build_int_truncate(v28, ctx.i32_type(), "v33")?;
    // tail call void @sim_put_pixel(i32 noundef %23, i32 noundef %33, i32 noundef %32) #5
    builder.build_call(
        get_sim_put_pixel_function(&module),
        &[v23.into(), v33.into(), v32.into()],
        "",
    )?;
    // %34 = add nuw nsw i64 %28, 1
    let v34 = builder.build_int_nuw_add(v28, ctx.i64_type().const_int(1, false), "v34")?;
    // %35 = icmp eq i64 %34, 256
    let v35 = builder.build_int_compare(
        IntPredicate::EQ,
        v34,
        ctx.i64_type().const_int(256, false),
        "v35",
    )?;
    // br i1 %35, label %24, label %27, !llvm.loop !14
    builder.build_conditional_branch(v35, bb24, bb27)?;

    // 36: ; preds = %24
    builder.position_at_end(bb36);
    // call fastcc void @get_next_state(ptr noundef nonnull %1, ptr noundef nonnull %2)
    builder.build_call(get_next_state_func, &[v1.into(), v2.into()], "")?;
    // br label %53
    builder.build_unconditional_branch(bb53)?;

    // 37: ; preds = %17, %40
    builder.position_at_end(bb37);
    // %38 = phi i64 [ %41, %40 ], [ 0, %17 ]
    let phi38 = builder.build_phi(ctx.i64_type(), "v38")?;
    let v38 = phi38.as_basic_value().into_int_value();
    // %39 = trunc i64 %38 to i32
    let v39 = builder.build_int_truncate(v38, ctx.i32_type(), "v39")?;
    // br label %43
    builder.build_unconditional_branch(bb43)?;

    // 40: ; preds = %43
    builder.position_at_end(bb40);
    // %41 = add nuw nsw i64 %38, 1
    let v41 = builder.build_int_nuw_add(v38, ctx.i64_type().const_int(1, false), "v41")?;
    // %42 = icmp eq i64 %41, 512
    let v42 = builder.build_int_compare(
        IntPredicate::EQ,
        v41,
        ctx.i64_type().const_int(512, false),
        "v42",
    )?;
    // br i1 %42, label %52, label %37, !llvm.loop !11
    builder.build_conditional_branch(v42, bb52, bb37)?;

    // 43: ; preds = %43, %37
    builder.position_at_end(bb43);
    // %44 = phi i64 [ 0, %37 ], [ %50, %43 ]
    let phi44 = builder.build_phi(ctx.i64_type(), "v44")?;
    let v44 = phi44.as_basic_value().into_int_value();
    // %45 = getelementptr [512 x [256 x i8]], ptr %2, i64 0, i64 %38, i64 %44
    let v45 = unsafe {
        builder.build_gep(
            board_s_type.get_field_type_at_index(0).unwrap(),
            v2,
            &[i64_0, v38, v44],
            "v45",
        )
    }?;
    // %46 = load i8, ptr %45, align 1, !tbaa !6, !range !12, !noundef !13
    let v46 = builder
        .build_load(ctx.i8_type(), v45, "v46")?
        .into_int_value();
    // %47 = icmp eq i8 %46, 0
    let v47 = builder.build_int_compare(
        IntPredicate::EQ,
        v46,
        ctx.i8_type().const_int(0, false),
        "v47",
    )?;
    // %48 = select i1 %47, i32 -65536, i32 -16711936
    let v48 = builder.build_select(
        v47,
        ctx.i32_type().const_int(0xffff0000, false),
        ctx.i32_type().const_int(0xff00ff00, false),
        "v48",
    )?;
    // %49 = trunc i64 %44 to i32
    let v49 = builder.build_int_truncate(v44, ctx.i32_type(), "v49")?;
    // tail call void @sim_put_pixel(i32 noundef %39, i32 noundef %49, i32 noundef %48) #5
    builder.build_call(
        get_sim_put_pixel_function(&module),
        &[v39.into(), v49.into(), v48.into()],
        "",
    )?;
    // %50 = add nuw nsw i64 %44, 1
    let v50 = builder.build_int_nuw_add(v44, ctx.i64_type().const_int(1, false), "v50")?;
    // %51 = icmp eq i64 %50, 256
    let v51 = builder.build_int_compare(
        IntPredicate::EQ,
        v50,
        ctx.i64_type().const_int(256, false),
        "v51",
    )?;
    // br i1 %51, label %40, label %43, !llvm.loop !14
    builder.build_conditional_branch(v51, bb40, bb43)?;

    // 52: ; preds = %40
    builder.position_at_end(bb52);
    // call fastcc void @get_next_state(ptr noundef nonnull %2, ptr noundef nonnull %1)
    builder.build_call(get_next_state_func, &[v2.into(), v1.into()], "")?;
    // br label %53
    builder.build_unconditional_branch(bb53)?;

    // 53: ; preds = %52, %36
    builder.position_at_end(bb53);
    // tail call void @sim_flush() #5
    builder.build_call(get_sim_flush_function(&module), &[], "")?;
    // %54 = and i32 %19, 1
    let v54 = builder.build_and(v19, ctx.i32_type().const_int(1, false), "v54")?;
    // %55 = xor i32 %54, 1
    let v55 = builder.build_xor(v54, ctx.i32_type().const_int(1, false), "v55")?;
    // %56 = add nuw nsw i32 %18, 1
    let v56 = builder.build_int_nuw_add(v18, ctx.i32_type().const_int(1, false), "v56")?;
    // %57 = icmp eq i32 %56, 10000
    let v57 = builder.build_int_compare(
        IntPredicate::EQ,
        v56,
        ctx.i32_type().const_int(10000, false),
        "v57",
    )?;

    // br i1 %57, label %16, label %17, !llvm.loop !15
    builder.build_conditional_branch(v57, bb16, bb17)?;

    // %4 = phi i64 [ 0, %0 ], [ %6, %5 ]
    phi4.add_incoming(&[(&i64_0, bb0), (&v6, bb5)]);

    // %9 = phi i64 [ 0, %3 ], [ %14, %8 ]
    phi9.add_incoming(&[(&i64_0, bb3), (&v14, bb8)]);

    // %18 = phi i32 [ %56, %53 ], [ 0, %5 ]
    phi18.add_incoming(&[(&v56, bb53), (&i32_0, bb5)]);

    // %19 = phi i32 [ %55, %53 ], [ 0, %5 ]
    phi19.add_incoming(&[(&v55, bb53), (&i32_0, bb5)]);

    // %22 = phi i64 [ %25, %24 ], [ 0, %17 ]
    phi22.add_incoming(&[(&v25, bb24), (&i64_0, bb17)]);

    // %28 = phi i64 [ 0, %21 ], [ %34, %27 ]
    phi28.add_incoming(&[(&i64_0, bb21), (&v34, bb27)]);

    // %38 = phi i64 [ %41, %40 ], [ 0, %17 ]
    phi38.add_incoming(&[(&v41, bb40), (&i64_0, bb17)]);

    // %44 = phi i64 [ 0, %37 ], [ %50, %43 ]
    phi44.add_incoming(&[(&i64_0, bb37), (&v50, bb43)]);

    Ok(())
}

#[derive(Debug)]
#[allow(dead_code)]
enum Error {
    StringError(LLVMString),
    BuilderError(BuilderError),
}

impl From<BuilderError> for Error {
    fn from(e: BuilderError) -> Self {
        Error::BuilderError(e)
    }
}

impl From<LLVMString> for Error {
    fn from(e: LLVMString) -> Self {
        Error::StringError(e)
    }
}

fn build_get_next_state(module: &Module, builder: &Builder) -> Result<(), BuilderError> {
    let ctx = module.get_context();

    // define internal fastcc void @get_next_state(ptr nocapture noundef readonly %0, ptr nocapture noundef writeonly %1) unnamed_addr #4 {
    let func_type = ctx.void_type().fn_type(
        &[
            ctx.ptr_type(AddressSpace::default()).into(),
            ctx.ptr_type(AddressSpace::default()).into(),
        ],
        false,
    );

    let func_val = module.add_function("get_next_state", func_type, Some(Linkage::Internal));

    let v0 = func_val.get_nth_param(0).unwrap().into_pointer_value();
    let v1 = func_val.get_nth_param(1).unwrap().into_pointer_value();

    let bb2 = ctx.append_basic_block(func_val, "bb2");
    let bb3 = ctx.append_basic_block(func_val, "bb3");
    let bb42 = ctx.append_basic_block(func_val, "bb42");
    let bb43 = ctx.append_basic_block(func_val, "bb43");
    let bb44 = ctx.append_basic_block(func_val, "bb44");
    let bb49 = ctx.append_basic_block(func_val, "bb49");
    let bb54 = ctx.append_basic_block(func_val, "bb54");
    let bb57 = ctx.append_basic_block(func_val, "bb57");
    let bb58 = ctx.append_basic_block(func_val, "bb58");
    let bb60 = ctx.append_basic_block(func_val, "bb60");
    let bb97 = ctx.append_basic_block(func_val, "bb97");
    let bb98 = ctx.append_basic_block(func_val, "bb98");
    let bb99 = ctx.append_basic_block(func_val, "bb99");
    let bb104 = ctx.append_basic_block(func_val, "bb104");
    let bb109 = ctx.append_basic_block(func_val, "bb109");

    builder.position_at_end(bb2);
    // br label %3
    builder.build_unconditional_branch(bb3)?;

    let i64_0 = ctx.i64_type().const_int(0, false);
    let i8_0 = ctx.i8_type().const_int(0, false);
    let i64_1 = ctx.i64_type().const_int(1, false);
    let i64_255 = ctx.i64_type().const_int(255, false);
    let i64_256 = ctx.i64_type().const_int(256, false);
    let i64_511 = ctx.i64_type().const_int(511, false);
    let i64_512 = ctx.i64_type().const_int(512, false);
    let i64_32 = ctx.i64_type().const_int(32, false);

    // 3:                                                ; preds = %2, %58
    builder.position_at_end(bb3);
    // %4 = phi i64 [ 0, %2 ], [ %11, %58 ]
    let phi4 = builder.build_phi(ctx.i64_type(), "v4")?;
    let v4 = phi4.as_basic_value().into_int_value();
    // %5 = icmp eq i64 %4, 0
    let v5 = builder.build_int_compare(IntPredicate::EQ, v4, i64_0, "v5")?;
    // %6 = icmp eq i64 %4, 511
    let v6 = builder.build_int_compare(IntPredicate::EQ, v4, i64_511, "v6")?;
    // %7 = shl i64 %4, 32
    let v7 = builder.build_left_shift(v4, ctx.i64_type().const_int(32, false), "v7")?;
    // %8 = add i64 %7, -4294967296
    let v8 = builder.build_int_add(
        v7,
        ctx.i64_type()
            .const_int_from_string("-4294967296", StringRadix::Decimal)
            .unwrap(),
        "v8",
    )?;
    // %9 = ashr exact i64 %8, 32
    let v9 = builder.build_right_shift(v8, ctx.i64_type().const_int(32, false), true, "v9")?;
    // %10 = select i1 %5, i64 511, i64 %9
    let v10 = builder
        .build_select(v5, i64_511, v9, "v10")?
        .into_int_value();
    // %11 = add nuw nsw i64 %4, 1
    let v11 = builder.build_int_nuw_add(v4, i64_1, "v11")?;
    // %12 = and i64 %11, 4294967295
    let v12 = builder.build_and(v11, ctx.i64_type().const_int(4294967295, false), "v12")?;
    // %13 = select i1 %6, i64 0, i64 %12
    let v13 = builder
        .build_select(v6, i64_0, v12, "v13")?
        .into_int_value();

    let board_s_type = ctx.struct_type(
        &[ctx.i8_type().array_type(256).array_type(512).into()],
        false,
    );

    let build_gep = |idx1, idx2, name| unsafe {
        builder.build_gep(
            board_s_type.get_field_type_at_index(0).unwrap(),
            v0,
            &[i64_0, idx1, idx2],
            name,
        )
    };

    // %14 = getelementptr [512 x [256 x i8]], ptr %0, i64 0, i64 %10, i64 255
    let v14 = build_gep(v10, i64_255, "v14")?;
    // %15 = load i8, ptr %14, align 1, !tbaa !6, !range !12, !noundef !13
    let v15 = builder
        .build_load(ctx.i8_type(), v14, "v15")?
        .into_int_value();
    // %16 = getelementptr [512 x [256 x i8]], ptr %0, i64 0, i64 %10, i64 0
    let v16 = build_gep(v10, i64_0, "v16")?;
    // %17 = load i8, ptr %16, align 1, !tbaa !6, !range !12, !noundef !13
    let v17 = builder
        .build_load(ctx.i8_type(), v16, "v17")?
        .into_int_value();
    // %18 = add nuw nsw i8 %17, %15
    let v18 = builder.build_int_nuw_add(v17, v15, "v18")?;
    // %19 = getelementptr [512 x [256 x i8]], ptr %0, i64 0, i64 %10, i64 1
    let v19 = build_gep(v10, i64_1, "v19")?;
    // %20 = load i8, ptr %19, align 1, !tbaa !6, !range !12, !noundef !13
    let v20 = builder
        .build_load(ctx.i8_type(), v19, "v20")?
        .into_int_value();
    // %21 = add nuw nsw i8 %18, %20
    let v21 = builder.build_int_nuw_add(v18, v20, "v21")?;
    // %22 = getelementptr [512 x [256 x i8]], ptr %0, i64 0, i64 %4, i64 255
    let v22 = build_gep(v4, i64_255, "v22")?;
    // %23 = load i8, ptr %22, align 1, !tbaa !6, !range !12, !noundef !13
    let v23 = builder
        .build_load(ctx.i8_type(), v22, "v23")?
        .into_int_value();
    // %24 = add nuw nsw i8 %21, %23
    let v24 = builder.build_int_nuw_add(v21, v23, "v24")?;
    // %25 = getelementptr [512 x [256 x i8]], ptr %0, i64 0, i64 %4, i64 1
    let v25 = build_gep(v4, i64_1, "v25")?;
    // %26 = load i8, ptr %25, align 1, !tbaa !6, !range !12, !noundef !13
    let v26 = builder
        .build_load(ctx.i8_type(), v25, "v26")?
        .into_int_value();
    // %27 = add nuw nsw i8 %24, %26
    let v27 = builder.build_int_nuw_add(v24, v26, "v27")?;
    // %28 = getelementptr [512 x [256 x i8]], ptr %0, i64 0, i64 %13, i64 255
    let v28 = build_gep(v13, i64_255, "v28")?;
    // %29 = load i8, ptr %28, align 1, !tbaa !6, !range !12, !noundef !13
    let v29 = builder
        .build_load(ctx.i8_type(), v28, "v29")?
        .into_int_value();
    // %30 = add nuw nsw i8 %27, %29
    let v30 = builder.build_int_nuw_add(v27, v29, "v30")?;
    // %31 = getelementptr [512 x [256 x i8]], ptr %0, i64 0, i64 %13, i64 0
    let v31 = build_gep(v13, i64_0, "v31")?;
    // %32 = load i8, ptr %31, align 1, !tbaa !6, !range !12, !noundef !13
    let v32 = builder
        .build_load(ctx.i8_type(), v31, "v32")?
        .into_int_value();
    // %33 = add nuw nsw i8 %30, %32
    let v33 = builder.build_int_nuw_add(v30, v32, "v33")?;
    // %34 = getelementptr [512 x [256 x i8]], ptr %0, i64 0, i64 %13, i64 1
    let v34 = build_gep(v13, i64_1, "v34")?;
    // %35 = load i8, ptr %34, align 1, !tbaa !6, !range !12, !noundef !13
    let v35 = builder
        .build_load(ctx.i8_type(), v34, "v35")?
        .into_int_value();
    // %36 = add i8 %33, %35
    let v36 = builder.build_int_add(v33, v35, "v36")?;
    // %37 = getelementptr [512 x [256 x i8]], ptr %0, i64 0, i64 %4, i64 0
    let v37 = build_gep(v4, i64_0, "v37")?;
    // %38 = load i8, ptr %37, align 1, !tbaa !6, !range !12, !noundef !13
    let v38 = builder
        .build_load(ctx.i8_type(), v37, "v38")?
        .into_int_value();
    // %39 = icmp eq i8 %38, 0
    let v39 = builder.build_int_compare(IntPredicate::EQ, v38, i8_0, "v39")?;
    // %40 = add i8 %36, -3
    let v40 = builder.build_int_add(
        v36,
        ctx.i8_type()
            .const_int_from_string("-3", StringRadix::Decimal)
            .unwrap(),
        "v40",
    )?;
    // %41 = icmp ult i8 %40, 6
    let v41 = builder.build_int_compare(
        IntPredicate::ULT,
        v40,
        ctx.i8_type().const_int(6, false),
        "v41",
    )?;
    // br i1 %39, label %43, label %42
    builder.build_conditional_branch(v39, bb43, bb42)?;

    // 42:                                               ; preds = %3
    builder.position_at_end(bb42);
    // br i1 %41, label %44, label %54
    builder.build_conditional_branch(v41, bb44, bb54)?;

    // 43:                                               ; preds = %3
    builder.position_at_end(bb43);
    // br i1 %41, label %49, label %54
    builder.build_conditional_branch(v41, bb49, bb54)?;

    // 44:                                               ; preds = %42
    builder.position_at_end(bb44);
    // %45 = shl nuw nsw i8 %40, 3
    let v45 = builder.build_left_shift(v40, ctx.i8_type().const_int(3, false), "v45")?;
    // %46 = zext nneg i8 %45 to i48
    let v46 = builder.build_int_z_extend(v45, ctx.custom_width_int_type(48), "v46")?;
    // %47 = lshr i48 1103823372545, %46
    let v47 = builder.build_right_shift(
        ctx.custom_width_int_type(48)
            .const_int(1103823372545, false),
        v46,
        false,
        "v47",
    )?;
    // %48 = trunc i48 %47 to i8
    let v48 = builder.build_int_truncate(v47, ctx.i8_type(), "v48")?;
    // br label %54
    builder.build_unconditional_branch(bb54)?;

    // 49:                                               ; preds = %43
    builder.position_at_end(bb49);
    // %50 = shl nuw nsw i8 %40, 3
    let v50 = builder.build_left_shift(v40, ctx.i8_type().const_int(3, false), "v50")?;
    // %51 = zext nneg i8 %50 to i48
    let v51 = builder.build_int_z_extend(v50, ctx.custom_width_int_type(48), "v51")?;
    // %52 = lshr i48 1103823372289, %51
    let v52 = builder.build_right_shift(
        ctx.custom_width_int_type(48)
            .const_int(1103823372289, false),
        v51,
        false,
        "v52",
    )?;
    // %53 = trunc i48 %52 to i8
    let v53 = builder.build_int_truncate(v52, ctx.i8_type(), "v53")?;
    // br label %54
    builder.build_unconditional_branch(bb54)?;

    // 54:                                               ; preds = %43, %49, %42, %44
    builder.position_at_end(bb54);
    // %55 = phi i8 [ %48, %44 ], [ 0, %42 ], [ %53, %49 ], [ 0, %43 ]
    let phi55 = builder.build_phi(ctx.i8_type(), "v55")?;
    let v55 = phi55.as_basic_value().into_int_value();
    // %56 = getelementptr [512 x [256 x i8]], ptr %1, i64 0, i64 %4, i64 0
    let v56 = unsafe {
        builder.build_gep(
            board_s_type.get_field_type_at_index(0).unwrap(),
            v1,
            &[i64_0, v4, i64_0],
            "v56",
        )
    }?;
    // store i8 %55, ptr %56, align 1, !tbaa !6
    builder.build_store(v56, v55)?;
    // br label %60
    builder.build_unconditional_branch(bb60)?;

    // 57:                                               ; preds = %58
    builder.position_at_end(bb57);
    // ret void
    builder.build_return(None)?;

    // 58:                                               ; preds = %109
    builder.position_at_end(bb58);
    // %59 = icmp eq i64 %11, 512
    let v59 = builder.build_int_compare(IntPredicate::EQ, v11, i64_512, "v59")?;
    // br i1 %59, label %57, label %3, !llvm.loop !16
    builder.build_conditional_branch(v59, bb57, bb3)?;

    // 60:                                               ; preds = %54, %109
    builder.position_at_end(bb60);
    // %61 = phi i64 [ 1, %54 ], [ %71, %109 ]
    let phi61 = builder.build_phi(ctx.i64_type(), "v61")?;
    let v61 = phi61.as_basic_value().into_int_value();
    // %62 = icmp eq i64 %61, 255
    let v62 = builder.build_int_compare(IntPredicate::EQ, v61, i64_255, "v62")?;
    // %63 = shl i64 %61, 32
    let v63 = builder.build_left_shift(v61, i64_32, "v63")?;
    // %64 = add i64 %63, -4294967296
    let v64 = builder.build_int_add(
        v63,
        ctx.i64_type()
            .const_int_from_string("-4294967296", StringRadix::Decimal)
            .unwrap(),
        "v63",
    )?;
    // %65 = ashr exact i64 %64, 32
    let v65 = builder.build_right_shift(v64, i64_32, true, "v65")?;

    let build_gep = |idx1, idx2, name| unsafe {
        builder.build_gep(
            board_s_type.get_field_type_at_index(0).unwrap(),
            v0,
            &[i64_0, idx1, idx2],
            name,
        )
    };

    // %66 = getelementptr [512 x [256 x i8]], ptr %0, i64 0, i64 %10, i64 %65
    let v66 = build_gep(v10, v65, "v66")?;
    // %67 = load i8, ptr %66, align 1, !tbaa !6, !range !12, !noundef !13
    let v67 = builder
        .build_load(ctx.i8_type(), v66, "v67")?
        .into_int_value();
    // %68 = getelementptr [512 x [256 x i8]], ptr %0, i64 0, i64 %10, i64 %61
    let v68 = build_gep(v10, v61, "v68")?;
    // %69 = load i8, ptr %68, align 1, !tbaa !6, !range !12, !noundef !13
    let v69 = builder
        .build_load(ctx.i8_type(), v68, "v69")?
        .into_int_value();
    // %70 = add nuw nsw i8 %69, %67
    let v70 = builder.build_int_nuw_add(v69, v67, "v70")?;
    // %71 = add nuw nsw i64 %61, 1
    let v71 = builder.build_int_nuw_add(v61, i64_1, "v71")?;
    // %72 = and i64 %71, 4294967295
    let v72 = builder.build_and(v71, ctx.i64_type().const_int(4294967295, false), "v72")?;
    // %73 = select i1 %62, i64 0, i64 %72
    let v73 = builder
        .build_select(v62, i64_0, v72, "v73")?
        .into_int_value();
    // %74 = getelementptr [512 x [256 x i8]], ptr %0, i64 0, i64 %10, i64 %73
    let v74 = build_gep(v10, v73, "v74")?;
    // %75 = load i8, ptr %74, align 1, !tbaa !6, !range !12, !noundef !13
    let v75 = builder
        .build_load(ctx.i8_type(), v74, "v75")?
        .into_int_value();
    // %76 = add nuw nsw i8 %70, %75
    let v76 = builder.build_int_nuw_add(v70, v75, "v76")?;
    // %77 = getelementptr [512 x [256 x i8]], ptr %0, i64 0, i64 %4, i64 %65
    let v77 = build_gep(v4, v65, "v77")?;
    // %78 = load i8, ptr %77, align 1, !tbaa !6, !range !12, !noundef !13
    let v78 = builder
        .build_load(ctx.i8_type(), v77, "v78")?
        .into_int_value();
    // %79 = add nuw nsw i8 %76, %78
    let v79 = builder.build_int_nuw_add(v76, v78, "v79")?;
    // %80 = getelementptr [512 x [256 x i8]], ptr %0, i64 0, i64 %4, i64 %73
    let v80 = build_gep(v4, v73, "v80")?;
    // %81 = load i8, ptr %80, align 1, !tbaa !6, !range !12, !noundef !13
    let v81 = builder
        .build_load(ctx.i8_type(), v80, "v81")?
        .into_int_value();
    // %82 = add nuw nsw i8 %79, %81
    let v82 = builder.build_int_nuw_add(v79, v81, "v82")?;
    // %83 = getelementptr [512 x [256 x i8]], ptr %0, i64 0, i64 %13, i64 %65
    let v83 = build_gep(v13, v65, "v83")?;
    // %84 = load i8, ptr %83, align 1, !tbaa !6, !range !12, !noundef !13
    let v84 = builder
        .build_load(ctx.i8_type(), v83, "v84")?
        .into_int_value();
    // %85 = add nuw nsw i8 %82, %84
    let v85 = builder.build_int_nuw_add(v82, v84, "v85")?;
    // %86 = getelementptr [512 x [256 x i8]], ptr %0, i64 0, i64 %13, i64 %61
    let v86 = build_gep(v13, v61, "v86")?;
    // %87 = load i8, ptr %86, align 1, !tbaa !6, !range !12, !noundef !13
    let v87 = builder
        .build_load(ctx.i8_type(), v86, "v87")?
        .into_int_value();
    // %88 = add nuw nsw i8 %85, %87
    let v88 = builder.build_int_nuw_add(v85, v87, "v88")?;
    // %89 = getelementptr [512 x [256 x i8]], ptr %0, i64 0, i64 %13, i64 %73
    let v89 = build_gep(v13, v73, "v89")?;
    // %90 = load i8, ptr %89, align 1, !tbaa !6, !range !12, !noundef !13
    let v90 = builder
        .build_load(ctx.i8_type(), v89, "v90")?
        .into_int_value();
    // %91 = add i8 %88, %90
    let v91 = builder.build_int_add(v88, v90, "v91")?;
    // %92 = getelementptr [512 x [256 x i8]], ptr %0, i64 0, i64 %4, i64 %61
    let v92 = build_gep(v4, v61, "v92")?;
    // %93 = load i8, ptr %92, align 1, !tbaa !6, !range !12, !noundef !13
    let v93 = builder
        .build_load(ctx.i8_type(), v92, "v93")?
        .into_int_value();
    // %94 = icmp eq i8 %93, 0
    let v94 = builder.build_int_compare(IntPredicate::EQ, v93, i8_0, "v94")?;
    // %95 = add i8 %91, -3
    let v95 = builder.build_int_add(
        v91,
        ctx.i8_type()
            .const_int_from_string("-3", StringRadix::Decimal)
            .unwrap(),
        "v95",
    )?;
    // %96 = icmp ult i8 %95, 6
    let v96 = builder.build_int_compare(
        IntPredicate::ULT,
        v95,
        ctx.i8_type().const_int(6, false),
        "v96",
    )?;
    // br i1 %94, label %98, label %97
    builder.build_conditional_branch(v94, bb98, bb97)?;

    // 97:                                               ; preds = %60
    builder.position_at_end(bb97);
    // br i1 %96, label %99, label %109
    builder.build_conditional_branch(v96, bb99, bb109)?;

    // 98:                                               ; preds = %60
    builder.position_at_end(bb98);
    // br i1 %96, label %104, label %109
    builder.build_conditional_branch(v96, bb104, bb109)?;

    // 99:                                               ; preds = %97
    builder.position_at_end(bb99);
    // %100 = shl nuw nsw i8 %95, 3
    let v100 = builder.build_left_shift(v95, ctx.i8_type().const_int(3, false), "v100")?;
    // %101 = zext nneg i8 %100 to i48
    let v101 = builder.build_int_z_extend(v100, ctx.custom_width_int_type(48), "v101")?;
    // %102 = lshr i48 1103823372545, %101
    let v102 = builder.build_right_shift(
        ctx.custom_width_int_type(48)
            .const_int(1103823372545, false),
        v101,
        false,
        "v102",
    )?;
    // %103 = trunc i48 %102 to i8
    let v103 = builder.build_int_truncate(v102, ctx.i8_type(), "v103")?;
    // br label %109
    builder.build_unconditional_branch(bb109)?;

    // 104:                                              ; preds = %98
    builder.position_at_end(bb104);
    // %105 = shl nuw nsw i8 %95, 3
    let v105 = builder.build_left_shift(v95, ctx.i8_type().const_int(3, false), "v105")?;
    // %106 = zext nneg i8 %105 to i48
    let v106 = builder.build_int_z_extend(v105, ctx.custom_width_int_type(48), "v106")?;
    // %107 = lshr i48 1103823372289, %106
    let v107 = builder.build_right_shift(
        ctx.custom_width_int_type(48)
            .const_int(1103823372289, false),
        v106,
        false,
        "v107",
    )?;
    // %108 = trunc i48 %107 to i8
    let v108 = builder.build_int_truncate(v107, ctx.i8_type(), "v108")?;
    // br label %109
    builder.build_unconditional_branch(bb109)?;

    // 109:                                              ; preds = %98, %104, %97, %99
    builder.position_at_end(bb109);
    // %110 = phi i8 [ %103, %99 ], [ 0, %97 ], [ %108, %104 ], [ 0, %98 ]
    let phi110 = builder.build_phi(ctx.i8_type(), "v110")?;
    let v110 = phi110.as_basic_value().into_int_value();
    // %111 = getelementptr [512 x [256 x i8]], ptr %1, i64 0, i64 %4, i64 %61
    let v111 = unsafe {
        builder.build_gep(
            board_s_type.get_field_type_at_index(0).unwrap(),
            v1,
            &[i64_0, v4, v61],
            "v111",
        )
    }?;
    // store i8 %110, ptr %111, align 1, !tbaa !6
    builder.build_store(v111, v110)?;
    // %112 = icmp eq i64 %71, 256
    let v112 = builder.build_int_compare(IntPredicate::EQ, v71, i64_256, "v112")?;
    // br i1 %112, label %58, label %60, !llvm.loop !17
    builder.build_conditional_branch(v112, bb58, bb60)?;

    // %4 = phi i64 [ 0, %2 ], [ %11, %58 ]
    phi4.add_incoming(&[(&i64_0, bb2), (&v11, bb58)]);

    // %55 = phi i8 [ %48, %44 ], [ 0, %42 ], [ %53, %49 ], [ 0, %43 ]
    phi55.add_incoming(&[(&v48, bb44), (&i8_0, bb42), (&v53, bb49), (&i8_0, bb43)]);

    // %61 = phi i64 [ 1, %54 ], [ %71, %109 ]
    phi61.add_incoming(&[(&i64_1, bb54), (&v71, bb109)]);

    // %110 = phi i8 [ %103, %99 ], [ 0, %97 ], [ %108, %104 ], [ 0, %98 ]
    phi110.add_incoming(&[(&v103, bb99), (&i8_0, bb97), (&v108, bb104), (&i8_0, bb98)]);

    Ok(())
}

fn run_with_execution_engine(module: &Module) -> Result<(), LLVMString> {
    let ee = module.create_execution_engine()?;

    ee.add_global_mapping(&get_sim_exit_function(&module), sim_exit as usize);
    ee.add_global_mapping(&get_sim_flush_function(&module), sim_flush as usize);
    ee.add_global_mapping(&get_sim_init_function(&module), sim_init as usize);
    ee.add_global_mapping(&get_sim_rand_function(&module), sim_rand as usize);
    ee.add_global_mapping(&get_sim_put_pixel_function(&module), sim_put_pixel as usize);

    unsafe {
        ee.run_function_as_main(module.get_function("main").unwrap(), &[]);
    };

    Ok(())
}

fn main() -> Result<(), Error> {
    Target::initialize_native(&InitializationConfig::default()).unwrap();

    let ctx = Context::create();
    let module = ctx.create_module("day_night_irgen");
    let builder = ctx.create_builder();

    build_get_next_state(&module, &builder)?;
    build_main(&module, &builder)?;

    let ll = module.print_to_string();
    print!("{}", ll.to_str().unwrap());

    run_with_execution_engine(&module).map_err(|e| e.into())
}
