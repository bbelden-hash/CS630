# CS 630/730 — HW0 build/grading environment
#
# Build:
#   docker build -t cs630-build:2026fa .
#
# Student self-check, submission mounted at /work (see README.md for more):
#   docker run --rm -v "$PWD/submission:/work:ro" -w /work cs630-build:2026fa \
#       hw0_check.sh ./hw0_toy_machine
#
# Interactive shell for debugging a submission:
#   docker run --rm -it -v "$PWD/submission:/work" -w /work cs630-build:2026fa bash

FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive \
    LANG=C.UTF-8 \
    LC_ALL=C.UTF-8

# --- Toolchains -------------------------------------------------------------
# build-essential   -> gcc/g++/make for C/C++ submissions
# python3           -> run Python submissions
# default-jdk       -> javac/java for Java submissions
# ocaml/dune/findlib-> build OCaml submissions (and the OCaml grading oracle)
# openssl/coreutils/bash -> required by hw0_check.sh / hw0_grade.sh (base64
#   decode via openssl/base64, GNU timeout)
# perl/gzip -> not required by the grading scripts themselves; kept as
#   general-purpose scripting/archive tools for later homeworks
RUN apt-get update && apt-get install -y --no-install-recommends \
        build-essential \
        python3 \
        python3-pip \
        default-jdk \
        ocaml-nox \
        ocaml-findlib \
        ocaml-dune \
        perl \
        gzip \
        openssl \
        coreutils \
        bash \
        ca-certificates \
        git \
        vim-tiny \
    && rm -rf /var/lib/apt/lists/*

# --- Grading script ----------------------------------------------------------
COPY hw0_check.sh /usr/local/bin/hw0_check.sh
RUN chmod 755 /usr/local/bin/hw0_check.sh

# --- Unprivileged grading user -----------------------------------------------
# Student code runs as an unprivileged user, not root. Submissions are
# expected to be bind-mounted at /work.
RUN useradd --create-home --shell /bin/bash grader
RUN mkdir -p /work && chown grader:grader /work
USER grader
WORKDIR /work

CMD ["bash"]
