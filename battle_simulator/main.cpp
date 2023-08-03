#include <iostream>
#include <map>

enum class Type {
  FIRE, WATER, GRASS
};

class Move {
};

class Monster {
  Type type;
  std::map<unsigned int, Move> moveset;
public:
  Monster(Type type, std::map<unsigned int, Move>& moveset) : type(type), moveset(moveset) {}
};