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
  const std::string getName() const {
    return name;
  }
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
  // Move& getMove(unsigned int n) {
  //   return moves[n];
  // }
  const std::vector<Move>& getMoves() {
    return moves;
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

    team1[i].addMove(MoveList[2]);
    team2[i].addMove(MoveList[2]);

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
    1+getmaxy(enemy_team_win), 1);
  box(player_team_win, 0, 0);

  WINDOW* control_win = subwin(main_win,
    getmaxy(main_win) - 2 - getmaxy(enemy_team_win) - getmaxy(player_team_win),
    getmaxx(main_win)-2, 1+getmaxy(enemy_team_win)+getmaxy(player_team_win), 1);
  box(control_win, 0, 0); 

  bool battleContinues = true;
  int selected_move = 0;

  int turn_count = 0;

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

    mvwprintw(control_win, 1, 1, "YOUR MOVES");
    const std::vector<Move>& player_moves = team1[0].getMoves();
    for (int i = 0; i < player_moves.size(); ++i) {
      if (i == selected_move) {
        wattron(control_win, A_REVERSE);
        mvwprintw(control_win, i+2, 1, player_moves[i].getName().c_str());
        wattroff(control_win, A_REVERSE);
      } else {
        mvwprintw(control_win, i+2, 1, player_moves[i].getName().c_str());
      }
    }

    mvwprintw(control_win, 6, 1, "%d", turn_count);

    wrefresh(main_win);
    wrefresh(enemy_team_win);
    wrefresh(player_team_win);
    wrefresh(control_win);

    int c = wgetch(main_win);
    switch (c) {
      case 'q':
      case 'Q':
        battleContinues = false;
        break;
      case KEY_UP:
        if (selected_move > 0) --selected_move;
        break;
      case KEY_DOWN:
        if (selected_move < player_moves.size()) ++selected_move;
        break;
      default:
        break;  //do nothing if unrecognized key pressed
    }
    
    ++turn_count;
  }
  
  endwin();

  return 0;
}

//made progress
//I don't understand how window refreshing works at all
//I don't understand why every time I press up or down turn_count
//gets incremented 3 times
//...
//this is debugging for tomorrow ... ?