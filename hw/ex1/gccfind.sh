#!/bin/bash
#Omer Shoulstein 214668840

if test $# -le 1; then
  echo "Not enough parameters"
  exit 1
fi

if [ "$3" != "-r" ]; then
  if [[ ! $1 = */ ]]; then
    set "$1"/
  fi
  for FILE in "$1"*.out; do
    if [ -f "$FILE" ]; then
      rm "$FILE"
    fi
  done

  for FILE in "$1"*.c; do
    if [ -f "$FILE" ]; then
      if grep -q -i -E "(^|\W)$2($|\W)" "$FILE"; then
        gcc "$FILE" -w -o "${FILE%.c}.out"
      fi
    fi
  done
else
  find "$1" -name "*.out" | while read -r FILE; do
    rm "$FILE"
  done
  find "$1" -name "*.c" | while read -r FILE; do
    if grep -q -i -E "(^|\W)$2($|\W)" "$FILE"; then
      gcc "$FILE" -w -o "${FILE%.c}.out"
    fi
  done
fi
