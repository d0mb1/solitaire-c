// libraries
// nejaky knihovny
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// enums for simplicity
// konstanty, at clovek vi na co vubec kouka, protoze nejaka jednicka nikomu nic
// nerekne
enum VALUE { JOKER, ACE, JACK = 11, QUEEN = 12, KING = 13 };
enum SHAPE { SPADE, DIAMOND, HEART, CLUB };
enum COLOR { RED, BLACK };
enum FIELD { TOP, BOTTOM };

// card struct
// struct karty
typedef struct {
  enum VALUE value;
  enum COLOR color;
  enum SHAPE shape;
  bool visibility;
} card;

void shuffle(card *arr, size_t n);
void cardPrint(card arr[]);
void fillField(card arr[], card a);
void printBottomField();
void startGameVisibility();
void moveCard(int from, int what, int to, card joker);
void printTopField();
void topDeckFlip(card joker);
void topDeckMove(int from, int to, card joker);
void clear();
void topLabels();
void bottomLabels();
void moveToTop(int from, int what, card joker);
int gameEndCheck();
void printGameboard();
int trueGameEnd();

// hezky barvicky
#define COLOR_RED "\x1b[31m"
#define COLOR_GREEN "\x1b[32m"
#define COLOR_YELLOW "\x1b[33m"
#define COLOR_BLUE "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN "\x1b[36m"
#define COLOR_RESET "\x1b[0m"

// Define size of playing fields
// Velikost poli jsou definovana tak, aby na maximech vzdy bylo misto pro
// kartu joker, ktera je definovana jako neviditelna karta a pouziva se pro
// jednoduche ukonceni cyklu pri hledani poslednich karet v poli
#define top_field_y 26
#define top_field_x 8
card top_field[top_field_y][top_field_x] = {};
#define bottom_field_y 21
#define bottom_field_x 9
card bottom_field[bottom_field_y][bottom_field_x] = {};
int moves = 0;

