#include <string.h>

#include <algorithm>
#include <cstdio>
#include <locale>

constexpr size_t maxlen_ = 128;

class FTitle {
    char lastc;

   public:
    FTitle() : lastc(0) {}

    char operator()(const char& c);
};

// this is the functor function
char FTitle::operator()(const char& c) {
    const char r = (lastc == ' ' || lastc == 0) ? toupper(c) : tolower(c);
    lastc = c;
    return r;
}

int main() {
    char lastc = 0;
    char s[] = "big light in sky slated to upper in east";

    // lambda can be used instead of Ftitle operator()
    // std::transform(s, s + strnlen(s, maxlen_), s, FTitle());
    std::transform(
        s, s + strnlen(s, maxlen_), s, [&lastc](const char& c) -> char {
            const char r =
                (lastc == ' ' || lastc == 0) ? toupper(c) : tolower(c);
            lastc = c;
            return r;
        });
    puts(s);

    return 0;
}