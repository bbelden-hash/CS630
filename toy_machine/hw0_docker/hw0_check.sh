#!/usr/bin/env bash

# hw0_check.sh -- Self-check for HW0 (The Toy Machine)
#
# This is a self-check tool: it runs your program against a
# sample of 10 of the assignment's test cases so you can sanity check your
# I/O contract and basic instruction semantics -- including LOAD_I/ADD
# wraparound behavior -- before submitting.
#
# It is NOT the full grading suite. Passing everything here is a good sign,
# but the graded set includes additional cases (chained accumulation over
# many steps, more wraparound combinations, and fresh inputs not shown here)
# that are not included in this script.

set -u
PROG="$(basename "$0")"

usage() {
  cat <<USAGE
Usage: ./$PROG [--list] [--] <run-command> [args...]

<run-command> is how to launch your toy machine; this checker appends the
path to a test file as the final argument. Examples:
  ./$PROG ./hw0_toy_machine
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
read -r -d '' MANIFEST <<'EOF' || true
input-1	Halt directly, check the init state	IyBIYWx0IGRpcmVjdGx5LCBjaGVjayB0aGUgaW5pdCBzdGF0ZQpIQUxUCg==	UjA9MApSMT0wClIyPTAKUjM9MApSND0wClI1PTAKUjY9MApSNz0w
input-2	LOAD_I: place an immediate into a register	IyBMT0FEX0k6IHBsYWNlIGFuIGltbWVkaWF0ZSBpbnRvIGEgcmVnaXN0ZXIKTE9BRF9JIFIwLCA0MgpMT0FEX0kgUjMsIDcKSEFMVAo=	UjA9NDIKUjE9MApSMj0wClIzPTcKUjQ9MApSNT0wClI2PTAKUjc9MA==
input-3	ADD: Rd <- Rs1 + Rs2	IyBBREQ6IFJkIDwtIFJzMSArIFJzMgpMT0FEX0kgUjEsIDIwCkxPQURfSSBSMiwgMjIKQUREIFIzLCBSMSwgUjIKSEFMVAo=	UjA9MApSMT0yMApSMj0yMgpSMz00MgpSND0wClI1PTAKUjY9MApSNz0w
input-4	compute 30 + 10, then 2 + result	IyBjb21wdXRlIDMwICsgMTAsIHRoZW4gMiArIHJlc3VsdApMT0FEX0kgUjAsIDMwCkxPQURfSSBSMSwgMTAKQUREIFIyLCBSMCwgUjEKTE9BRF9JIFIzLCAyCkFERCBSNCwgUjMsIFIyCkhBTFQK	UjA9MzAKUjE9MTAKUjI9NDAKUjM9MgpSND00MgpSNT0wClI2PTAKUjc9MA==
input-5	every register written, printed R0..R7 in order	IyBldmVyeSByZWdpc3RlciB3cml0dGVuLCBwcmludGVkIFIwLi5SNyBpbiBvcmRlcgpMT0FEX0kgUjAsIDAKTE9BRF9JIFIxLCAxMQpMT0FEX0kgUjIsIDIyCkxPQURfSSBSMywgMzMKTE9BRF9JIFI0LCA0NApMT0FEX0kgUjUsIDU1CkxPQURfSSBSNiwgNjYKTE9BRF9JIFI3LCA3NwpIQUxUCg==	UjA9MApSMT0xMQpSMj0yMgpSMz0zMwpSND00NApSNT01NQpSNj02NgpSNz03Nw==
input-6	comments and blank lines are ignored	IyBjb21tZW50cyBhbmQgYmxhbmsgbGluZXMgYXJlIGlnbm9yZWQKCkxPQURfSSBSMCwgMQoKIyBhIGNvbW1lbnQgaW4gdGhlIG1pZGRsZSBvZiB0aGUgcHJvZ3JhbQpMT0FEX0kgUjEsIDIKQUREIFIyLCBSMCwgUjEKCiMgb25lIG1vcmUgY29tbWVudCByaWdodCBiZWZvcmUgSEFMVApIQUxUCg==	UjA9MQpSMT0yClIyPTMKUjM9MApSND0wClI1PTAKUjY9MApSNz0w
input-7	dead code: only the first HALT is reached	IyBkZWFkIGNvZGU6IG9ubHkgdGhlIGZpcnN0IEhBTFQgaXMgcmVhY2hlZApMT0FEX0kgUjAsIDcKTE9BRF9JIFIxLCAzCkhBTFQKQUREIFIyLCBSMCwgUjEKTE9BRF9JIFI3LCA5OTkK	UjA9NwpSMT0zClIyPTAKUjM9MApSND0wClI1PTAKUjY9MApSNz0w
input-9	R0 is a general-purpose register here (no hardcoded zero)	IyBSMCBpcyBhIGdlbmVyYWwtcHVycG9zZSByZWdpc3RlciBoZXJlIChubyBoYXJkY29kZWQgemVybykKTE9BRF9JIFIwLCAxMDAKQUREIFIxLCBSMCwgUjAKSEFMVAo=	UjA9MTAwClIxPTIwMApSMj0wClIzPTAKUjQ9MApSNT0wClI2PTAKUjc9MA==
input-10	operands are read before the write: Rd may also be a source	IyBvcGVyYW5kcyBhcmUgcmVhZCBiZWZvcmUgdGhlIHdyaXRlOiBSZCBtYXkgYWxzbyBiZSBhIHNvdXJjZQpMT0FEX0kgUjAsIDQKTE9BRF9JIFIxLCAxMApBREQgUjAsIFIwLCBSMQpBREQgUjAsIFIwLCBSMApIQUxUCg==	UjA9MjgKUjE9MTAKUjI9MApSMz0wClI0PTAKUjU9MApSNj0wClI3PTA=
input-14	INT32_MAX + 1 wraps to INT32_MIN (two's complement)	IyBJTlQzMl9NQVggKyAxIHdyYXBzIHRvIElOVDMyX01JTiAodHdvJ3MgY29tcGxlbWVudCkKTE9BRF9JIFIwLCAyMTQ3NDgzNjQ3CkxPQURfSSBSMSwgMQpBREQgUjIsIFIwLCBSMQpIQUxUCg==	UjA9MjE0NzQ4MzY0NwpSMT0xClIyPS0yMTQ3NDgzNjQ4ClIzPTAKUjQ9MApSNT0wClI2PTAKUjc9MA==
EOF

TAB="$(printf '\t')"

if [ "$LIST" -eq 1 ]; then
  while IFS="$TAB" read -r name desc _ _; do
    [ -n "$name" ] && printf '  %-12s %s\n' "$name" "$desc"
  done <<< "$MANIFEST"
  exit 0
fi

tmp="$(mktemp -d "${TMPDIR:-/tmp}/hw0check.XXXXXX")" || { echo "mktemp failed" >&2; exit 3; }
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