int main() {
  // seed pro shuffle funkci, at je to hezky zamichany
  srand(time(NULL));

  // creating all the different cards manually
  // inicializovany vsechny karty
  // Karta joker slouzi jako stopka. Kdyz pomoci while loopu hledam posledni
  // kartu v nejaky sloupci, tak akorat kontroluju, jestli jsem nenarazil na
  // jokera a tim padem je posledni karta nad nim
  const card joker = {0, 0, 0, 0};

  const card ace_of_spades = {1, BLACK, SPADE, 0};
  const card two_of_spades = {2, BLACK, SPADE, 0};
  const card three_of_spades = {3, BLACK, SPADE, 0};
  const card four_of_spades = {4, BLACK, SPADE, 0};
  const card five_of_spades = {5, BLACK, SPADE, 0};
  const card six_of_spades = {6, BLACK, SPADE, 0};
  const card seven_of_spades = {7, BLACK, SPADE, 0};
  const card eight_of_spades = {8, BLACK, SPADE, 0};
  const card nine_of_spades = {9, BLACK, SPADE, 0};
  const card ten_of_spades = {10, BLACK, SPADE, 0};
  const card jack_of_spades = {11, BLACK, SPADE, 0};
  const card queen_of_spades = {12, BLACK, SPADE, 0};
  const card king_of_spades = {13, BLACK, SPADE, 0};

  const card ace_of_clubs = {1, BLACK, CLUB, 0};
  const card two_of_clubs = {2, BLACK, CLUB, 0};
  const card three_of_clubs = {3, BLACK, CLUB, 0};
  const card four_of_clubs = {4, BLACK, CLUB, 0};
  const card five_of_clubs = {5, BLACK, CLUB, 0};
  const card six_of_clubs = {6, BLACK, CLUB, 0};
  const card seven_of_clubs = {7, BLACK, CLUB, 0};
  const card eight_of_clubs = {8, BLACK, CLUB, 0};
  const card nine_of_clubs = {9, BLACK, CLUB, 0};
  const card ten_of_clubs = {10, BLACK, CLUB, 0};
  const card jack_of_clubs = {11, BLACK, CLUB, 0};
  const card queen_of_clubs = {12, BLACK, CLUB, 0};
  const card king_of_clubs = {13, BLACK, CLUB, 0};

  const card ace_of_hearts = {1, RED, HEART, 0};
  const card two_of_hearts = {2, RED, HEART, 0};
  const card three_of_hearts = {3, RED, HEART, 0};
  const card four_of_hearts = {4, RED, HEART, 0};
  const card five_of_hearts = {5, RED, HEART, 0};
  const card six_of_hearts = {6, RED, HEART, 0};
  const card seven_of_hearts = {7, RED, HEART, 0};
  const card eight_of_hearts = {8, RED, HEART, 0};
  const card nine_of_hearts = {9, RED, HEART, 0};
  const card ten_of_hearts = {10, RED, HEART, 0};
  const card jack_of_hearts = {11, RED, HEART, 0};
  const card queen_of_hearts = {12, RED, HEART, 0};
  const card king_of_hearts = {13, RED, HEART, 0};

  const card ace_of_diamonds = {1, RED, DIAMOND, 0};
  const card two_of_diamonds = {2, RED, DIAMOND, 0};
  const card three_of_diamonds = {3, RED, DIAMOND, 0};
  const card four_of_diamonds = {4, RED, DIAMOND, 0};
  const card five_of_diamonds = {5, RED, DIAMOND, 0};
  const card six_of_diamonds = {6, RED, DIAMOND, 0};
  const card seven_of_diamonds = {7, RED, DIAMOND, 0};
  const card eight_of_diamonds = {8, RED, DIAMOND, 0};
  const card nine_of_diamonds = {9, RED, DIAMOND, 0};
  const card ten_of_diamonds = {10, RED, DIAMOND, 0};
  const card jack_of_diamonds = {11, RED, DIAMOND, 0};
  const card queen_of_diamonds = {12, RED, DIAMOND, 0};
  const card king_of_diamonds = {13, RED, DIAMOND, 0};

  // placing all cards in a deck
  // karty nahazeny do jednoho "balicku"
  card deck[52] = {
      ace_of_spades,   two_of_spades,     three_of_spades,   four_of_spades,
      five_of_spades,  six_of_spades,     seven_of_spades,   eight_of_spades,
      nine_of_spades,  ten_of_spades,     jack_of_spades,    queen_of_spades,
      king_of_spades,  ace_of_clubs,      two_of_clubs,      three_of_clubs,
      four_of_clubs,   five_of_clubs,     six_of_clubs,      seven_of_clubs,
      eight_of_clubs,  nine_of_clubs,     ten_of_clubs,      jack_of_clubs,
      queen_of_clubs,  king_of_clubs,     ace_of_hearts,     two_of_hearts,
      three_of_hearts, four_of_hearts,    five_of_hearts,    six_of_hearts,
      seven_of_hearts, eight_of_hearts,   nine_of_hearts,    ten_of_hearts,
      jack_of_hearts,  queen_of_hearts,   king_of_hearts,    ace_of_diamonds,
      two_of_diamonds, three_of_diamonds, four_of_diamonds,  five_of_diamonds,
      six_of_diamonds, seven_of_diamonds, eight_of_diamonds, nine_of_diamonds,
      ten_of_diamonds, jack_of_diamonds,  queen_of_diamonds, king_of_diamonds};

  // shuffeling
  // balicek se 2x zamicha
  shuffle(deck, 52);
  shuffle(deck, 52);

  // fill the playing with the shuffled cards
  // hraci pole se naplni
  fillField(deck, joker);

  // makes the cards that should be visible at the start of the game visible
  // funkce zviditelni spodni karty
  startGameVisibility();

  // inicialising some variables
  // inicializovany nejaky promenny
  int y1 = 0;
  bool end = false;
  int from, from8, what, to;
  int true_end = 0;
  clear();

  // game loop
  // herni loop
  while (end == false) {
    y1 = 0;

    // prints the game board (all the cards in different decks)
    printGameboard();
    // test();

    // choosing the deck from which we want to take a card
    // tady se vybira z jakeho balicku se karta bude brat
    printf("WHICH DECK: ");
    from = 20;
    while (from < 0 || 9 < from) {
      if (scanf("%d", &from) != 1 || from < 0 || 9 < from) {
        while (getchar() != '\n')
          ;
        clear();
        printGameboard();
        printf("WHICH DECK: ");
      }
    }

    // if we're taking a card from one of the finishing decks we need to specify
    // which one it is
    // pokud z osmeho, tak se pak vybira jeste konkretni balicek
    if (from == 8) {
      printf("WHICH DECK: ");
      from8 = 20;
      while (from8 < 1 || 4 < from8) {
        if (scanf("%d", &from8) != 1 || from8 < 1 || 4 < from8) {
          while (getchar() != '\n')
            ;
          clear();
          printGameboard();
          printf("WHICH DECK: %d\nWHICH DECK: ", from);
        }
      }

      // choosing the destination of the card
      // kam se karta hodi
      printf("TO: ");
      to = 20;
      while (to < 1 || 7 < to) {
        if (scanf("%d", &to) != 1 || to < 1 || 7 < to) {
          while (getchar() != '\n')
            ;
          clear();
          printGameboard();
          printf("WHICH DECK: %d\nWHICH DECK: %d\nTO: ", from, from8);
        }
      }

      // function that moves the card
      // funkce, ktera karty presouva
      topDeckMove(from8, to, joker);
      clear();
    } else if (from == 9) {
      // if a card is taken from the 9th deck then it's placed on the num 0 deck
      // pokud se bude brat karta z devateho, tak se akorat otoci na nulty
      // balicek
      topDeckFlip(joker);
      clear();

      // the same stuff. pick a card and choose where it goes
      // porad prakticky to samy... vybere se karta a pak kam se hodi
    } else if (from == 0) {
      y1 = 0;

      // searching for the top card in a deck
      // tady se hleda posledni karta v balicku
      while (top_field[y1][5].value != 0) {
        y1++;
      }

      // if the card is an ace then its automaticaly placed in one of the final
      // decks
      // pokud je ta karta eso, tak se rovnou hodi do balicku 8
      if (top_field[y1 - 1][5].value == ACE) {
        to = 0;
        topDeckMove(from, to, joker);
      } else {
        printf("TO: ");
        to = 20;
        while (to < 1 || 8 < to) {
          if (scanf("%d", &to) != 1 || to < 1 || 8 < to) {
            while (getchar() != '\n')
              ;
            clear();
            printGameboard();
            printf("WHICH DECK: %d\nTO: ", from);
          }
        }
        topDeckMove(from, to, joker);
      }
      clear();
    } else {

      // pretty much the same thing as before with some variation
      printf("CARD: ");
      what = 20;
      while (what < 1 || 13 < what) {
        if (scanf("%d", &what) != 1 || what < 1 || 13 < what) {
          while (getchar() != '\n')
            ;
          clear();
          printGameboard();
          printf("WHICH DECK: %d\nCARD: ", from);
        }
      }
      if (what == ACE) {
        moveToTop(from, what, joker);
        clear();
      } else {
        printf("TO: ");
        to = 20;
        while (to < 1 || 8 < to) {
          if (scanf("%d", &to) != 1 || to < 1 || 8 < to) {
            while (getchar() != '\n')
              ;
            clear();
            printGameboard();
            printf("WHICH DECK: %d\nCARD: %d\nTO: ", from, what);
          }
        }
        if (to == 8) {
          moveToTop(from, what, joker);
          clear();
        } else {
          moveCard(from, what, to, joker);
          clear();
        }
      }
    }

    // here's some swith spagetti to check if the game is finished or not.
    // if the game is winnable then it asks you if you want to continue or if
    // you want to start a new game
    // tady jsou trocha swith spagety. Nevim, jak to udelat lip. Ale prakticky
    // je to tu, aby clovek po dokonceni mohl hrat dal nebo restartovat hru
    switch (true_end) {
    case 0:
      end = gameEndCheck();
      if (end == true) {
        clear();
        printGameboard();
        int choice = 20;
        printf("CONTINUE - 1\nRESTART - 2\nEXIT - 3\n");
        while (choice < 1 || 3 < choice) {
          if (scanf("%d", &choice) != 1 || choice < 1 || 3 < choice) {
            while (getchar() != '\n')
              ;
            clear();
            printGameboard();
            printf("CONTINUE - 1\nRESTART - 2\nEXIT - 3\n");
          }
        }
        switch (choice) {
        case 1:
          end = 0;
          true_end = 1;
          clear();
          break;
        case 2:
          end = 0;
          for (int i = 0; i < 20; i++) {
            for (int j = 0; j < 7; j++) {
              bottom_field[i][j] = joker;
            }
          }
          for (int i = 0; i < 25; i++) {
            for (int j = 0; j < 6; j++) {
              top_field[i][j] = joker;
            }
          }
          shuffle(deck, 52);
          shuffle(deck, 52);

          fillField(deck, joker);
          startGameVisibility();
          clear();
          break;
        case 3:
          return 0;
        }
      }
      break;
    case 1:
      end = trueGameEnd();
      if (end == true) {
        clear();
        printGameboard();
        int choice = 20;
        printf("RESTART - 1\nEXIT - 2\n");
        while (choice < 1 || 2 < choice) {
          if (scanf("%d", &choice) != 1 || choice < 1 || 2 < choice) {
            while (getchar() != '\n')
              ;
            clear();
            printGameboard();
            printf("RESTART - 1\nEXIT - 2\n");
          }
        }
        choice = tolower(choice);
        switch (choice) {
        case 1:
          end = 0;
          true_end = 0;
          moves = 0;
          for (int i = 0; i < 20; i++) {
            for (int j = 0; j < 7; j++) {
              bottom_field[i][j] = joker;
            }
          }
          for (int i = 0; i < 25; i++) {
            for (int j = 0; j < 6; j++) {
              top_field[i][j] = joker;
            }
          }
          shuffle(deck, 52);
          shuffle(deck, 52);

          fillField(deck, joker);
          startGameVisibility();
          clear();
          break;
        case 2:
          return 0;
        }
      }
      break;
    }
  }
}

