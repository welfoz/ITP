#include <unistd.h>
#include "primlib.h"
#include <stdlib.h>
#include <math.h>
#include <time.h> 
#include <stdio.h>

typedef struct {
  int x;
  int y;
  int size;
  enum color c;
  int fire_angle;
  int is_shot;
  double distance;
  int x_init;
  int y_init;
  int is_explosionBasic;
  int explosion_ctpBasic;
  int fireOdds;
} TargetBullet;


typedef struct {
	int x;
	int y;
	int speed;
	int is_explosion;
	int size;
	enum color c; // target.c means color
  int explosion_cpt; 
  int direction;
  int is_display;
  TargetBullet tb; // -1 if target is not displayed, 0 or 1 if it is
} Target;

struct Bullet {
	int x;
	int y; 
	int size;
	enum color c;
	double fire_angle;
  double distance;
  int is_shot;
  int x_init;
  int fire_cpt;
};

void targetReInit(Target *t) {
  // srand(time( NULL ));
  t->x = 0;
  t->y = 100 + rand() % 500;
  t->speed = 2 + rand() % 7; //speed between 2 and 8
  t->size = 5 + rand() % 20;
  t->is_explosion = 0;
  //t->c = GREEN;
  t->direction = -1;
  // printf("ici");
}

struct Basic {
	int x;
	int y; 
	int size;
	enum color c;
};
 // first function to be called, draw the basic elements 
void draw_basic(double angle, struct Basic *b, int score, int fire_score, int death_score, int numberOfLives, int level_score, int targetLeft_score) {

    
    // double delta_angle= 2.0 * (M_PI/180.0);
    // int x1_barrel = 150 * cos(angle-delta_angle);
    // int y1_barrel = 150 * sin(angle-delta_angle);
    // int x2_barrel = 150 * cos(angle+delta_angle);
    // i y2_barrel = 150 * sin(angle+delta_angle);
    const int width = 70;
    const int decalage = 50;
    const int hauteurY = 135;
    
    
    gfx_filledRect(0, 0, gfx_screenWidth() - 1, gfx_screenHeight() - 1, BLACK);
    // here we draw the cannon shape 
    gfx_filledCircle(b->x, b->y, b->size, b->c);
    // gfx_filledTriangle(b->x, b->y, b->x + x1_barrel, b->y - y1_barrel,
  // b->x + x2_barrel, b->y - y2_barrel, YELLOW);
    gfx_filledTriangle(b->x - width, b->y - width, b->x - decalage, b->y - hauteurY, b->x + width, b->y - width, RED); 
    gfx_filledTriangle(b->x - decalage, b->y - hauteurY, b->x + width, b->y - width, b->x + decalage, b->y - hauteurY, GREEN); 
    char *x = "SCORES";
    char *scoreTarget = "Targets shot";
    char *scoreBullet = "Bullets shot";
    char *deathScore = "Death score";
    char *livesLeft = "Lives left";
    char *levels = "Level";
    char *targetLeft = "Targets left";

    char count [5];
    char fire_count [5];
    char death_count [5];
    char lives_left [5];
    
    char levels_char [5];
    char targetLeft_char [5];
    numberOfLives = numberOfLives - death_score;
    

    //SDL_itoa(score, buf, 10); 
//    itoa(score, buf, 10;

    gfx_textout(10, 10, x, WHITE);
    gfx_textout(150, 50, SDL_itoa(score, count, 10), RED);
    gfx_textout(20, 50, scoreTarget, WHITE);
    gfx_textout(150, 80, SDL_itoa(fire_score, fire_count, 10), RED);
    gfx_textout(150, 110, SDL_itoa(death_score, death_count, 10), RED);
    gfx_textout(20, 80, scoreBullet, WHITE);
    gfx_textout(20, 110, deathScore, WHITE);
    gfx_textout(20, 140, livesLeft, WHITE);
    gfx_textout(150, 140, SDL_itoa(numberOfLives, lives_left, 10), RED);

    gfx_textout(20, 170, targetLeft, WHITE);
    gfx_textout(150, 170, SDL_itoa(targetLeft_score, targetLeft_char, 10), RED);

    gfx_textout(20, 200, levels, WHITE);
    gfx_textout(150, 200, SDL_itoa(level_score, levels_char, 10), RED);
}

