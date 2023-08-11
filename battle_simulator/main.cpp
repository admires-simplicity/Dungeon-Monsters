#include <iostream>
#include <map>
#include <vector>
#include <ncurses.h>

enum class Type {
  FIRE, WATER, GRASS, NORMAL,
};

class Move {
  Type type;
  std::string name;
public:
  const unsigned int power;
  Move() : type(Type::NORMAL), power(0) {} //same as MonsterSpecies class
  Move(Type type, unsigned int power, std::string name) : type(type), power(power),
    name(name) {}
};

std::map<unsigned int, Move> MoveList {
  { 1, Move(Type::NORMAL, 10, "Tackle") }, // tackle
  { 2, Move(Type::NORMAL, 1, "Stare")}
};

class MonsterSpecies {
  Type type;
  std::map<unsigned int, Move> moveset;
  std::string name;
public:
  MonsterSpecies() : type(Type::NORMAL) {}    // not valid but I need this for map operator[] to work... maybe I should not use mape operator[] ? // or I could just construct a valid MonsterSpecies, e.g. species 1
  MonsterSpecies(Type type, std::string name) : type(type), name(name) {}
  //MonsterSpecies(Type type, std::map<unsigned int, Move>& moveset) : type(type), moveset(moveset) {}
  std::string getName() const {
    return name;
  }
};

std::map<unsigned int, MonsterSpecies> MonsterEncyclopedia {
  { 1, MonsterSpecies(Type::GRASS, "Grassman") },
  { 4, MonsterSpecies(Type::FIRE, "Firecreature") },
};

class Monster {
  const MonsterSpecies species;
  std::vector<Move> moves;
  int hp;
  bool alive;
  std::string name;
public:
  Monster(unsigned int id) : species(MonsterEncyclopedia[id]), hp(25),
      alive(true) {
    name = species.getName();
  }
  void addMove(Move m) {
    moves.push_back(m);
  }
  const int& getHp() {
    return hp;
  }
  Move& getMove(unsigned int n) {
    return moves[n];
  }

  const char* getName() {
    return name.c_str();
  }

  bool receiveAttack(Move attack) {
    //maybe this should use the visitor pattern for different types of attacks?
    hp -= attack.power;
    if (hp < 0) die();

    if (alive) return false;
    return true;
  }

  void die() {
    alive = false;
  }
};

int main() {
  int turn;

  std::vector<Monster> team1;
  team1.push_back(Monster{ 1 });
  team1.push_back(Monster{ 1 });

  std::vector<Monster> team2;
  team2.push_back(Monster{ 4 });
  team2.push_back(Monster{ 4 });

  for (int i = 0; i < team1.size(); ++i) {
    team1[i].addMove(MoveList[1]);
    team2[i].addMove(MoveList[1]);
  }

  //initialize ncurses
  initscr();            //start curses mode
  cbreak();             //line buffering disabled -- don't have to wait until \n to receive input
  noecho();             //don't echo() while doing getch()
  keypad(stdscr, TRUE); //enable function keys
  curs_set(0);          //hide cursor
  //timeout(0);           //non-blocking getch()

  WINDOW* main_win = newwin(50, 100, 0, 0);
  box(main_win, 0, 0);

  wmove(main_win, 1, 1);
  
  WINDOW* enemy_team_win = subwin(main_win, 20, getmaxx(main_win) - 2, 1, 1);
  box(enemy_team_win, 0, 0);

  WINDOW* player_team_win = subwin(main_win, 20, getmaxx(main_win) - 2,
    getmaxy(enemy_team_win)+1, 1);
  box(player_team_win, 0, 0);

  bool battleContinues = true;

  while (battleContinues) {
    mvwprintw(enemy_team_win, 1, 1, "ENEMY TEAM");
    for (int i = 0; i < team1.size(); ++i) {
      mvwprintw(enemy_team_win, i+2, 1, team1[i].getName());
      mvwprintw(enemy_team_win, i+2, 21, "%3d", team1[i].getHp());    
    }

    mvwprintw(player_team_win, 1, 1, "YOUR TEAM");
      for (int i = 0; i < team2.size(); ++i) {
      mvwprintw(player_team_win, i+2, 1, team2[i].getName());
      mvwprintw(player_team_win, i+2, 21, "%3d", team2[i].getHp());    
    }
  }



  //getch(); // equivalent to wgetch(stdscr);
  //         // wgetch(win) does wrefresh(win) then reads input, so getch refreshes
  //         // stdscr then reads input
  
  // just calling getch, or just refresh()ing won't actually update the screen
  // because refresh outputs stdscr, but I'm not actually writing anything
  // to stdscr, I'm writing to window, so I need to refresh that window.

  // i.e. I don't think contents of windows are actually composed 
  // e.g. stdscr doesn't contain the contents of whatever windows are inside itself.

  wgetch(main_win); // refreshes window (outputs content of window to terminal)
                  // and then reads window
  
  // wait a minute... none of those comments make any sense because
  // this whole time I've been calling wgetch(main_win) and it has been updating
  // enemy_team_win and player_team_win too... ???

  endwin();

  return 0;
}