// ╭─────────╮
// │ 󰣑 󰣏 󰣐 󰣎 │
// ╰─────────╯
// ╭─  ───  ─╮
//   ∷∷∷∷∷∷∷
// │ ∷∷∷∷∷∷∷ │
//   ∷∷∷∷∷∷∷
// │ ∷∷∷∷∷∷∷ │
//   ∷∷∷∷∷∷∷
// ╰─  ───  ─╯

// funkce

// funkce by mela jednoduse hledat zadane karty. Jeste neni dedelany
// int find_card_index(enum FIELD field, enum VALUE card, int where) {
//   int index = 0;
//   if (field == 0) {
//     while (top_field[index][where].value != JOKER) {
//       index++;
//     }
//     index--;
//     return index;
//   } else {
//     while (bottom_field[index][where].value != card) {
//       index++;
//     }
//     return index;
//   }
// }

// function fills game board with all the cards at the start of the game
// funkce, ktera na zacatku hry naplni prazdny pole kartama
void fillField(card arr[], card a) {
  for (int x = 0; x < 7; x++) {
    for (int y = 0; y < 13; y++) {
      bottom_field[y][x] = a;
    }
  }
  int n = 1;
  int m = 0;
  for (int x = 0; x < 7; x++) {
    for (int y = 0; y < n; y++) {
      bottom_field[y][x] = arr[m];
      m++;
    }
    n++;
  }
  for (int i = 0; i < 25; i++) {
    for (int j = 0; j < 6; j++) {
      top_field[i][j] = a;
    }
  }
  for (int i = 0; i < 24; i++) {
    top_field[i][4] = arr[m];
    m++;
  }
}

