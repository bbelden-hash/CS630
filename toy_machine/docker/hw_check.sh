#!/usr/bin/env bash
# hw1_check.sh -- PUBLIC self-check for HW1 (A Toy Machine with Memory)
#
# This is a self-check tool: it runs your program against a
# sample of 10 of the assignment's test cases so you can sanity check your
# I/O contract and the new instructions -- SUB, MOVE, LOAD/STORE and
# LOAD_B/STORE_B, including little-endian byte order -- before submitting.
#
# It is NOT the full grading suite. Passing everything here is a good sign,
# but the graded set includes additional cases (more byte-order and
# address-arithmetic combinations, base/destination register aliasing,
# memory boundaries, and fresh multi-instruction programs) that are not
# included in this script.
set -u
PROG="$(basename "$0")"

usage() {
  cat <<USAGE
Usage: ./$PROG [--list] [--] <run-command> [args...]

<run-command> is how to launch your toy machine; this checker appends the
path to a test file as the final argument. Examples:
  ./$PROG ./hw1_toy_machine
  ./$PROG python3 toy_machine.py
  ./$PROG -- dune exec ./toy.exe --

  --list        list the case names/descriptions and exit
  -h, --help    this help
USAGE
}

LIST=0
while [ $# -gt 0 ]; do
  case "$1" in
    --list)      LIST=1; shift ;;
    -h|--help)   usage; exit 0 ;;
    --)          shift; break ;;
    --*)         echo "unknown option: $1" >&2; usage; exit 2 ;;
    *)           break ;;
  esac
