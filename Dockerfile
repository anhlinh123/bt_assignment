FROM ubuntu:noble

RUN apt-get update && apt-get install -y gcc g++ clang-format jq cmake git pipx
RUN pipx ensurepath && pipx install conan
ENV PATH=${PATH}:/root/.local/bin
RUN conan profile detect --force
RUN sed -i 's|\(compiler.cppstd=\)\(.\+\)|\1gnu20|g' /root/.conan2/profiles/default