// this function shuffles the initial deck
// funkce, ktera zamicha karty
void shuffle(card *arr, size_t n) {
  if (n > 1) {
    size_t i;
    for (i = 0; i < n - 1; i++) {
      size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
      card t = arr[j];
      arr[j] = arr[i];
      arr[i] = t;
    }
  }
}

// function that makes all the relevant cards visible at the start of the game
// funkce, ktera zviditelnuje nejvyzsi karty ve hre na zacatku hry
void startGameVisibility() {
  for (int x = 0; x < 7; x++) {
    for (int y = 0; y < 13; y++) {
      if (bottom_field[y][x].value == JOKER) {
        bottom_field[y - 1][x].visibility = true;
      }
    }
  }
}

// checks if the game is winable
// funkce kontroluje, jestli hra uz dohratelna
int gameEndCheck() {
  int counter = 0;
  for (int i = 0; i < 20; i++) {
    for (int j = 0; j < 7; j++) {
      if (bottom_field[i][j].value > 0 &&
          bottom_field[i][j].visibility == true) {
        counter++;
      }
    }
  }
  for (int i = 0; i < 25; i++) {
    for (int j = 0; j < 6; j++) {
      if (top_field[i][j].value > 0 && top_field[i][j].visibility == true) {
        counter++;
      }
    }
  }
  int y = 0;
  while (top_field[y][4].value != JOKER) {
    y++;
  }

  if (counter == 52 - y) {
    printf("Hra zkoncila\n");
    return 1;
  } else {
    return 0;
  }
}

// checks if the game is compleatly finished
// funkce, ktera kontroluje, jestli je hra uplne u konce a vsechny karty jsou ve
// finalnim balicku
int trueGameEnd() {
  int counter = 0;
  for (int i = 0; i < 25; i++) {
    for (int j = 0; j < 4; j++) {
      if (top_field[i][j].value > 0) {
        counter++;
      }
    }
  }
  if (counter == 52) {
    return 1;
  } else {
    return 0;
  }
}

// moves cards only in the bottom fields
// funkce ktera pohybuje s kartami pouze ve spodnim poli
// Ty karty se prakticky akorat presunou a na misto, od kud se karta posouvala
// se vlozi joker
void moveCard(int from, int what, int to, card joker) {
  from--;
  to--;

  int y_from;
  for (int i = 0; i < 20; i++) {
    if (bottom_field[i][from].visibility == true &&
        bottom_field[i][from].value == what) {
      y_from = i;
      break;
    } else if (i == 19 && bottom_field[i][from].value != what) {
      return;
    }
  }

  int y_to;
  for (int i = 0; i < 20; i++) {
    if (bottom_field[i][to].value == JOKER) {
      y_to = i;
      break;
    }
  }

  if (bottom_field[y_from][from].value ==
          bottom_field[y_to - 1][to].value - 1 &&
      bottom_field[y_from][from].color != bottom_field[y_to - 1][to].color) {

    if (y_from != 0) {
      bottom_field[y_from - 1][from].visibility = true;
    }

    while (bottom_field[y_from][from].value != 0) {
      bottom_field[y_to][to] = bottom_field[y_from][from];
      bottom_field[y_from][from] = joker;
      y_from++;
      y_to++;
    }
    moves++;
  } else if (what == KING && bottom_field[0][to].value == JOKER) {
    if (y_from != 0) {
      bottom_field[y_from - 1][from].visibility = true;
    }

    while (bottom_field[y_from][from].value != JOKER) {
      bottom_field[y_to][to] = bottom_field[y_from][from];
      bottom_field[y_from][from] = joker;
      y_from++;
      y_to++;
    }
    moves++;
  }
}

