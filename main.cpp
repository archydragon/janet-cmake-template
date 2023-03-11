#include <iostream>
#include <janet.h>

static Janet cfun_myinc(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 2);

    int base = janet_getinteger(argv, 0);
    int n = janet_getinteger(argv, 1);

    return janet_wrap_integer(base + n);
}

int main() {
    std::cout << "[c++] init\n";
    int a = 0;
    std::cout << "[c++] a = " << a <<"\n";

    janet_init();
    JanetTable *env = janet_core_env(nullptr);
    static const JanetReg cfuns[] = {{"myinc", cfun_myinc, ""}};
    janet_cfuns(env, "my", cfuns);

    janet_dostring(env, "(print `[janet] init`)", "main", nullptr);

    janet_def(env, "a", janet_wrap_integer(a), "");
    Janet ret;
    janet_dostring(env, "(def b (myinc a 5)) (print `[janet] a = ` a ` b = ` b) b", "main", &ret);
    if (janet_checktype(ret, JANET_NUMBER)) {
        int b = janet_unwrap_integer(ret);
        std::cout << "[c++] returned b = " << b << "\n";
    }

    janet_deinit();

    return 0;
}
