title() {
  echo -e "\e[1;34m== $1 =="
}

test() {
  COL=""
  if [ "$(./calc $1)" != "$2" ]; then
    COL="\e[1;31m"
  else
    COL="\e[1;32m"
  fi

  echo -e "$COL$1 = $2"
}

title "basic"
test "5 + 5" "10"
test "5 - 5" "0"
test "5 \* 5" "25"
test "5 / 5" "1"

title "signed num"
test "5 + -5"  "0"
test "5 \* -5"  "-25"
test "5 / -5"  "-1"
test "-5 - +5" "-10"

title "parentheses"
test "(5 + -5)+5"  "5"

title "misc"
test "5315-63169+56316-5315" "-6853"
