#include <bits/stdc++.h>
using namespace std;

struct XorShift128Plus {
  uint64_t S0, S1;
  static uint64_t SplitMix64(uint64_t &x) {
    uint64_t z = (x += 0x9e3779b97f4a7c15ULL);
    z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9ULL;
    z = (z ^ (z >> 27)) * 0x94d049bb133111ebULL;
    return z ^ (z >> 31);
  }
  explicit XorShift128Plus(uint64_t seed = 0) {
    if (!seed) seed = (uint64_t)chrono::high_resolution_clock::now().time_since_epoch().count();
    uint64_t x = seed;
    S0 = SplitMix64(x);
    S1 = SplitMix64(x);
    if (!S0 && !S1) S1 = 1;
  }
  inline uint64_t NextU64() {
    uint64_t x = S0, y = S1;
    S0 = y;
    x ^= x << 23;
    x ^= x >> 17;
    x ^= y ^ (y >> 26);
    S1 = x;
    return x + y;
  }
  inline uint32_t NextU32() { return (uint32_t)NextU64(); }
  inline uint64_t NextRangeU64(uint64_t n) {
    if (n <= 1) return 0;
    __uint128_t m = (__uint128_t)NextU64() * (__uint128_t)n;
    return (uint64_t)(m >> 64);
  }
  inline int NextInt(int lo, int hi) {
    if (hi <= lo) return lo;
    return lo + (int)NextRangeU64((uint64_t)(hi - lo + 1));
  }
  inline bool NextBool() { return (NextU64() & 1ULL) != 0; }
};

static inline void AppendInt(string &s, int v) {
  char buf[32];
  auto [p, ec] = to_chars(buf, buf + 32, v);
  s.append(buf, p);
}

static inline string BuildChromeLike(XorShift128Plus &rng, bool isEdge, bool isMobile, bool isAndroid, bool isWindows, bool isMac, bool isLinux, bool isIPhone, bool isIPad) {
  int chromeMajor = rng.NextInt(118, 131);
  int chromeMinor = 0;
  int chromeBuild = rng.NextInt(5200, 6900);
  int chromePatch = rng.NextInt(10, 220);

  int webkitMajor = 537;
  int webkitMinor = 36;

  string ua;
  ua.reserve(180);

  ua += "Mozilla/5.0 (";

  if (isAndroid) {
    int aMajor = rng.NextInt(9, 15);
    ua += "Linux; Android ";
    AppendInt(ua, aMajor);
    ua += "; ";
    static const vector<string> Models = {
      "SM-S918B","SM-S911B","SM-A546B","SM-A325F","Pixel 7","Pixel 8","Pixel 6a",
      "M2012K11AG","M2102J20SG","CPH2401","VOG-L29","IN2013","RMX3700"
    };
    ua += Models[(size_t)rng.NextRangeU64(Models.size())];
  } else if (isIPhone) {
    int iosMajor = rng.NextInt(14, 17);
    int iosMinor = rng.NextInt(0, 6);
    ua += "iPhone; CPU iPhone OS ";
    AppendInt(ua, iosMajor);
    ua += "_";
    AppendInt(ua, iosMinor);
    ua += " like Mac OS X";
  } else if (isIPad) {
    int iosMajor = rng.NextInt(14, 17);
    int iosMinor = rng.NextInt(0, 6);
    ua += "iPad; CPU OS ";
    AppendInt(ua, iosMajor);
    ua += "_";
    AppendInt(ua, iosMinor);
    ua += " like Mac OS X";
  } else if (isWindows) {
    ua += "Windows NT 10.0; Win64; x64";
  } else if (isMac) {
    int macMajor = rng.NextInt(10, 15);
    int macMinor = rng.NextInt(0, 6);
    ua += "Macintosh; Intel Mac OS X ";
    AppendInt(ua, macMajor);
    ua += "_";
    AppendInt(ua, macMinor);
    ua += "_";
    AppendInt(ua, rng.NextInt(0, 9));
  } else if (isLinux) {
    ua += "X11; Linux x86_64";
  } else {
    ua += "X11; Linux x86_64";
  }

  ua += ") AppleWebKit/";
  AppendInt(ua, webkitMajor);
  ua += ".";
  AppendInt(ua, webkitMinor);
  ua += " (KHTML, like Gecko) ";

  if (isEdge) {
    ua += "Chrome/";
    AppendInt(ua, chromeMajor);
    ua += ".";
    AppendInt(ua, chromeMinor);
    ua += ".";
    AppendInt(ua, chromeBuild);
    ua += ".";
    AppendInt(ua, chromePatch);
    ua += " Safari/";
    AppendInt(ua, webkitMajor);
    ua += ".";
    AppendInt(ua, webkitMinor);
    ua += " Edg/";
    int edgMajor = chromeMajor;
    ua += to_string(edgMajor) + ".0.";
    AppendInt(ua, rng.NextInt(1500, 2600));
    ua += ".";
    AppendInt(ua, rng.NextInt(40, 140));
  } else {
    ua += "Chrome/";
    AppendInt(ua, chromeMajor);
    ua += ".";
    AppendInt(ua, chromeMinor);
    ua += ".";
    AppendInt(ua, chromeBuild);
    ua += ".";
    AppendInt(ua, chromePatch);
    ua += " Safari/";
    AppendInt(ua, webkitMajor);
    ua += ".";
    AppendInt(ua, webkitMinor);
  }

  if (isMobile && !isAndroid && !isIPhone && !isIPad) {
    ua += " Mobile";
  }

  return ua;
}