void animation(Target *t, int i) 
{
	//t.size +=100;

	int size = t->size + 100;
	int x = t->x;
	int y = t->y;
  // printf("address animation 3: %u  \n", t); 
  gfx_filledCircle(x, y, size, t->c);

  t->is_display = 0; // do not display it as it is explosed

  targetReInit(t);
}


void draw_target(Target *tt, int i)
{	
  gfx_filledRect(tt->x, tt->y, tt->x + tt->size, tt->y - tt->size, tt->c);
 	
  //printf("adress draw_target 2: %u    \n", tt);
	if (tt->is_explosion == 1) 
	{
		animation(tt, i);
	}
}


void move_target(Target *t, int i) {
	if (t->is_explosion == 0)
	{
    // srand(time( NULL ));
    // printf("%d \n", rand % 20);
    if ((rand() % 10) == 1) {
      t->x += t->speed + (rand() % 20) - 10;
    } else {
      t->x += t->speed;
    }
   if (t->direction == 0) {
     t->y += t->speed;
     if ((rand() % 15) == 1) { 
       // change the direction randomly
       t->direction = 1;
     }
   } else {
     t->y -= t->speed;
     if ((rand() % 15) == 1) {
       t->direction = 0;
     }
   }

   if (t->y > 400) {
     t->direction = 1;
   }
   if (t->y < 100) {
     t->direction = 0;
   }
   // y_target = cos(x_target)*50 + 50;
	if (t->x > gfx_screenWidth()) 
	{
    targetReInit(t);
    // printf("targetreinit en action pour %u \n", t);
	}
 }
}

void draw_bullet(struct Bullet *b) {
      b->x = b->distance * cos(b->fire_angle) + b->x_init;
      b->y = -b->distance * sin(b->fire_angle) + gfx_screenHeight();
      const int widthDecalage = 10;
      const int heightDecalage = 40;
      const int partDecalage = 20;
      const int partDecalage2 = 40;

      gfx_filledTriangle(b->x, b->y, b->x + widthDecalage, b->y + heightDecalage, b->x - widthDecalage, b->y + heightDecalage, b->c);
      gfx_filledTriangle(b->x, b->y + partDecalage, b->x + widthDecalage, b->y + heightDecalage + partDecalage, b->x - widthDecalage, b->y + heightDecalage + partDecalage, b->c);
      gfx_filledTriangle(b->x, b->y + partDecalage2, b->x + widthDecalage, b->y + heightDecalage + partDecalage2, b->x - widthDecalage, b->y + heightDecalage + partDecalage2, b->c);
      // gfx_filledCircle(b->x, b->y, b->size,b->c);
      
}


void move_bullet(struct Bullet *b) {
	b->distance += 15;
}

void check_distance(struct Bullet *b, Target *t, int i) {

	if ((b->y > 0) && (b->x > 0) && (b->y < gfx_screenHeight()) && (b->x < gfx_screenWidth())) {
 				if ((((b->x - t->x)*(b->x - t->x)) + ((b->y - t->y)*(b->y - t->y))) < 2500)
   			{
   					//gf("pooummm");
            t->is_explosion = 1;
            b->is_shot = 0;
            t->explosion_cpt += 1;
            // targetReInit(t);
 				}
     
  } else { // the bullet is outside the screen
      b->is_shot = 0;
  }
}

void init_shot(struct Bullet *b, double angle, struct Basic *ba) {
          b->is_shot = 1;
          b->fire_angle = angle;
          b->distance = 170;
          b->x_init = ba->x;
          b->fire_cpt += 1;
}

void targetShoot(Target *t) {
  t->tb.x_init = t->x;
  t->tb.y_init = t->y;
  t->tb.is_shot = 1;
  t->tb.fire_angle = 0 + rand() % 2;
}

void moveTargetBullet(Target *t) {
      t->tb.x = t->tb.distance * sin(t->tb.fire_angle) + t->tb.x_init;
      t->tb.y = t->tb.distance * cos(t->tb.fire_angle) + t->tb.y_init;
      // printf("%d %d %d \n", i, t->tb.y, gfx_screenHeight());
      if ((t->tb.y > gfx_screenHeight()) || (t->tb.x > gfx_screenWidth()) || (t->tb.x < 0) || (t->tb.y < 0) || (t->tb.is_explosionBasic == 1)) {
        t->tb.is_shot = 0;
        t->tb.is_explosionBasic = 0;
        t->tb.distance = 0;
        t->tb.fire_angle = 0;
      } else {
      t->tb.distance += 8;
      }

}





