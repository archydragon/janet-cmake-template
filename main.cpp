#include <iostream>
#include <janet.h>

int main() {
    std::cout << "Hello from C++!\n";

    // Initialize the virtual machine. Do this before any calls to Janet functions.
    janet_init();

    // Get the core janet environment. This contains all of the C functions in the core
    // as well as the code in src/boot/boot.janet.
    JanetTable *env = janet_core_env(nullptr);

    // One of several ways to begin the Janet vm.
    janet_dostring(env, "(print `Hello from Janet!`)", "main", nullptr);

    // Use this to free all resources allocated by Janet.
    janet_deinit();

    return 0;
}
