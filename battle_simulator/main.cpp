#include <iostream>
#include <map>
#include <vector>

enum class Type {
  FIRE, WATER, GRASS, NORMAL,
};

class Move {
  Type type;
public:
  const unsigned int power;
  Move() : type(Type::NORMAL), power(0) {} //same as MonsterSpecies class
  Move(Type type, unsigned int power) : type(type), power(power) {}
};

std::map<unsigned int, Move> MoveList {
  { 1, Move(Type::NORMAL, 10) }, // tackle
};

class MonsterSpecies {
  Type type;
  std::map<unsigned int, Move> moveset;
public:
  MonsterSpecies() : type(Type::NORMAL) {}    // not valid but I need this for map operator[] to work... maybe I should not use mape operator[] ? // or I could just construct a valid MonsterSpecies, e.g. species 1
  MonsterSpecies(Type type) : type(type) {}
  //MonsterSpecies(Type type, std::map<unsigned int, Move>& moveset) : type(type), moveset(moveset) {}
};

std::map<unsigned int, MonsterSpecies> MonsterEncyclopedia {
  { 1, MonsterSpecies(Type::GRASS) },
  { 4, MonsterSpecies(Type::FIRE) },
};

class Monster {
  const MonsterSpecies species;
  std::vector<Move> moves;
  int hp;
  bool alive;
public:
  Monster(unsigned int id) : species(MonsterEncyclopedia[id]), hp(25), alive(true) {}
  void addMove(Move m) {
    moves.push_back(m);
  }
  const int& getHp() {
    return hp;
  }
  Move& getMove(unsigned int n) {
    return moves[n];
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

  Monster grassman { 1 };
  grassman.addMove(MoveList[1]);

  Monster firecreature { 4 };
  firecreature.addMove(MoveList[1]);

  std::vector<Monster> team1;
  std::vector<Monster> team2;

  std::cout << "GAME START" << std::endl;

  turn = 1;
  while (turn != 0) {
    std::cout << "grassman has " << grassman.getHp() << "HP" << std::endl;
    std::cout << "firecreature has " << firecreature.getHp() << "HP" << std::endl;

    int move;
    std::cout << "what move?" << std::endl;
    std::cin >> move;
    
    if (turn == 1) {
      std::cout << "grassman used move " << move << std::endl;
      bool kill = firecreature.receiveAttack(grassman.getMove(move));
      if (kill) {
        "grassman wins!";
        turn = 0;
        continue;
      }
      turn = 2;
    } else if (turn == 2) {
      std::cout << "firecreature used move " << move << std::endl;
      bool kill = grassman.receiveAttack(firecreature.getMove(move));
      if (kill) {
        "firecreature wins!";
        turn = 0;
        continue;
      }
      turn = 1;
    } else {
      //error
      std::cout << "ERROR" << std::endl;
      std::exit(1);
    }
  };

  std::cout << "GAME OVER. THANKS FOR PLAYING" << std::endl;

  return 0;
}