void animationBasic(Target *t) 
{
	int size = t->tb.size + 220;
  gfx_filledCircle(t->tb.x, t->tb.y, size, t->tb.c);
}

void drawTargetBullet(Target *t){
     gfx_filledCircle(t->tb.x, t->tb.y, t->tb.size, t->tb.c); 

     if (t->tb.is_explosionBasic == 1) {
       animationBasic(t);
     }
}

void checkTargetBulletBasicDistance(Target *t, struct Basic *b)
{
  if ((((b->x - t->tb.x)*(b->x - t->tb.x)) + ((b->y - t->tb.y)*(b->y - t->tb.y))) < 8500)
  {
    // printf("explosion");
    t->tb.explosion_ctpBasic += 1;
    t->tb.is_shot = 0;
    t->tb.is_explosionBasic = 1;
  }
}

void newlevel(int *numberOfEnnimes, int level) {
// transition 
  *numberOfEnnimes += 2;
// realloc
}

void targetInit(Target *t, int level) {
      t->x = 0;
      t->y = 100 + rand() % 100;
      t->speed = level*2 + rand() % 7 ; //speed between 2 and 8
      t->size = 5 + rand() % 20;
      t->is_explosion = 0;
      t->c = GREEN;
      t->explosion_cpt = 0;
      t->direction = -1;
      t->tb.c = RED;
      t->tb.fire_angle = rand() % 180;
      t->tb.size = 5 + rand() % 20;
      t->tb.x = 0;
      t->tb.y = 0;
      t->tb.is_shot = 0;
      t->tb.distance = 0;
      t->tb.x_init = 0;
      t->tb.y_init = 0;
      t->tb.is_explosionBasic = 0;
      t->tb.explosion_ctpBasic = 0; 
      t->is_display = 1;
      t->tb.fireOdds = level*2;
}

#define NUMBER_OF_BULLET 05
#define RANDOM_NUMBER 1
#define NUMBER_OF_LIVES 10