// moves a card from deck 9 to deck 0
// akorat presouva kartu z balicku 9 do balicku 0
void topDeckFlip(card joker) {
  if (top_field[0][4].value == JOKER) {
    int y1 = 0;
    while (top_field[y1][5].value != JOKER) {
      y1++;
    }
    int y2 = y1;
    y1--;
    for (int i = 0; i < y2; i++) {
      top_field[i][4] = top_field[y1][5];
      top_field[i][4].visibility = false;
      top_field[y1][5] = joker;
      y1--;
    }
  } else {
    int y1 = 0;
    int y2 = 0;
    while (top_field[y1][4].value != JOKER) {
      y1++;
    }
    // printf("\n%d\n", y1);
    while (top_field[y2][5].value != JOKER) {
      y2++;
    }
    // printf("\n%d\n", y2);
    top_field[y2][5] = top_field[y1 - 1][4];
    top_field[y2][5].visibility = true;
    top_field[y1 - 1][4] = joker;
  }
  moves++;
}

// moves cards between decks
// presouva karty z mista na misto
void topDeckMove(int from, int to, card joker) {
  int y1 = 0;
  int y2 = 0;
  if (0 < from && from < 5) {
    from--;
    to--;
    if (top_field[0][from].value == JOKER) {
      return;
    }
    while (top_field[y1][from].value != JOKER) {
      y1++;
    }
    if (top_field[y1 - 1][from].value == ACE) {
      return;
    }
    while (bottom_field[y2][to].value != JOKER) {
      y2++;
    }
    if (top_field[y1 - 1][from].value == bottom_field[y2 - 1][to].value - 1 &&
        top_field[y1 - 1][from].color != bottom_field[y2 - 1][to].color) {
      bottom_field[y2][to] = top_field[y1 - 1][from];
      top_field[y1 - 1][from] = joker;
      moves++;
    }
  } else {
    if (top_field[0][5].value == JOKER) {
      return;
    }
    while (top_field[y1][5].value != JOKER) {
      y1++;
    }
    if (top_field[y1 - 1][5].value == ACE) {
      for (int x = 0; x < 4; x++) {
        if (top_field[0][x].value == JOKER) {
          top_field[0][x] = top_field[y1 - 1][5];
          top_field[y1 - 1][5] = joker;
          moves++;
          return;
        }
      }
    }
    if (to == 8) {
      for (int x = 0; x < 4; x++) {
        if (top_field[0][x].shape == top_field[y1 - 1][5].shape) {
          while (top_field[y2][x].value != JOKER) {
            y2++;
          }
          if (top_field[y2 - 1][x].value == top_field[y1 - 1][5].value - 1) {
            top_field[y2][x] = top_field[y1 - 1][5];
            top_field[y1 - 1][5] = joker;
            moves++;
            return;
          }
        }
      }
      return;
    }
    to--;
    if (top_field[y1 - 1][5].value != 13 &&
        bottom_field[0][to].value == JOKER) {
      return;
    }
    if (top_field[y1 - 1][5].value == 13 &&
        bottom_field[0][to].value == JOKER) {
      bottom_field[0][to] = top_field[y1 - 1][5];
      top_field[y1 - 1][5] = joker;
      moves++;
      return;
    }
    while (bottom_field[y2][to].value != JOKER) {
      y2++;
    }
    if (top_field[y1 - 1][5].color != bottom_field[y2 - 1][to].color &&
        top_field[y1 - 1][5].value == bottom_field[y2 - 1][to].value - 1) {
      bottom_field[y2][to] = top_field[y1 - 1][5];
      top_field[y1 - 1][5] = joker;
      moves++;
    }
  }
}

// function that automaticaly picks to which finishing deck should card move to
// funkce, ktera automaticky presouva karty do spravnych finalnich balicku hry
void moveToTop(int from, int what, card joker) {
  from--;
  int y1 = 0;
  while (bottom_field[y1][from].value != JOKER) {
    y1++;
  }
  if (bottom_field[y1 - 1][from].value == ACE) {
    for (int i = 0; i < 4; i++) {
      if (top_field[0][i].value == 0) {
        top_field[0][i] = bottom_field[y1 - 1][from];
        bottom_field[y1 - 1][from] = joker;
        if (y1 - 1 != 0) {
          bottom_field[y1 - 2][from].visibility = true;
        }
        moves++;
        break;
      }
    }
  }
  int x = 10;
  for (int i = 0; i < 4; i++) {
    if (bottom_field[y1 - 1][from].shape == top_field[0][i].shape) {
      x = i;
      break;
    }
  }
  if (x == 10) {
    return;
  }
  int y2 = 0;
  while (top_field[y2][x].value != JOKER) {
    y2++;
  }
  if (top_field[y2 - 1][x].value == bottom_field[y1 - 1][from].value - 1) {
    top_field[y2][x] = bottom_field[y1 - 1][from];
    bottom_field[y1 - 1][from] = joker;
    if (y1 - 1 != 0) {
      bottom_field[y1 - 2][from].visibility = true;
    }
    moves++;
  }
}

// unified function that prints the gameboard
// jednotna funkce, co printuje hraci pole
void printGameboard() {
  topLabels();
  printTopField();
  printf("\n");
  bottomLabels();
  printBottomField();
}

// clearing the terminal
// cisti terminal
void clear() { printf("\e[1;1H\e[2J"); }

