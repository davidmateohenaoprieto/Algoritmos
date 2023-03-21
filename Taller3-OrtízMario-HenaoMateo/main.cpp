#include <chrono>
#include <fstream>
#include <iostream>
#include <limits>
#include <unordered_map>
#include <vector>

double BuildOptTree_Aux(const std::vector<double> &P,
                        const std::vector<double> &Q, unsigned long long i,
                        unsigned long long j,
                        std::unordered_map<std::string, double> &memo) {
  std::string key = std::to_string(i) + "," + std::to_string(j);
  if (memo.find(key) != memo.end()) {
    return memo[key];
  }

  if (j == i - 1)
    return (Q[i - 1]);
  else {
    double q = std::numeric_limits<double>::max();
    for (unsigned long long r = i; r <= j; r++) {
      double el = BuildOptTree_Aux(P, Q, i, r - 1, memo);
      double er = BuildOptTree_Aux(P, Q, r + 1, j, memo);
      double w = Q[i - 1];
      for (unsigned long long l = i; l <= j; l++)
        w += P[l - 1] + Q[l];
      double e = el + er + w;
      if (e < q)
        q = e;
    }
    memo[key] = q;
    return (q);
  }
}

double BuildOptTree(const std::vector<double> &P, std::vector<double> &Q) {
  std::unordered_map<std::string, double> memo;
  return (BuildOptTree_Aux(P, Q, 1, P.size(), memo));
}

template <typename _T> void process(const std::string &fname) {
  std::ifstream fstr(fname.c_str(), std::ios::binary | std::ios::ate);
  unsigned long long size = fstr.tellg();
  fstr.seekg(0, std::ios::beg);

  std::vector<_T> data(size / sizeof(_T));
  char *buffer = reinterpret_cast<char *>(data.data());
  fstr.read(buffer, data.size() * sizeof(_T));
  fstr.close();

  // Build Histogram
  std::vector<double> P(1 << (sizeof(_T) << 3), 0);
  for (unsigned long long i = 0; i < data.size(); ++i)
    P[data[i]] += double(1) / double(data.size());
  std::vector<double> Q(P.size() + 1, 0);

  std::cout << BuildOptTree(P, Q) << std::endl;

  // Tamaño del mensaje
  std::cout << "Tamaño del mensaje: " << data.size() << std::endl;

  double max_freq = 0;
unsigned long long max_freq_index = 0;
for (unsigned long long i = 0; i < P.size(); ++i) {
  if (P[i] > max_freq) {
    max_freq = P[i];
    max_freq_index = i;
  }
}

std::unordered_map<unsigned long long, unsigned long long> depths;
for (unsigned long long i = 0; i < data.size(); ++i) {
  unsigned long long val = data[i];
  unsigned long long depth = 0;
  while (val != max_freq_index) {
    val = (val < max_freq_index) ? val + 1 : val - 1;
    depth++;
  }
  depths[depth]++;
}

unsigned long long max_depth = 0;
unsigned long long max_depth_count = 0;
for (const auto &p : depths) {
  if (p.second > max_depth_count) {
    max_depth = p.first;
    max_depth_count = p.second;
  }
}

double min_freq = std::numeric_limits<double>::max();
unsigned long long min_freq_index = 0;
for (unsigned long long i = 0; i < P.size(); ++i) {
  if (P[i] < min_freq) {
    min_freq = P[i];
    min_freq_index = i;
  }
}

depths.clear();
for (unsigned long long i = 0; i < data.size(); ++i) {
  unsigned long long val = data[i];
  unsigned long long depth = 0;
  while (val != min_freq_index) {
    val = (val < min_freq_index) ? val + 1 : val - 1;
    depth++;
  }
  depths[depth]++;
}

unsigned long long min_depth = 0;
unsigned long long min_depth_count = 0;
for (const auto &p : depths) {
  if (p.second > min_depth_count) {
    min_depth = p.first;
    min_depth_count = p.second;
  }
}

auto start_time = std::chrono::steady_clock::now();
for (int i = 0; i < 100; ++i) {
  unsigned long long val = max_freq_index;
  while (val != 0) {
    val = (val < max_freq_index) ? val + 1 : val - 1;
  }
}
auto end_time = std::chrono::steady_clock::now();
auto time_diff = end_time - start_time;
double max_freq_search_time = std::chrono::duration_cast<std::chrono::nanoseconds>(time_diff).count() / 100.0;

start_time = std::chrono::steady_clock::now();
for (int i = 0; i < 100; ++i) {
  unsigned long long val = min_freq_index;
  while (val != 0) {
    val = (val < min_freq_index) ? val + 1 : val - 1;
  }
}

end_time = std::chrono::steady_clock::now();
time_diff = end_time - start_time;
double min_freq_search_time = std::chrono::duration_cast<std::chrono::nanoseconds>(time_diff).count() / 100.0;

std::cout<<"frecuencia maxima: "<<max_freq<<std::endl;
std::cout<<"frecuencia minima: "<<min_freq<<std::endl;
std::cout<<"profundidad maxima: "<<max_depth<<std::endl;
std::cout<<"profundidad minima: "<<min_depth<<std::endl;
std::cout<<"tiempo de busqueda maxima: "<<max_freq_search_time<<std::endl;
std::cout<<"tiempo de busqueda minima: "<<min_freq_search_time<<std::endl;
}

int main(int argc, char *argv[]) {
  if (argc < 3) {
    std::cout << "Usage: " << argv[0] << " input byte/word" << std::endl;
    return (-1);
  }
  std::string fname = argv[1];
  std::string type = argv[2];
  if (type == "byte")
    process<unsigned char>(fname);
  else if (type == "word")
    process<unsigned short>(fname);

  return 0;
}
