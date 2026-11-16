clang++ -std=c++20 -stdlib=libc++ \
  -isystem "$LLVM/include/c++/v1" \
  -L"$LLVM/lib/c++" -Wl,-rpath,"$LLVM/lib/c++" \
  -fmodule-file=std=std.pcm \
  std.pcm main.cpp