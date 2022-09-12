

#include <cstdio>
#include <cstdlib>
#include <ctime>

namespace bw {
static unsigned x_ = 0;
class R {
   public:
    unsigned operator()() {
        srand(x_ + (unsigned)time(0));
        return (x_ += rand()) % 100;
    }
};
}  // namespace bw

template <typename T>
void print_max(T a, T b) {
    printf("Max of %d and %d is %d\n", a, b, (a > b ? a : b));
}

int main() {
    bw::R bill;
    bw::R bob;

    // passing functor results
    print_max(bill(), bob());

    return 0;
}