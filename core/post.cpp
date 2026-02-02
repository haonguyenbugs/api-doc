#include <curl/curl.h>
#include <string>
#include <map>
#include <stdexcept>

using StrMap = std::map<std::string, std::string>;

static size_t WriteCb(char* ptr, size_t size, size_t nmemb, void* userdata) {
  auto* s = (std::string*)userdata;
  s->append(ptr, size * nmemb);
  return size * nmemb;
}

static inline void AddHeaders(struct curl_slist*& lst, const StrMap& h) {
  for (auto& [k, v] : h) {
    std::string line = k + ": " + v;
    lst = curl_slist_append(lst, line.c_str());
  }
}

struct HttpResp {
  long status = 0;
  std::string body;
};

class HttpSession {
  CURL* c = nullptr;
  std::string cookieFile;
  std::string lastErr;

public:
  explicit HttpSession(std::string cookieJar = "cookies.txt") : cookieFile(std::move(cookieJar)) {
    c = curl_easy_init();
    if (!c) throw std::runtime_error("curl_easy_init failed");

    curl_easy_setopt(c, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(c, CURLOPT_ACCEPT_ENCODING, "");
    curl_easy_setopt(c, CURLOPT_TCP_KEEPALIVE, 1L);

    curl_easy_setopt(c, CURLOPT_COOKIEFILE, cookieFile.c_str());
    curl_easy_setopt(c, CURLOPT_COOKIEJAR, cookieFile.c_str());

    curl_easy_setopt(c, CURLOPT_TIMEOUT, 30L);
    curl_easy_setopt(c, CURLOPT_CONNECTTIMEOUT, 10L);

    curl_easy_setopt(c, CURLOPT_ERRORBUFFER, ErrBuf());
  }

  ~HttpSession() {
    if (c) curl_easy_cleanup(c);
  }

  HttpResp PostJson(const std::string& url, const std::string& json,
                    const StrMap& headers = {}, const std::string& bearer = "") {
    std::string resp;
    struct curl_slist* hdr = nullptr;

    StrMap h = headers;
    h.emplace("Content-Type", "application/json");
    h.emplace("Accept", "application/json");
    if (!bearer.empty()) h["Authorization"] = "Bearer " + bearer;

    AddHeaders(hdr, h);

    curl_easy_setopt(c, CURLOPT_URL, url.c_str());
    curl_easy_setopt(c, CURLOPT_HTTPHEADER, hdr);

    curl_easy_setopt(c, CURLOPT_POST, 1L);
    curl_easy_setopt(c, CURLOPT_POSTFIELDS, json.c_str());
    curl_easy_setopt(c, CURLOPT_POSTFIELDSIZE, (long)json.size());

    curl_easy_setopt(c, CURLOPT_WRITEFUNCTION, WriteCb);
    curl_easy_setopt(c, CURLOPT_WRITEDATA, &resp);

    CURLcode rc = curl_easy_perform(c);
    long code = 0;
    curl_easy_getinfo(c, CURLINFO_RESPONSE_CODE, &code);

    curl_slist_free_all(hdr);

    if (rc != CURLE_OK) throw std::runtime_error(std::string("curl: ") + curl_easy_strerror(rc));
    return {code, std::move(resp)};
  }

  HttpResp PostForm(const std::string& url, const StrMap& form,
                    const StrMap& headers = {}) {
    std::string resp;
    struct curl_slist* hdr = nullptr;

    AddHeaders(hdr, headers);

    curl_mime* mime = curl_mime_init(c);
    for (auto& [k, v] : form) {
      curl_mimepart* part = curl_mime_addpart(mime);
      curl_mime_name(part, k.c_str());
      curl_mime_data(part, v.c_str(), CURL_ZERO_TERMINATED);
    }

    curl_easy_setopt(c, CURLOPT_URL, url.c_str());
    curl_easy_setopt(c, CURLOPT_HTTPHEADER, hdr);
    curl_easy_setopt(c, CURLOPT_MIMEPOST, mime);

    curl_easy_setopt(c, CURLOPT_WRITEFUNCTION, WriteCb);
    curl_easy_setopt(c, CURLOPT_WRITEDATA, &resp);

    CURLcode rc = curl_easy_perform(c);
    long code = 0;
    curl_easy_getinfo(c, CURLINFO_RESPONSE_CODE, &code);

    curl_mime_free(mime);
    curl_slist_free_all(hdr);

    if (rc != CURLE_OK) throw std::runtime_error(std::string("curl: ") + curl_easy_strerror(rc));
    return {code, std::move(resp)};
  }

private:
  char* ErrBuf() {
    lastErr.assign(CURL_ERROR_SIZE, '\0');
    return lastErr.data();
  }
};

int main() {
  curl_global_init(CURL_GLOBAL_DEFAULT);

  try {
    HttpSession s("session.cookies");

    auto r1 = s.PostJson(
      "https://httpbin.org/post",
      R"({"username":"test","password":"123"})",
      {{"User-Agent", "CppSession/1.0"}}
    );

    auto r2 = s.PostJson(
      "https://httpbin.org/post",
      R"({"action":"ping"})"
    );

    printf("r1 status=%ld\n%s\n\n", r1.status, r1.body.c_str());
    printf("r2 status=%ld\n%s\n", r2.status, r2.body.c_str());
  } catch (const std::exception& e) {
    fprintf(stderr, "err: %s\n", e.what());
  }

  curl_global_cleanup();
  return 0;
}