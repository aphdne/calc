test() {
  if [ "$(./calc $1)" != "$2" ]; then
    echo -e "\e[1;31m[error] test for '$2' failed"
  else
    echo -e "\e[1;32mtest for '$1 = $2' passed"
  fi
}

test "5 + 5" "10"