static inline string BuildFirefoxLike(XorShift128Plus &rng, bool isMobile, bool isAndroid, bool isWindows, bool isMac, bool isLinux) {
  int ffMajor = rng.NextInt(109, 123);
  string ua;
  ua.reserve(170);

  if (isAndroid) {
    int aMajor = rng.NextInt(9, 15);
    ua += "Mozilla/5.0 (Android ";
    AppendInt(ua, aMajor);
    ua += "; Mobile; rv:";
    AppendInt(ua, ffMajor);
    ua += ".0) Gecko/";
    ua += "20100101 Firefox/";
    AppendInt(ua, ffMajor);
    ua += ".0";
    return ua;
  }

  ua += "Mozilla/5.0 (";
  if (isWindows) ua += "Windows NT 10.0; Win64; x64";
  else if (isMac) {
    int macMajor = rng.NextInt(10, 15);
    int macMinor = rng.NextInt(0, 6);
    ua += "Macintosh; Intel Mac OS X ";
    AppendInt(ua, macMajor);
    ua += "_";
    AppendInt(ua, macMinor);
    ua += "_";
    AppendInt(ua, rng.NextInt(0, 9));
  } else if (isLinux) ua += "X11; Linux x86_64";
  else ua += "X11; Linux x86_64";
  ua += "; rv:";
  AppendInt(ua, ffMajor);
  ua += ".0) Gecko/20100101 Firefox/";
  AppendInt(ua, ffMajor);
  ua += ".0";
  if (isMobile) ua += " Mobile";
  return ua;
}

static inline string BuildSafariLike(XorShift128Plus &rng, bool isIPhone, bool isIPad, bool isMac) {
  int webkitMajor = 605;
  int webkitMinor = rng.NextInt(1, 60);
  int safariMajor = rng.NextInt(14, 17);
  int safariMinor = rng.NextInt(0, 6);

  string ua;
  ua.reserve(180);

  ua += "Mozilla/5.0 (";
  if (isIPhone) {
    int iosMajor = rng.NextInt(14, 17);
    int iosMinor = rng.NextInt(0, 6);
    ua += "iPhone; CPU iPhone OS ";
    AppendInt(ua, iosMajor);
    ua += "_";
    AppendInt(ua, iosMinor);
    ua += " like Mac OS X";
  } else if (isIPad) {
    int iosMajor = rng.NextInt(14, 17);
    int iosMinor = rng.NextInt(0, 6);
    ua += "iPad; CPU OS ";
    AppendInt(ua, iosMajor);
    ua += "_";
    AppendInt(ua, iosMinor);
    ua += " like Mac OS X";
  } else {
    int macMajor = rng.NextInt(10, 15);
    int macMinor = rng.NextInt(0, 6);
    ua += "Macintosh; Intel Mac OS X ";
    AppendInt(ua, macMajor);
    ua += "_";
    AppendInt(ua, macMinor);
    ua += "_";
    AppendInt(ua, rng.NextInt(0, 9));
  }
  ua += ") AppleWebKit/";
  AppendInt(ua, webkitMajor);
  ua += ".";
  AppendInt(ua, webkitMinor);
  ua += " (KHTML, like Gecko) Version/";
  AppendInt(ua, safariMajor);
  ua += ".";
  AppendInt(ua, safariMinor);
  ua += " Safari/";
  AppendInt(ua, webkitMajor);
  ua += ".";
  AppendInt(ua, webkitMinor);
  return ua;
}