done
if [ "$LIST" -eq 0 ] && [ $# -eq 0 ]; then usage; exit 2; fi

_b64d() { openssl base64 -d -A 2>/dev/null || base64 -d 2>/dev/null || base64 -D; }

TMO=""
command -v timeout  >/dev/null 2>&1 && TMO=timeout
[ -z "$TMO" ] && command -v gtimeout >/dev/null 2>&1 && TMO=gtimeout
run_one() { if [ -n "$TMO" ]; then "$TMO" 10 "$@"; else "$@"; fi; }

# Bail out with a clear diagnosis if the run-command itself couldn't be
# launched at all (as opposed to running and printing wrong output). Exit
# 127 is the shell's own convention for "command not found"; 126 means it
# was found but isn't executable. Without this, a typo'd or unresolved
# run-command just shows up as a mysterious FAIL with empty output on every
# single case.
check_launch_error() {
  status="$1"; cmd_name="$2"; stderr_file="$3"
  if [ "$status" -eq 127 ] || [ "$status" -eq 126 ]; then
    echo >&2
    echo "error: could not run '$cmd_name' (exit $status)" >&2
    [ -s "$stderr_file" ] && cat "$stderr_file" >&2
    if [ "$status" -eq 127 ]; then
      case "$cmd_name" in
        */*) echo "hint: '$cmd_name' was not found. Check the path is correct." >&2 ;;
        *)   echo "hint: '$cmd_name' was not found on PATH. If you meant a file in the current directory, run it as ./$cmd_name instead." >&2 ;;
      esac
    else
      echo "hint: '$cmd_name' was found but is not executable. Try: chmod +x $cmd_name" >&2
    fi
    exit 3
  fi
}

normalize() {
  sed 's/[[:space:]]*$//' \
    | awk '{a[NR]=$0} END{n=NR; while(n>0 && a[n]=="") n--; for(i=1;i<=n;i++) print a[i]}'
}

# Sample cases: name<TAB>description<TAB>input-asm(base64)<TAB>expected-output(base64)
# The graded suite has additional cases not included here.
read -r -d '' MANIFEST <<'EOF' || true
input-1	SUB: Rd <- Rs1 - Rs2, positive and negative results	IyBTVUI6IFJkIDwtIFJzMSAtIFJzMiwgcG9zaXRpdmUgYW5kIG5lZ2F0aXZlIHJlc3VsdHMKTE9BRF9JIFIwLCA1MApMT0FEX0kgUjEsIDgKU1VCIFIyLCBSMCwgUjEKU1VCIFIzLCBSMSwgUjAKSEFMVAo=	UjA9NTAKUjE9OApSMj00MgpSMz0tNDIKUjQ9MApSNT0wClI2PTAKUjc9MA==
input-2	MOVE: copy a register, source is left unchanged	IyBNT1ZFOiBjb3B5IGEgcmVnaXN0ZXIsIHNvdXJjZSBpcyBsZWZ0IHVuY2hhbmdlZApMT0FEX0kgUjEsIC0xNwpMT0FEX0kgUjUsIDY0Ck1PVkUgUjQsIFIxCk1PVkUgUjAsIFI1CkhBTFQK	UjA9NjQKUjE9LTE3ClIyPTAKUjM9MApSND0tMTcKUjU9NjQKUjY9MApSNz0w
input-4	handout example 1: store two words, load them back, SUB, MOVE	IyBoYW5kb3V0IGV4YW1wbGUgMTogc3RvcmUgdHdvIHdvcmRzLCBsb2FkIHRoZW0gYmFjaywgU1VCLCBNT1ZFCkxPQURfSSBSMCwgMApMT0FEX0kgUjEsIDQyClNUT1JFIFIxLCAwKFIwKQpMT0FEX0kgUjIsIDEwMApTVE9SRSBSMiwgNChSMCkKTE9BRCBSMywgMChSMCkKTE9BRCBSNCwgNChSMCkKU1VCIFI1LCBSNCwgUjMKTU9WRSBSNiwgUjUKSEFMVAo=	UjA9MApSMT00MgpSMj0xMDAKUjM9NDIKUjQ9MTAwClI1PTU4ClI2PTU4ClI3PTA=
input-5	handout example 2: four STORE_B bytes read back as one little-endian word	IyBoYW5kb3V0IGV4YW1wbGUgMjogZm91ciBTVE9SRV9CIGJ5dGVzIHJlYWQgYmFjayBhcyBvbmUgbGl0dGxlLWVuZGlhbiB3b3JkCkxPQURfSSBSMCwgOApMT0FEX0kgUjEsIDIzOQpMT0FEX0kgUjIsIDI1NQpMT0FEX0kgUjMsIDUyCkxPQURfSSBSNCwgMQpTVE9SRV9CIFIxLCAwKFIwKQpTVE9SRV9CIFIyLCAxKFIwKQpTVE9SRV9CIFIzLCAyKFIwKQpTVE9SRV9CIFI0LCAzKFIwKQpMT0FEIFI3LCAwKFIwKQpIQUxUCg==	UjA9OApSMT0yMzkKUjI9MjU1ClIzPTUyClI0PTEKUjU9MApSNj0wClI3PTIwMjUwNjA3
input-6	memory starts zeroed: loads from untouched addresses give 0	IyBtZW1vcnkgc3RhcnRzIHplcm9lZDogbG9hZHMgZnJvbSB1bnRvdWNoZWQgYWRkcmVzc2VzIGdpdmUgMApMT0FEX0kgUjEsIDcKTE9BRF9JIFIyLCA3CkxPQURfSSBSMywgNwpMT0FEX0kgUjQsIDcKTE9BRCBSMSwgMChSMCkKTE9BRCBSMiwgMTAyMChSMCkKTE9BRF9CIFIzLCA1MTMoUjApCkxPQURfQiBSNCwgMTAyMyhSMCkKSEFMVAo=	UjA9MApSMT0wClIyPTAKUjM9MApSND0wClI1PTAKUjY9MApSNz0w
input-7	STORE a word, then LOAD_B each byte: little-endian byte order	IyBTVE9SRSBhIHdvcmQsIHRoZW4gTE9BRF9CIGVhY2ggYnl0ZTogbGl0dGxlLWVuZGlhbiBieXRlIG9yZGVyCkxPQURfSSBSMCwgMzA1NDE5ODk2ClNUT1JFIFIwLCAxNihSNykKTE9BRF9CIFIxLCAxNihSNykKTE9BRF9CIFIyLCAxNyhSNykKTE9BRF9CIFIzLCAxOChSNykKTE9BRF9CIFI0LCAxOShSNykKSEFMVAo=	UjA9MzA1NDE5ODk2ClIxPTEyMApSMj04NgpSMz01MgpSND0xOApSNT0wClI2PTAKUjc9MA==
input-9	LOAD_B zero-extends: a stored 0xFF byte loads as 255, not -1	IyBMT0FEX0IgemVyby1leHRlbmRzOiBhIHN0b3JlZCAweEZGIGJ5dGUgbG9hZHMgYXMgMjU1LCBub3QgLTEKTE9BRF9JIFIxLCAtMQpTVE9SRV9CIFIxLCA1KFIwKQpMT0FEX0IgUjIsIDUoUjApCkxPQURfSSBSMywgLTEyOApTVE9SRV9CIFIzLCA2KFIwKQpMT0FEX0IgUjQsIDYoUjApCkhBTFQK	UjA9MApSMT0tMQpSMj0yNTUKUjM9LTEyOApSND0xMjgKUjU9MApSNj0wClI3PTA=
input-10	STORE_B writes only the low 8 bits of the register	IyBTVE9SRV9CIHdyaXRlcyBvbmx5IHRoZSBsb3cgOCBiaXRzIG9mIHRoZSByZWdpc3RlcgpMT0FEX0kgUjEsIDUxMQpMT0FEX0kgUjIsIDI1NgpMT0FEX0kgUjMsIDMwMApMT0FEX0kgUjQsIC0yMDAKU1RPUkVfQiBSMSwgNDAoUjApClNUT1JFX0IgUjIsIDQxKFIwKQpTVE9SRV9CIFIzLCA0MihSMCkKU1RPUkVfQiBSNCwgNDMoUjApCkxPQUQgUjUsIDQwKFIwKQpIQUxUCg==	UjA9MApSMT01MTEKUjI9MjU2ClIzPTMwMApSND0tMjAwClI1PTk0MjQwNzkzNQpSNj0wClI3PTA=
input-11	overwrite a word: a later LOAD sees the newer STORE	IyBvdmVyd3JpdGUgYSB3b3JkOiBhIGxhdGVyIExPQUQgc2VlcyB0aGUgbmV3ZXIgU1RPUkUKTE9BRF9JIFIxLCAxMTEKU1RPUkUgUjEsIDMyKFIwKQpMT0FEIFIyLCAzMihSMCkKTE9BRF9JIFIxLCAyMjIKU1RPUkUgUjEsIDMyKFIwKQpMT0FEIFIzLCAzMihSMCkKSEFMVAo=	UjA9MApSMT0yMjIKUjI9MTExClIzPTIyMgpSND0wClI1PTAKUjY9MApSNz0w
input-13	SUB wraps around (two's complement): MIN - 1 and 0 - MIN	IyBTVUIgd3JhcHMgYXJvdW5kICh0d28ncyBjb21wbGVtZW50KTogTUlOIC0gMSBhbmQgMCAtIE1JTgpMT0FEX0kgUjAsIC0yMTQ3NDgzNjQ4CkxPQURfSSBSMSwgMQpTVUIgUjIsIFIwLCBSMQpMT0FEX0kgUjMsIDAKU1VCIFI0LCBSMywgUjAKTE9BRF9JIFI1LCAyMTQ3NDgzNjQ3CkxPQURfSSBSNiwgLTEKU1VCIFI3LCBSNSwgUjYKSEFMVAo=	UjA9LTIxNDc0ODM2NDgKUjE9MQpSMj0yMTQ3NDgzNjQ3ClIzPTAKUjQ9LTIxNDc0ODM2NDgKUjU9MjE0NzQ4MzY0NwpSNj0tMQpSNz0tMjE0NzQ4MzY0OA==
EOF

TAB="$(printf '\t')"

if [ "$LIST" -eq 1 ]; then
  while IFS="$TAB" read -r name desc _ _; do
    [ -n "$name" ] && printf '  %-12s %s\n' "$name" "$desc"
  done <<< "$MANIFEST"
  exit 0
fi

tmp="$(mktemp -d "${TMPDIR:-/tmp}/hw1check.XXXXXX")" || { echo "mktemp failed" >&2; exit 3; }
trap 'rm -rf "$tmp"' EXIT INT TERM

total=0; pass=0
while IFS="$TAB" read -r name desc ib64 wb64; do
  [ -n "$name" ] || continue
  total=$((total + 1))
  printf '%s' "$ib64" | _b64d > "$tmp/case.asm"
  want="$(printf '%s' "$wb64" | _b64d | normalize)"
  raw="$(run_one "$@" "$tmp/case.asm" 2>"$tmp/stderr")"; status=$?
  check_launch_error "$status" "$1" "$tmp/stderr"
  got="$(printf '%s' "$raw" | normalize)"
  if [ "$got" = "$want" ]; then
    pass=$((pass + 1))
    printf 'PASS  %-12s %s\n' "$name" "$desc"
  else
    printf 'FAIL  %-12s %s\n' "$name" "$desc"
    printf '%s\n' "$want" | sed 's/^/         expected | /'
    printf '%s\n' "$got"  | sed 's/^/         yours    | /'
  fi
done <<< "$MANIFEST"

echo
printf '%d/%d public sample cases passed\n' "$pass" "$total"
echo "(this is a sample self-check, not the full graded suite)"
[ "$pass" -eq "$total" ]
