#include <SPI.h>
#include <Gamebuino.h>


// C'est deux #include sont très importants, ils vont chercher des librairies qui contribuent au fonctionnement de votre code pour la Gamebuino.

Gamebuino gb;
// Créer un objet gamebuino. 

// Variables qui caractérise la balle. En ordre: Sa position, sa vitesse (v) et sa dimension.
int balle_dimension = 4; 
int balle_X = (LCDWIDTH - balle_dimension) / 2;
int balle_Y = rand()% (LCDHEIGHT-2) + 1;

int balle_vX = 2;
int balle_vY = 2;


// Variables qui caractérise les dimensions des deux raquettes. Pas besoin de le faire pour chacun d'eux puisqu'ils auront les mêmes dimensions
int raquette_hauteur1 = 8;
int raquette_hauteur2 = 8;
int raquette_largeur = 4;


// Variables qui définissent la position de la raquette 1 (celle que le joueur peut contrôler)
int raquette1_X = 0;
int raquette1_Y = (LCDHEIGHT - raquette_hauteur1)/2;

// Variables qui définissent la postion de la raquette 2 (celle de l'adversaire)
int raquette2_X = LCDWIDTH - raquette_largeur ;
int raquette2_Y = (LCDHEIGHT - raquette_hauteur2)/2 ;

// Scores
int score_1 ;
int score_2 ;

void setup() {
  // C'est ici que votre code de configuration sera (pour run une seule fois)
  gb.begin();
  gb.titleScreen(F("MON PONG")); 
  gb.battery.show = false;
  }

void loop() {
// C'est ici que la majorité de votre code sera (le code qui run en boucle)
  while (!gb.update());
  gb.display.clear();

// Vérifie si la balle est sortie du côté de l'adversaire, remet la balle de façon aléatoire sur l'axe des y et ajoute un point au joueur
  if (balle_X > LCDWIDTH) {
    balle_X = (LCDWIDTH - balle_dimension) / 2;
    balle_Y = rand() % (LCDHEIGHT-2) + 1;
    balle_vX = 2;
    balle_vY = 2;

    score_1 = score_1 + 1;
    gb.sound.playTick();
    }

// Vérifie si la balle est sortie du côté du joueur, remet la balle de façon aléatoire sur l'axe des y et ajoute un point à l'adversaire
  if (balle_X < 0) {
    balle_X = (LCDWIDTH - balle_dimension) / 2;
    balle_Y = rand()% (LCDHEIGHT-2) + 1;
    balle_vX = 2;
    balle_vY = 2;

    score_2 = score_2 +1;
    gb.sound.playTick();
  }

// Diminution de la grosseur de la raquette
  if (score_1 == 1) {
    raquette_hauteur1 = 7;
  }
  
  if (score_2 == 1) {
    raquette_hauteur2 = 7;
  }
  
  if (score_1 == 2) {
    raquette_hauteur1 = 6;
  }
  
  if (score_2 == 2) {
    raquette_hauteur2 = 6;
  }
  
  if (score_1 == 3) {
    raquette_hauteur1 = 4;
  }
  
  if (score_2 == 3) {
    raquette_hauteur2 = 4;
  }

  if (score_1 == 4) {
    raquette_hauteur1 = 3; 
    gb.popup(F("Continue!"), 40);
  }
  if (score_2 == 4) {
    raquette_hauteur2 = 3;
    gb.popup(F("Ne perd pas!"), 40);
  }

//Revenir au menu
 if (gb.buttons.pressed(BTN_C)) {
  gb.changeGame();
 }
  
//Remmet le score à 0 
  if ((score_1 == 5) || (score_2 == 5)) {
    score_1 = 0;
    score_2 = 0;
    gb.sound.playOK();
    raquette_hauteur1 = 8;
    raquette_hauteur2 = 8;
    gb.popup(F("On recommence!"), 40);
  }  

// Déplacement de la raquette du jouer
  if (gb.buttons.repeat(BTN_UP, 0)) {
    raquette1_Y = max (0, raquette1_Y - 1);
  }

  if (gb.buttons.repeat(BTN_DOWN, 0)) {
    raquette1_Y = min(LCDHEIGHT - raquette_hauteur1, raquette1_Y + 1);
  }
  
// Mouvement constant de la balle
  balle_X  = balle_X + balle_vX ;
  balle_Y = balle_Y + balle_vY ;

// Déplacement de la raquette de l'adversaire
  if (balle_Y > raquette2_Y + raquette_hauteur2 / 2) {
  raquette2_Y = min(LCDHEIGHT - raquette_hauteur2, raquette2_Y + 1.9) ;
  }

  else {
  raquette2_Y = max(0, raquette2_Y - 1.9) ;
  }


// Collision avec le mur du haut et du bas
  if (balle_Y < 0) {
    balle_vY = 2;
    gb.sound.playTick();
  }

  if (balle_Y > LCDHEIGHT - balle_dimension) {
   balle_vY = -2;
   gb.sound.playTick();
  }


// Collision avec la raquette1 (le joueur)
  if ((balle_X == raquette1_X + raquette_largeur) 
    && (balle_Y + balle_dimension >= raquette1_Y)
    && (balle_Y <= raquette1_Y + raquette_hauteur1)) {
    balle_vX = 2;
    gb.sound.playTick();
    }

// Collison avec la raquette2 (l'adversaire)
  if ((balle_X + balle_dimension == raquette2_X) 
    && (balle_Y + balle_dimension >= raquette2_Y)
    && (balle_Y <= raquette2_Y + raquette_hauteur2)) {
    balle_vX = -2;
    gb.sound.playTick();
    }


// Affiche sur l'écran la balle, la raquette1, la raquette2
  gb.display.fillRect(balle_X, balle_Y, balle_dimension, balle_dimension);
  gb.display.fillRect(raquette1_X, raquette1_Y, raquette_largeur, raquette_hauteur1);
  gb.display.fillRect(raquette2_X, raquette2_Y, raquette_largeur, raquette_hauteur2);

// Affiche les scores
  gb.display.cursorX = 35;
  gb.display.cursorY = 5;
  gb.display.print(score_1);
  
  gb.display.cursorX = 42;
  gb.display.cursorY = 5;
  gb.display.print(score_2);
}
