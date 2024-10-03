// Example from: https://github.com/jamesmth/llvm-plugin-rs/blob/master/examples/hello_world.rs
// See https://github.com/banach-space/llvm-tutor/blob/main/HelloWorld/HelloWorld.cpp
// for a more detailed explanation.

pub mod passes;

use llvm_plugin::{PassBuilder, PipelineParsing};

#[llvm_plugin::plugin(name = "cswl-sim", version = "0.1")]
fn plugin_registrar(builder: &mut PassBuilder) {
    builder.add_module_pipeline_parsing_callback(|name, manager| match name {
        "opcode-counter" => {
            manager.add_pass(passes::opcode_counter::OpcodeCounterPass);
            PipelineParsing::Parsed
        }
        "log-inserter" => {
            manager.add_pass(passes::log_inserter::LogInserterPass);
            PipelineParsing::Parsed
        }
        _ => PipelineParsing::NotParsed,
    });
}
