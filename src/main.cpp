#include <ncurses.h>

struct window_data {
  const int height;
  const int width;
};

struct position {
  int y;
  int x;
  bool operator==(const position& rhs) const {
    return y == rhs.y && x == rhs.x;
  }
};

class Player {
public:
  position pos ;
  Player(position pos) : pos(pos) {}
};

void ncurses_setup() {
  initscr();            //start curses mode
  cbreak();             //line buffering disabled
  noecho();             //don't echo() while doing getch()
  keypad(stdscr, TRUE); //enable function keys
  curs_set(0);          //hide cursor
  timeout(0);           //non-blocking getch()
}

int main() {
  ncurses_setup();

  window_data db_wd { 60, 121 };
  WINDOW* db_win = newwin(db_wd.height, db_wd.width, 0, 0);
  box(db_win, 0, 0);

  window_data game_win_wd { db_wd.height - 2, db_wd.width - 2 };
  WINDOW* game_win = subwin(db_win, game_win_wd.height, game_win_wd.width, 1, 1);

  Player player({0, 0});
  
  refresh();
  wrefresh(db_win);

  int input_c;
  do {
    //box(db_win, 0, 0);
    for (int i = 0; i < game_win_wd.height; ++i) {
      for (int j = 0; j < game_win_wd.width; ++j) {
        mvwaddch(game_win, i, j, '.');
      }
    }

    mvwaddch(game_win, player.pos.y, player.pos.x, '@');
    
    wrefresh(game_win);

    input_c = getch();
    //input_c = wgetch(game_win);
    switch (input_c) {
      case KEY_UP:       --player.pos.y; break;
      case KEY_DOWN:     ++player.pos.y; break;
      case KEY_LEFT:     --player.pos.x; break;
      case KEY_RIGHT:    ++player.pos.x; break;
    }
  } while (input_c != 'q' && input_c != 'Q');

  endwin();

  return 0;
}