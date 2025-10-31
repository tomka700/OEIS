// g++ -std=c++23 -o3 -march=native -flto A324603.cpp -o A324603
#include <bitset>
#include <iostream>
#include <fstream>
#include <utility>
#include <format>
#include <filesystem>

constexpr int n = 9;
constexpr int k = 8;
// outfile sizes range for a few KB to hundreds of GB, user discretion is advised
constexpr int MAX_LEN = n * k;

unsigned long long count = 0;
std::bitset<MAX_LEN> torus;
std::pair<int,int> loop[MAX_LEN];
std::ofstream fout;

constexpr int idx(int x, int y) noexcept {
    return y * n + x;
}

void write_path(int len) {
    fout << ++count << " (0,0)";
    for (int i = 0; i < len; ++i)
        fout << '(' << loop[i].first << ',' << loop[i].second << ')';
    fout << "(0,0)\n";
}

void dfs(int x, int y, int len) {
    int nx = x + 1;
    if (nx == n) nx = 0;
    int ny = y;
    int i = idx(nx, ny);

    if (nx == 0 && ny == 0) write_path(len);
    else if (!torus[i]) {
        torus.set(i);
        loop[len] = {nx, ny};
        dfs(nx, ny, len + 1);
        torus.reset(i);
    }

    nx = x;
    ny = y + 1;
    if (ny == k) ny = 0;
    i = idx(nx, ny);

    if (nx == 0 && ny == 0) write_path(len);
    else if (!torus[i]) {
        torus.set(i);
        loop[len] = {nx, ny};
        dfs(nx, ny, len + 1);
        torus.reset(i);
    }
}

int main() {
    std::filesystem::path dir = "out";
    if (!std::filesystem::exists(dir)) std::filesystem::create_directory(dir);
    std::filesystem::path file = dir / std::format("{}x{}.txt", n, k);
    fout.open(file);
    if (!fout.is_open()) return 1;

    torus.set(0);
    dfs(0, 0, 0);
    std::cout << count;

    fout.close();
    return 0;

}
