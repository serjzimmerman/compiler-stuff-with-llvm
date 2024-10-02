// Similar to: https://github.com/comododragon/opcount

use inkwell::module::Module;
use llvm_plugin::{LlvmModulePass, ModuleAnalysisManager, PreservedAnalyses};
use prettytable::{format::consts::FORMAT_NO_LINESEP_WITH_TITLE, row, Table};
use std::collections::hash_map::HashMap;

struct InstructionInfo {
    count: u64,
}

impl InstructionInfo {
    fn increment(&mut self) {
        self.count += 1;
    }

    fn new() -> Self {
        Self { count: 1 }
    }
}

pub struct OpcodeCounterPass;

impl LlvmModulePass for OpcodeCounterPass {
    fn run_pass(&self, module: &mut Module, _manager: &ModuleAnalysisManager) -> PreservedAnalyses {
        let mut frequency_map: HashMap<_, InstructionInfo> = HashMap::new();

        for instr in module
            .get_functions()
            .map(|function| function.get_basic_blocks())
            .flatten()
            .map(|bb| bb.get_instructions())
            .flatten()
        {
            let opcode = instr.get_opcode();
            frequency_map
                .entry(opcode)
                .and_modify(|e| e.increment())
                .or_insert(InstructionInfo::new());
        }

        let mut table = Table::new();

        table.set_titles(row!["Opcode", "Count"]);

        let mut sorted_map = frequency_map.iter().collect::<Vec<_>>();
        sorted_map.sort_by_key(|(_, info)| info.count);

        for (opcode, InstructionInfo { count }) in sorted_map.iter().rev() {
            let opcode_name = format!("{:?}", opcode);
            table.add_row(row![opcode_name.to_lowercase(), count]);
        }

        table.set_format(*FORMAT_NO_LINESEP_WITH_TITLE);
        table.printstd();

        PreservedAnalyses::All
    }
}