// printing lables of the decks
void topLabels() {
  printf("╭───────────────────── 8 ─────────────────────╮\n╭─── 1 ───╮ ╭─── 2 "
         "───╮ ╭─── 3 ───╮ ╭─── 4 ───╮  MOVES:%3d  ╭─── 9 "
         "───╮ ╭─── 0 ───╮\n",
         moves);
}
void bottomLabels() {
  printf("╭─── 1 ───╮ ╭─── 2 ───╮ ╭─── 3 ───╮ ╭─── 4 ───╮ ╭─── 5 ───╮ ╭─── 6 "
         "───╮ ╭─── 7 ───╮ \n");
}

// prints the bottom playing field
// funkce printuje z 2D array field karty do terminalu
void printBottomField() {
  int ukonceni = 0;
  for (int y = 0; y < 20; y++) {
    for (int x = 0; x < 7; x++) {
      if (bottom_field[0][x].value == JOKER && y == 0) {
        printf("╭─  ───  ─╮ ");
      } else if (bottom_field[0][x].value == JOKER && y == 1) {
        printf("│         │ ");
      } else if (bottom_field[0][x].value == JOKER && y == 2) {
        printf("│         │ ");
      } else if (bottom_field[0][x].value == JOKER && y == 3) {
        printf("╰─  ───  ─╯ ");
      } else if (bottom_field[y][x].value == JOKER) {
        if (bottom_field[y - 1][x].value != JOKER) {
          printf("│         │ ");
        } else if (bottom_field[y - 2][x].value != JOKER) {
          printf("│         │ ");
        } else if (bottom_field[y - 3][x].value != JOKER) {
          printf("╰─────────╯ ");
        } else {
          printf("            ");
        }
        ukonceni++;
      } else {
        printf("╭─────────╮ ");
      }
    }
    printf("\n");
    // this here checks when to stop printing to preserve screen space
    // tady akorat se kontroluje, jak jsou sloupce vysoky, aby to porad
    // neprintovalo prazdny pole a vlezlo se vsechno dynamicky na obrazovku
    if (ukonceni == 21) {
      return;
    } else if (ukonceni < 7) {
      ukonceni = 0;
    }
    for (int x = 0; x < 7; x++) {
      if (bottom_field[y][x].value == JOKER) {
        if (bottom_field[y - 1][x].value != JOKER) {
          if (bottom_field[y - 1][x].shape == SPADE) {
            printf("│    󰣑    │ ");
          } else if (bottom_field[y - 1][x].shape == CLUB) {
            printf("│    󰣎    │ ");
          } else if (bottom_field[y - 1][x].shape == HEART) {
            printf("│    ");
            printf(COLOR_RED "󰣐" COLOR_RESET);
            printf("    │ ");
          } else {
            printf("│    ");
            printf(COLOR_RED "󰣏" COLOR_RESET);
            printf("    │ ");
          }
        } else if (bottom_field[y - 2][x].value != JOKER) {
          if (bottom_field[y - 2][x].color == BLACK) {
            if (bottom_field[y - 2][x].shape == SPADE) {
              printf("│ 󰣑");
            } else {
              printf("│ 󰣎");
            }
            if (bottom_field[y - 2][x].value == ACE) {
              printf("     A │ ");
            } else if (bottom_field[y - 2][x].value == JACK) {
              printf("     J │ ");
            } else if (bottom_field[y - 2][x].value == QUEEN) {
              printf("     Q │ ");
            } else if (bottom_field[y - 2][x].value == KING) {
              printf("     K │ ");
            } else if (bottom_field[y - 2][x].value == 10) {
              printf("     10│ ");
            } else {
              printf("     %d │ ", bottom_field[y - 2][x].value);
            }
          } else if (bottom_field[y - 2][x].color == RED) {
            if (bottom_field[y - 2][x].shape == HEART) {
              printf("│ ");
              printf(COLOR_RED "󰣐" COLOR_RESET);
            } else {
              printf("│ ");
              printf(COLOR_RED "󰣏" COLOR_RESET);
            }
            if (bottom_field[y - 2][x].value == ACE) {
              printf(COLOR_RED "     A " COLOR_RESET);
              printf("│ ");
            } else if (bottom_field[y - 2][x].value == JACK) {
              printf(COLOR_RED "     J " COLOR_RESET);
              printf("│ ");
            } else if (bottom_field[y - 2][x].value == QUEEN) {
              printf(COLOR_RED "     Q " COLOR_RESET);
              printf("│ ");
            } else if (bottom_field[y - 2][x].value == KING) {
              printf(COLOR_RED "     K " COLOR_RESET);
              printf("│ ");
            } else if (bottom_field[y - 2][x].value == 10) {
              printf(COLOR_RED "     10" COLOR_RESET);
              printf("│ ");
            } else {
              printf(COLOR_RED "     %d " COLOR_RESET,
                     bottom_field[y - 2][x].value);
              printf("│ ");
            }
          }
        } else {
          printf("            ");
        }
      } else if (bottom_field[y][x].visibility == false) {
        printf("│ ∷∷∷∷∷∷∷ │ ");
      } else if (bottom_field[y][x].color == BLACK) {
        if (bottom_field[y][x].value == ACE) {
          printf("│ A");
        } else if (bottom_field[y][x].value == JACK) {
          printf("│ J");
        } else if (bottom_field[y][x].value == QUEEN) {
          printf("│ Q");
        } else if (bottom_field[y][x].value == KING) {
          printf("│ K");
        } else {
          printf("│%2d", bottom_field[y][x].value);
        }
        if (bottom_field[y][x].shape == SPADE) {
          printf("     󰣑 │ ");
        } else {
          printf("     󰣎 │ ");
        }
      } else if (bottom_field[y][x].color == RED) {
        if (bottom_field[y][x].value == ACE) {
          printf("│");
          printf(COLOR_RED " A" COLOR_RESET);
        } else if (bottom_field[y][x].value == JACK) {
          printf("│");
          printf(COLOR_RED " J" COLOR_RESET);
        } else if (bottom_field[y][x].value == QUEEN) {
          printf("│");
          printf(COLOR_RED " Q" COLOR_RESET);
        } else if (bottom_field[y][x].value == KING) {
          printf("│");
          printf(COLOR_RED " K" COLOR_RESET);
        } else {
          printf("│");
          printf(COLOR_RED "%2d" COLOR_RESET, bottom_field[y][x].value);
        }
        if (bottom_field[y][x].shape == HEART) {
          printf(COLOR_RED "     󰣐 " COLOR_RESET);
        } else {
          printf(COLOR_RED "     󰣏 " COLOR_RESET);
        }
        printf("│ ");
      }
    }
    printf("\n");
  }
}

