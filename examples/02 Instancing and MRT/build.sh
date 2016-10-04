#!/bin/bash
cd "$(dirname -- "$(readlink -fn -- "${0}")")"

function clangs { clang --analyze -Xanalyzer -analyzer-output=text $@ && clang -Werror=conversion -Werror=nullability-completeness -Werror=return-type -Werror=switch -Werror=switch-default -Werror=switch-enum -Werror=uninitialized -Werror=unused-result $@; }
clangs main.c ../../glfast.c flycamera/flycamera.c -lSDL2 -lm ${@}