static inline string PickRealUa(XorShift128Plus &rng) {
  static const vector<string> Pool = {
    "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/130.0.0.0 Safari/537.36",
    "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/129.0.0.0 Safari/537.36",
    "Mozilla/5.0 (Macintosh; Intel Mac OS X 14_4_1) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/17.4 Safari/605.1.15",
    "Mozilla/5.0 (X11; Linux x86_64; rv:122.0) Gecko/20100101 Firefox/122.0",
    "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:121.0) Gecko/20100101 Firefox/121.0",
    "Mozilla/5.0 (Linux; Android 14; Pixel 8) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/130.0.0.0 Mobile Safari/537.36",
    "Mozilla/5.0 (iPhone; CPU iPhone OS 17_3 like Mac OS X) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/17.3 Mobile/15E148 Safari/604.1",
    "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/130.0.0.0 Safari/537.36 Edg/130.0.0.0"
  };
  return Pool[(size_t)rng.NextRangeU64(Pool.size())];
}

static inline string GenFakeUa(XorShift128Plus &rng) {
  int family = rng.NextInt(0, 2);
  int platform = rng.NextInt(0, 4);

  bool isWindows = platform == 0;
  bool isMac = platform == 1;
  bool isLinux = platform == 2;
  bool isMobile = platform == 3;

  if (family == 0) {
    bool edge = rng.NextInt(0, 4) == 0;
    bool android = isMobile && rng.NextBool();
    bool iphone = isMobile && !android && rng.NextInt(0, 3) == 0;
    bool ipad = isMobile && !android && !iphone && rng.NextInt(0, 4) == 0;

    if (android || iphone || ipad) isWindows = isMac = isLinux = false;

    return BuildChromeLike(rng, edge, isMobile, android, isWindows, isMac, isLinux, iphone, ipad);
  }

  if (family == 1) {
    bool android = isMobile && rng.NextBool();
    if (android) return BuildFirefoxLike(rng, true, true, false, false, false);
    return BuildFirefoxLike(rng, false, false, isWindows, isMac, isLinux);
  }

  bool iphone = isMobile && rng.NextInt(0, 2) == 0;
  bool ipad = isMobile && !iphone && rng.NextInt(0, 3) == 0;
  bool mac = !isMobile || rng.NextInt(0, 3) == 0;
  return BuildSafariLike(rng, iphone, ipad, mac);
}

static inline string GetArgValue(int argc, char **argv, const string &key, const string &def = "") {
  for (int i = 1; i < argc; i++) {
    string a = argv[i];
    if (a == key && i + 1 < argc) return argv[i + 1];
    if (a.rfind(key + "=", 0) == 0) return a.substr(key.size() + 1);
  }
  return def;
}

static inline bool HasFlag(int argc, char **argv, const string &key) {
  for (int i = 1; i < argc; i++) if (string(argv[i]) == key) return true;
  return false;
}

int main(int argc, char **argv) {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  string nStr = GetArgValue(argc, argv, "--n", "1000");
  string mode = GetArgValue(argc, argv, "--mode", "both");
  string outPath = GetArgValue(argc, argv, "--out", "useragents.txt");
  string seedStr = GetArgValue(argc, argv, "--seed", "");

  uint64_t seed = 0;
  if (!seedStr.empty()) {
    try { seed = stoull(seedStr); } catch (...) { seed = 0; }
  }

  long long n = 1000;
  try { n = stoll(nStr); } catch (...) { n = 1000; }
  if (n < 1) n = 1;

  bool doReal = (mode == "real" || mode == "both");
  bool doFake = (mode == "fake" || mode == "both");
  if (!doReal && !doFake) { doReal = doFake = true; }

  XorShift128Plus rng(seed);

  unordered_set<string> seen;
  seen.reserve((size_t)min<long long>(n * 2, 2000000LL));

  ofstream out(outPath, ios::binary);
  if (!out) {
    cerr << "Cannot open output: " << outPath << "\n";
    return 1;
  }

  string line;
  line.reserve(256);

  long long produced = 0;
  while (produced < n) {
    string ua;
    if (doReal && doFake) ua = (rng.NextBool() ? PickRealUa(rng) : GenFakeUa(rng));
    else if (doReal) ua = PickRealUa(rng);
    else ua = GenFakeUa(rng);

    if (seen.emplace(ua).second) {
      out.write(ua.data(), (streamsize)ua.size());
      out.put('\n');
      produced++;
    }
  }

  return 0;
}