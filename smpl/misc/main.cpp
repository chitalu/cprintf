#define CPF_EXPERIMENTAL
#include <cprintf/cprintf.hpp>

#include <thread>
#include <chrono>

// TODO: fix this entire file
void flag_specification(void) {
  auto big_string = LR"bs(
                    THREAD INDEX(%d).i =
      %d $y*ADD $w*m*SOME$?.c*COLOUR!$?)bs";
      auto f = [&](int p) {
        for (int i(0); i < 5; ++i)
          cprintf<CPF_STDE>(big_string, p, i);
      };

  std::thread threads[20];

  for (int i(0); i < 20; ++i)
    threads[i] = std::thread(f, i);

  for (int i(0); i < 20; ++i)
    threads[i].join();
}

void progress_bar(void) {
  std::size_t c = 0;
  float m = 20.0f;
  std::wstring bar;
  while (++c <= m) {
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    auto progress = (float)c / m * 100.0f;
    if ((((((int)progress / 100) * int(m))) % 5) == 0) {
      bar.append(L"#");
    }
    auto downloaded = ((float)c / m) * 512.0f;
    auto data = std::make_tuple(L"some-big-file", downloaded, progress,
                                512.0f - downloaded, bar.c_str());
     cprintf_t(L"$0,2`filename: $c*%S$?\ndownloaded: %.2f (%.2f%%)\nremaining:"
     "%.2f\nprogress: $gg%S", data);
  }
}

#include <random>

void rand_colours(void) {
  std::vector<wchar_t> colours = { 'r', 'g', 'b', 'y', 'm', 'c', 'w' };
  for (auto i(0); i < 500; ++i) {
    std::this_thread::sleep_for(std::chrono::milliseconds(40));

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(97, 122);
    std::uniform_int_distribution<> h_(0, 40);
    std::uniform_int_distribution<> w_(0, 40);
    std::uniform_int_distribution<> c_(0, 6);

    wchar_t character = dis(gen);
    wchar_t h = h_(gen);
    wchar_t w = w_(gen);

    wchar_t buf[512];
#ifdef _WIN32
    swprintf_s(
#else
    swprintf(
#endif
        buf, 512, L"$%c*.%d,%d %c", colours[c_(gen)], w, h, character);
    
    cprintf(buf);
  }
}

void args_and_colours(void) {
  cprintf(L"foo $r%S\n", L"red");
  cprintf(L"bar $r*%S\n", L"red");
  cprintf(L"quux$g %S\n", L"green");
  cprintf(L"$b%S\n", L"blue");
  cprintf(L"$b*%S\n", L"blue");
}

void buffer_wipe(void) {
  cprintf(L"screen$r buffer $?.!`clear and reset colour\n");
  cprintf(L"screen$r buffer $!`clear\n");
  cprintf(L"screen$r buffer $?` reset colour\n");

  cprintf(L"screen $r buffer $!~ clear but retain cursor position\n");
}

void lull_token(void) {
  /*
  lull token

  this does not necesarilly modify the proceeding-character's
  attributes. Instead it halts printing of the format
  string until keyboard input is recieved.
  In effective, the format string (from first character to the lull
  token occurance) is printed as normal until a lull token is encountered,
  which is the point at which the cprintf waits until keypress/ input
  is recieved.
  */

  cprintf(L"wait for keypress $|printing rest of string...\n");
  cprintf(L"wait for keypress$|.r`rest...\n");
  cprintf(L"wait for keypress $r.|`rest...\n");
}

int main(void) {
  flag_specification();
  std::getchar();
  cprintf(L"$!");
  args_and_colours();
  std::getchar();
  cprintf(L"$!");
  progress_bar();
  std::getchar();
  cprintf(L"$!");
  rand_colours();
  std::getchar();
  cprintf(L"$!");
  lull_token();

  return 0;
}