int main() {

  if (gfx_init())
    exit(3);

  // level 1 data
  int numberOfEnnemies; 
  numberOfEnnemies = 3;
  int level_score = 1;

  // init Target Structure
  Target* t = NULL;
  t = malloc(sizeof(Target) * numberOfEnnemies);
  for (int i=0; i < numberOfEnnemies; i++) {
   targetInit(&(t[i]), level_score);
  }

  // initialize variables
  int score = 0;
  int death_score = 0;
  int fire_sc = 0; 
  int stop = 0;
  int total_TargetShoot =0;
  int targetLeft_score = 0;
  int delayCpt = 0;
  double angle = 90.0 * (M_PI / 180.0);
  int is_shooting = 0;

  // init Basic Structure
  struct Basic ba[1]; 
  ba[1].x = gfx_screenWidth() / 2;
  ba[1].y = gfx_screenHeight();
  ba[1].size = 100;
  ba[1].c = YELLOW;

  // init bullet structure
  struct Bullet b[NUMBER_OF_BULLET +1];
  for (int i=1; i < NUMBER_OF_BULLET +1; i++) {
    b[i].x = 0;
    b[i].y = 0;
    b[i].size = 10; 
    b[i].c = YELLOW;
    b[i].fire_angle = 0;
    b[i].is_shot = 0; // if is_shot = 1 then the bullet is shot
    b[i].fire_cpt = 0;
    b[i].x_init = ba->x;
  }
 
  while(!stop)
  { 
    
    //check scores
    score = total_TargetShoot;
    for (int i = 0; i < numberOfEnnemies; i++) {
      score += t[i].explosion_cpt;
      death_score += t[i].tb.explosion_ctpBasic;
      targetLeft_score += t[i].is_display;
    } 

    //end game when no more lives left
    if (death_score >= NUMBER_OF_LIVES) {
      stop = 1;
      char *lose = "Game Lose !";
      gfx_textout(200,3000, lose, WHITE);

      gfx_updateScreen();
      sleep(5);
      
    } 

    //fire score: number of bullet shot 
    for (int j = 1; j < NUMBER_OF_BULLET +1; j++) {
      fire_sc += b[j].fire_cpt;
    }  
    draw_basic(angle, &(ba[1]), score, fire_sc, death_score, NUMBER_OF_LIVES, level_score, targetLeft_score);
    fire_sc = 0;
    death_score = 0;

    // next level
    if (targetLeft_score == 0) {
      // animation transition 
      // new level 
      // realloc + reinit ennemies
      total_TargetShoot += score - total_TargetShoot;
      newlevel(&(numberOfEnnemies), level_score);
      t = realloc(t, sizeof(Target)* numberOfEnnemies);
      printf("%d", numberOfEnnemies);
      for (int i=0; i < numberOfEnnemies; i++) {
        targetInit(&(t[i]), level_score);
      }
      level_score += 1;
      char *nextLevel = "Next level !";
      char level_char [5];
      // middle of the screen
      gfx_filledCircle(gfx_screenWidth()/2,gfx_screenHeight()/2, 150, RED);
      gfx_textout(-50 + gfx_screenWidth()/2,gfx_screenHeight()/2, nextLevel, WHITE);
      gfx_textout(gfx_screenWidth()/2, 30 + gfx_screenHeight()/2, SDL_itoa(level_score, level_char, 10), WHITE);

      gfx_updateScreen();
      sleep(2);
    }
    targetLeft_score = 0;

    // draw targets
    for (int i=0; i<numberOfEnnemies;i++) {
      // if t->direction = 0 or 1, draw target
      // else => random number to display 

      if (t[i].is_display == 1) {

        
        if (t[i].direction != -1) {
        //printf(1adress main 1 : %u", &(t[i]));
        draw_target(&(t[i]), i);
        move_target(&(t[i]), i);
        // printf("%d %d\n", i, t[i]->tb.is_shot);
        if (t[i].tb.is_shot == 1){
          //  printf("draw");
          drawTargetBullet(&(t[i]));
          moveTargetBullet(&(t[i]));
          checkTargetBulletBasicDistance(&(t[i]), &(ba[1]));
          //  printf("is shot %d\n", t[i].tb.is_shot);
        } else if (rand() % 1000 <= t[i].tb.fireOdds) { // + level + fireOdds

          // the target throw a bullet 
          //  printf("shoot\n");
          targetShoot(&(t[i]));
        }
        } else if (RANDOM_NUMBER == (rand() % 4))  {
            t[i].direction = 0; 
        }
      }
    }
    
    // draw bullets already shot 
    if(is_shooting)
    {
      for (int i = 1; i < NUMBER_OF_BULLET +1; i++){
	      if (b[i].is_shot == 1) {
          draw_bullet(&(b[i]));
	        move_bullet(&(b[i]));
        }
      }

     	for (int j = 1; j<NUMBER_OF_BULLET +1;j++) {
        // for each target we need to check the distance with all bullets, then explode them if they are close 
		    for (int k = 0; k < numberOfEnnemies; k++) {
          if (b[j].is_shot == 1) {
			      check_distance((&b[j]), &(t[k]), k);
	    	}	
    	}
    }
    }

    gfx_updateScreen();

    // listen keyboard
    if(gfx_isKeyDown(SDLK_SPACE))
    { 
      if (delayCpt == 0) {
        is_shooting = 1;
        for (int i=1; i<NUMBER_OF_BULLET +1; i++) {
          if (b[i].is_shot == 0) {
            //gf("oui");
            init_shot(&(b[i]), angle, &(ba[1]));
            delayCpt++;
            break;
          }
        }
      }
    }
    if(gfx_isKeyDown(SDLK_c)) {
     ba[1].x -= 5;
    }
    if(gfx_isKeyDown(SDLK_v)) {
     ba[1].x += 5;
    }
    
    if (delayCpt >= 20) {
      // here we define how many bullet we can throw per second according to SDL_Delay(..) AND delayCpt >= ... 
      delayCpt = 0;
    } else if (delayCpt != 0) {
      delayCpt += 1;
    }
    
    SDL_Delay(10);
  };
  free(t);
  return 0;
}
