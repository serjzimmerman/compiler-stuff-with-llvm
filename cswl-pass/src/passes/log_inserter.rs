use inkwell::{
    module::{Linkage, Module},
    values::{FunctionValue, GlobalValue, InstructionOpcode},
    AddressSpace,
};
use llvm_plugin::{LlvmModulePass, ModuleAnalysisManager, PreservedAnalyses};
use std::collections::HashMap;

pub struct LogInserterPass;

const LOG_FUNCTION_NAME: &str = "cswl_pass_log_instrumentation";

impl LlvmModulePass for LogInserterPass {
    fn run_pass(&self, module: &mut Module, _manager: &ModuleAnalysisManager) -> PreservedAnalyses {
        let mut inserted_one_logger_call = false;
        let mut opcode_name_values = HashMap::new();

        for function in module.get_functions() {
            match self.run_on_function(module, function, &mut opcode_name_values) {
                PreservedAnalyses::None => {
                    inserted_one_logger_call = true;
                }
                _ => {}
            }
        }

        inserted_one_logger_call
            .then_some(PreservedAnalyses::None)
            .unwrap_or(PreservedAnalyses::All)
    }
}

fn get_logger_function<'ctx>(module: &'ctx mut Module) -> FunctionValue<'ctx> {
    let context = module.get_context();

    match module.get_function(LOG_FUNCTION_NAME) {
        Some(function) => function,
        None => {
            let ptr_type = context.ptr_type(AddressSpace::default());
            let size_type = context.i64_type();
            let function_type = context
                .void_type()
                .fn_type(&[ptr_type.into(), size_type.into()], false);
            module.add_function(LOG_FUNCTION_NAME, function_type, Some(Linkage::External))
        }
    }
}

struct OpcodeNameAndLength<'ctx> {
    string_value: GlobalValue<'ctx>,
    length: u64,
}

impl LogInserterPass {
    fn run_on_function<'ctx>(
        &self,
        module: &mut Module<'ctx>,
        function: FunctionValue,
        opcode_name_values: &mut HashMap<InstructionOpcode, OpcodeNameAndLength<'ctx>>,
    ) -> PreservedAnalyses {
        if function.is_undef() {
            return PreservedAnalyses::All;
        }

        let name = function.get_name().to_str().unwrap();
        if name == LOG_FUNCTION_NAME {
            return PreservedAnalyses::All;
        }

        let context = module.get_context();
        let builder = context.create_builder();
        let logger = get_logger_function(module);

        for instr in function
            .get_basic_blocks()
            .iter()
            .map(|bb| bb.get_instructions())
            .flatten()
        {
            builder.position_before(&instr);
            match instr.get_opcode() {
                InstructionOpcode::Phi => {
                    continue;
                }
                opcode => {
                    let name_with_length = opcode_name_values.entry(opcode).or_insert_with(|| {
                        let opcode_name = format!("{:?}", opcode).to_lowercase();
                        let name_value = builder.build_global_string_ptr(&opcode_name, "").unwrap();
                        OpcodeNameAndLength {
                            string_value: name_value,
                            length: opcode_name.len() as u64,
                        }
                    });

                    let length_value = context.i64_type().const_int(name_with_length.length, false);

                    let _ = builder.build_call(
                        logger,
                        &[
                            name_with_length.string_value.as_pointer_value().into(),
                            length_value.into(),
                        ],
                        "",
                    );
                }
            }
        }

        PreservedAnalyses::None
    }
}
