# https://github.com/quarkslab/llvm-dev-meeting-tutorial-2015/blob/master/Tests/lit.cfg.in

import subprocess
from typing import TYPE_CHECKING, Any

import lit.formats

# https://stackoverflow.com/questions/72824417/ignoring-a-specific-undefined-variable-with-pyright
if TYPE_CHECKING:
    config: Any = None

config.name = "pclc test suite"
config.test_format = lit.formats.ShTest(execute_external=True)
config.suffixes = [".pcl"]


def has_filecheck() -> bool:
    try:
        subprocess.check_output(
            ["FileCheck", "--version"], stderr=subprocess.STDOUT
        ).decode()
    except:  # noqa: E722
        return False
    return True


if has_filecheck():
    config.available_features.add("filecheck")
