https://www.tron.org/ja/page-722/rtos01/


```sh
nix-shell -p clang-tools --run "clang-format -i *.c"
nix-shell -p clang-tools --run "clang-tidy *.c"
```