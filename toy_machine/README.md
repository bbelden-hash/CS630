# CS 630/730 — HW0 grading image

## What's inside the image

| Tool | Purpose |
|---|---|
| `gcc` / `g++` / `make` (build-essential) | build C/C++ submissions |
| `python3` | run Python submissions |
| `javac` / `java` (default-jdk) | build & run Java submissions |
| `ocaml`, `ocaml-dune`, `ocamlfind` | build OCaml submissions (and the OCaml oracle) |


## Build

```bash
docker build -t cs630-build:2026fa .
```

(`cs630-build:2026fa` follows the per-semester tagging convention used
elsewhere in the course's build environment.)

## Self-check a submission

Mount the submission directory at `/work` and run `hw0_check.sh`
against your build/run command. The checker appends the path to each
test's assembly file as the final argument, so give it however you'd
normally invoke your program.

C/C++ (assumes the submission's Makefile produces `./hw0_toy_machine`):

```bash
docker run --rm -v "$PWD/submission:/work:ro" -w /work cs630-build:2026fa \
  bash -c "make && hw0_check.sh ./hw0_toy_machine"
```

Python:

```bash
docker run --rm -v "$PWD/submission:/work:ro" -w /work cs630-build:2026fa \
  hw0_check.sh python3 toy_machine.py
```

Java (assumes `Main.class` after compiling):

```bash
docker run --rm -v "$PWD/submission:/work:ro" -w /work cs630-build:2026fa \
  bash -c "javac Main.java && hw0_check.sh java Main"
```

OCaml (via dune):

```bash
docker run --rm -v "$PWD/submission:/work:ro" -w /work cs630-build:2026fa \
  bash -c "dune build && hw0_check.sh -- dune exec ./toy.exe --"
```

## Notes 

- This image intentionally covers only C/C++, Python, Java, and OCaml for
  HW0. 