// this is practicaly the same as the previous function but for the top field
// prakticky to stejny jako predchozi funkce, ale printuje horni cast hraciho
// pole
void printTopField() {
  int y = 0;
  for (int x = 0; x < 6; x++) {
    if (x == 4) {
      printf(COLOR_RED "╭─────────╮ " COLOR_RESET);
    }
    if (top_field[0][x].value == JOKER) {
      printf("╭─  ───  ─╮ ");
    } else {
      printf("╭─────────╮ ");
    }
  }
  printf("\n");
  for (int x = 0; x < 6; x++) {
    if (x == 4) {
      printf(COLOR_RED "│ 󰣐" COLOR_RESET);
      printf("     󰣑 ");
      printf(COLOR_RED "│ " COLOR_RESET);
    }
    if (top_field[0][x].value == JOKER) {
      printf("            ");
    } else {
      while (top_field[y][x].value != JOKER) {
        y++;
      }
      if (top_field[y - 1][x].visibility == false) {
        printf("│ ∷∷∷∷∷∷∷ │ ");
      } else {
        if (top_field[y - 1][x].color == BLACK) {
          if (top_field[y - 1][x].value == KING) {
            printf("│ K     ");
          } else if (top_field[y - 1][x].value == QUEEN) {
            printf("│ Q     ");
          } else if (top_field[y - 1][x].value == JACK) {
            printf("│ J     ");
          } else if (top_field[y - 1][x].value == 10) {
            printf("│10     ");
          } else if (top_field[y - 1][x].value == ACE) {
            printf("│ A     ");
          } else {
            printf("│ %d     ", top_field[y - 1][x].value);
          }
          if (top_field[y - 1][x].shape == SPADE) {
            printf("󰣑 │ ");
          } else {
            printf("󰣎 │ ");
          }
        } else {
          if (top_field[y - 1][x].value == KING) {
            printf("│ ");
            printf(COLOR_RED "K     " COLOR_RESET);
          } else if (top_field[y - 1][x].value == QUEEN) {
            printf("│ ");
            printf(COLOR_RED "Q     " COLOR_RESET);
          } else if (top_field[y - 1][x].value == JACK) {
            printf("│ ");
            printf(COLOR_RED "J     " COLOR_RESET);
          } else if (top_field[y - 1][x].value == 10) {
            printf("│");
            printf(COLOR_RED "10     " COLOR_RESET);
          } else if (top_field[y - 1][x].value == ACE) {
            printf("│ ");
            printf(COLOR_RED "A     " COLOR_RESET);
          } else {
            printf("│ ");
            printf(COLOR_RED "%d     " COLOR_RESET, top_field[y - 1][x].value);
          }
          if (top_field[y - 1][x].shape == HEART) {
            printf(COLOR_RED "󰣐" COLOR_RESET);
            printf(" │ ");
          } else {
            printf(COLOR_RED "󰣏" COLOR_RESET);
            printf(" │ ");
          }
        }
      }
    }
    y = 0;
  }
  printf("\n");
  for (int x = 0; x < 6; x++) {
    if (x == 4) {
      printf(COLOR_RED "│" COLOR_RESET);
      printf("  S ");
      printf(COLOR_RED "O" COLOR_RESET);
      printf(" L  ");
      printf(COLOR_RED "│ " COLOR_RESET);
    }
    if (top_field[0][x].value == JOKER) {
      printf("│         │ ");
    } else {
      while (top_field[y][x].value != JOKER) {
        y++;
      }
      if (top_field[y - 1][x].visibility == true) {
        printf("│         │ ");
      } else {
        printf("│ ∷∷∷∷∷∷∷ │ ");
      }
    }
    y = 0;
  }
  printf("\n");
  for (int x = 0; x < 6; x++) {
    if (x == 4) {
      printf(COLOR_RED "│  I" COLOR_RESET);
      printf(" T ");
      printf(COLOR_RED "A  │ " COLOR_RESET);
    }
    if (top_field[0][x].value == JOKER) {
      printf("            ");
    } else {
      while (top_field[y][x].value != JOKER) {
        y++;
      }
      if (top_field[y - 1][x].visibility == false) {
        printf("│ ∷∷∷∷∷∷∷ │ ");
      } else {
        if (top_field[y - 1][x].shape == SPADE) {
          printf("│    󰣑    │ ");
        } else if (top_field[y - 1][x].shape == CLUB) {
          printf("│    󰣎    │ ");
        } else if (top_field[y - 1][x].shape == HEART) {
          printf("│    ");
          printf(COLOR_RED "󰣐" COLOR_RESET);
          printf("    │ ");
        } else {
          printf("│    ");
          printf(COLOR_RED "󰣏" COLOR_RESET);
          printf("    │ ");
        }
      }
    }
    y = 0;
  }
  printf("\n");
  for (int x = 0; x < 6; x++) {
    if (x == 4) {
      printf(COLOR_RED "│" COLOR_RESET);
      printf("  I ");
      printf(COLOR_RED "R" COLOR_RESET);
      printf(" E  ");
      printf(COLOR_RED "│ " COLOR_RESET);
    }
    if (top_field[0][x].value == JOKER) {
      printf("│         │ ");
    } else {
      while (top_field[y][x].value != JOKER) {
        y++;
      }
      if (top_field[y - 1][x].visibility == true) {
        printf("│         │ ");
      } else {
        printf("│ ∷∷∷∷∷∷∷ │ ");
      }
    }
    y = 0;
  }
  printf("\n");
  for (int x = 0; x < 6; x++) {
    if (x == 4) {
      printf(COLOR_RED "│" COLOR_RESET);
      printf(" 󰣎     ");
      printf(COLOR_RED "󰣏 │ " COLOR_RESET);
    }
    if (top_field[0][x].value == JOKER) {
      printf("            ");
    } else {
      while (top_field[y][x].value != JOKER) {
        y++;
      }
      if (top_field[y - 1][x].visibility == false) {
        printf("│ ∷∷∷∷∷∷∷ │ ");
      } else {
        if (top_field[y - 1][x].color == BLACK) {
          if (top_field[y - 1][x].shape == SPADE) {
            printf("│ 󰣑");
          } else {
            printf("│ 󰣎");
          }
          if (top_field[y - 1][x].value == KING) {
            printf("     K │ ");
          } else if (top_field[y - 1][x].value == QUEEN) {
            printf("     Q │ ");
          } else if (top_field[y - 1][x].value == JACK) {
            printf("     J │ ");
          } else if (top_field[y - 1][x].value == 10) {
            printf("     10│ ");
          } else if (top_field[y - 1][x].value == ACE) {
            printf("     A │ ");
          } else {
            printf("     %d │ ", top_field[y - 1][x].value);
          }
        } else {
          if (top_field[y - 1][x].shape == HEART) {
            printf("│ ");
            printf(COLOR_RED "󰣐" COLOR_RESET);
          } else {
            printf("│ ");
            printf(COLOR_RED "󰣏" COLOR_RESET);
          }
          if (top_field[y - 1][x].value == KING) {
            printf(COLOR_RED "     K" COLOR_RESET);
            printf(" │ ");
          } else if (top_field[y - 1][x].value == QUEEN) {
            printf(COLOR_RED "     Q" COLOR_RESET);
            printf(" │ ");
          } else if (top_field[y - 1][x].value == JACK) {
            printf(COLOR_RED "     J" COLOR_RESET);
            printf(" │ ");
          } else if (top_field[y - 1][x].value == 10) {
            printf(COLOR_RED "     10" COLOR_RESET);
            printf("│ ");
          } else if (top_field[y - 1][x].value == ACE) {
            printf(COLOR_RED "     A" COLOR_RESET);
            printf(" │ ");
          } else {
            printf(COLOR_RED "     %d" COLOR_RESET, top_field[y - 1][x].value);
            printf(" │ ");
          }
        }
      }
    }
    y = 0;
  }
  printf("\n");
  for (int x = 0; x < 6; x++) {
    if (x == 4) {
      printf(COLOR_RED "╰─────────╯ " COLOR_RESET);
    }
    if (top_field[0][x].value == JOKER) {
      printf("╰─  ───  ─╯ ");
    } else {
      printf("╰─────────╯ ");
    }
  }
}
