#include <math.h>

#include <array>
#include <iostream>
#include <string>
#include <vector>

static std::array<uint32_t, 0x10000> peq;

uint32_t myers_32(std::string a, std::string b) {
    size_t n = a.length();
    size_t m = b.length();
    int lst = 1 << (n - 1);
    int pv = -1;
    int mv = 0;
    auto sc = n;
    int i = n;
    while (i--)
        peq[a.at(i)] |= 1 << i;
    for (size_t i = 0; i < m; i++) {
        auto eq = peq[b.at(i)];
        auto xv = eq | mv;
        eq |= ((eq & pv) + pv) ^ pv;
        mv |= ~(eq | pv);
        pv &= eq;
        if (mv & lst) sc++;
        if (pv & lst) sc--;
        mv = (mv << 1) | 1;
        pv = (pv << 1) | ~(xv | mv);
        mv &= xv;
    }
    i = n;
    while (i--)
        peq[a.at(i)] = 0;
    return sc;
}

uint32_t myers_x(std::string a, std::string b) {
    size_t THIRTYTWO = 32;
    size_t n = a.length();
    size_t m = b.length();
    auto hsize = uint32_t(ceil(n / THIRTYTWO));
    auto vsize = uint32_t(ceil(m / THIRTYTWO));
    std::vector<int> mhc(hsize, 0);
    std::vector<int> phc(hsize, -1);
    for (int j = 0; j < vsize - 1; j++) {
        auto mv = 0;
        auto pv = -1;
        const auto start = j * THIRTYTWO;
        const auto vlen = std::min(THIRTYTWO, m) + start;
        for (int k = start; k < vlen; k++)
            peq[b.at(k)] |= 1 << k;
        for (int i = 0; i < n; i++) {
            const auto eq = peq[a.at(i)];
            const auto pb = (uint32_t(phc[(i / THIRTYTWO) | 0]) >> i) & 1;
            const auto mb = (uint32_t(mhc[(i / THIRTYTWO) | 0]) >> i) & 1;
            const auto xv = eq | mv;
            const auto xh = ((((eq | mb) & pv) + pv) ^ pv) | eq | mb;
            auto ph = mv | ~(xh | pv);
            auto mh = pv & xh;
            if ((uint32_t(ph) >> 31) ^ pb)
                phc[(i / THIRTYTWO) | 0] ^= 1 << i;
            if ((uint32_t(mh) >> 31) ^ mb)
                mhc[(i / THIRTYTWO) | 0] ^= 1 << i;
            ph = (ph << 1) | pb;
            mh = (mh << 1) | mb;
            pv = mh | ~(xv | ph);
            mv = ph & xv;
        }
        for (int k = start; k < vlen; k++)
            peq[b.at(k)] = 0;
    }
    auto mv = 0;
    auto pv = -1;
    const auto start = (vsize - 2) * THIRTYTWO;
    const auto vlen = std::min(THIRTYTWO, m - start) + start;
    for (int k = start; k < vlen; k++)
        peq[b.at(k)] |= 1 << k;
    auto score = m;
    for (int i = 0; i < n; i++) {
        const auto eq = peq[a.at(i)];
        const auto pb = (uint32_t(phc[(i / THIRTYTWO) | 0]) >> i) & 1;
        const auto mb = (uint32_t(mhc[(i / THIRTYTWO) | 0]) >> i) & 1;
        const auto xv = eq | mv;
        const auto xh = ((((eq | mb) & pv) + pv) ^ pv) | eq | mb;
        auto ph = mv | ~(xh | pv);
        auto mh = pv & xh;
        score += (ph >> m - 1) & 1;
        score -= (mh >> m - 1) & 1;
        if ((ph >> 31) ^ pb)
            phc[(i / THIRTYTWO) | 0] ^= 1 << i;
        if ((mh >> 31) ^ mb)
            mhc[(i / THIRTYTWO) | 0] ^= 1 << i;
        ph = (ph << 1) | pb;
        mh = (mh << 1) | mb;
        pv = mh | ~(xv | ph);
        mv = ph & xv;
    }
    for (int k = start; k < vlen; k++) {
        peq[b.at(k)] = 0;
    }
    return score;
}

uint32_t distance(std::string a, std::string b) {
    if (a.length() < b.length()) {
        a.swap(b);
    }
    if (b.empty()) {
        return a.length();
    }
    if (a.length() <= 32) {
        return myers_32(a, b);
    }
    return myers_x(a, b);
}

int main() {
    while (true) {
        std::string str1;
        std::cout << "Enter first string: ";
        std::getline(std::cin, str1);
        size_t str1_len = str1.length();

        std::string str2;
        std::cout << "Enter second string: ";
        std::getline(std::cin, str2);
        size_t str2_len = str2.length();

        auto dist = distance(str1, str2);

        std::cout << "Levenshtein distance is " << dist << std::endl;
        std::cout << "Would you like to continue? Press 'q' to exit: ";

        std::string response;
        std::cin >> response;
        if (response == "q") {
            break;
        }
    }

    return EXIT_SUCCESS;
}