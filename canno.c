#include "primlib.h"
#include <stdlib.h>
#include <math.h>
#include <time.h> 
#include <stdio.h>

struct Target {
	int x;
	int y;
	int speed;
	int is_explosion;
	int size;
	enum color c; // target.c means color
  int explosion_cpt; 
  int direction; // -1 if target is not displayed, 0 or 1 if it is
};

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

struct TargetBullet {
  int x;
  int y;
  int size;
  enum color c;
  int fire_angle;
};

void targetReInit(struct Target *t) {
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
void draw_basic(double angle, struct Basic *b, int score, int fire_score) {
    
    double delta_angle= 2.0 * (M_PI/180.0);
    int x1_barrel = 150 * cos(angle-delta_angle);
    int y1_barrel = 150 * sin(angle-delta_angle);
    int x2_barrel = 150 * cos(angle+delta_angle);
    int y2_barrel = 150 * sin(angle+delta_angle);
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
    char count [5];
    char fire_count [5];



    //SDL_itoa(score, buf, 10); 
//    itoa(score, buf, 10;

    gfx_textout(10, 10, x, WHITE);
    gfx_textout(150, 50, SDL_itoa(score, count, 10), RED);
    gfx_textout(20, 50, scoreTarget, WHITE);
    gfx_textout(150, 80, SDL_itoa(fire_score, fire_count, 10), RED);
    gfx_textout(20, 80, scoreBullet, WHITE);

}

void animation(struct Target *t, int i) 
{
	//t.size +=100;

	int size = t->size + 100;
	int x = t->x;
	int y = t->y;
  // printf("address animation 3: %u  \n", t); 
  gfx_filledCircle(x, y, size, t->c);

  targetReInit(t);
}


void draw_target(struct Target *tt, int i)
{	

  gfx_filledCircle(tt->x, tt->y, tt->size, tt->c);
 	
  //printf("adress draw_target 2: %u    \n", tt);
	if (tt->is_explosion == 1) 
	{
		animation(tt, i);
	}
}


void move_target(struct Target *t, int i) {
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

      gfx_filledCircle(b->x, b->y, b->size,b->c);
}


void move_bullet(struct Bullet *b) {
	b->distance += 5;
}

void check_distance(struct Bullet *b, struct Target *t, int i) {

	if ((b->y > 0) && (b->x > 0) && (b->y < gfx_screenHeight) && (b->x < gfx_screenWidth)) {
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

void targetShoot(struct Target *t) {

}

#define NUMBER_OF_ENNEMY 3
#define NUMBER_OF_BULLET 30
#define RANDOM_NUMBER 1
#define NUMBER_OF_TARGET_BULLET 10


int main() {

  if (gfx_init())
    exit(3);
  // srand(time( NULL ));
  struct Target t[NUMBER_OF_ENNEMY];
  for (int i=0; i < NUMBER_OF_ENNEMY; i++) {
    t[i].x = 0;
    t[i].y = 100 + rand() % 500;
    t[i].speed = 2 + rand() % 7; //speed between 2 and 8
    t[i].size = 5 + rand() % 20;
    t[i].is_explosion = 0;
    t[i].c = GREEN;
    t[i].explosion_cpt = 0;
    t[i].direction = -1;
  }

  struct TargetBullet tb[NUMBER_OF_TARGET_BULLET];
  for (int i = 0; i < NUMBER_OF_TARGET_BULLET; i++) {
    tb[i].x = 0;
    tb[i].y = 0;
    tb[i].size = 0;
    tb[i].c = RED;
    tb[i].fire_angle = 0;
  }

  int delayCpt = 0;
  // int bulletPerSec = 0;
  double angle = 90.0 * (M_PI / 180.0);
  int is_shooting = 0;

  struct Bullet b[NUMBER_OF_BULLET];
  struct Basic ba[1]; 


  ba[1].x = gfx_screenWidth() / 2;
  ba[1].y = gfx_screenHeight();
  ba[1].size = 100;
  ba[1].c = YELLOW;



  for (int i=0; i < NUMBER_OF_BULLET; i++) {
    b[i].x = 0;
    b[i].y = 0;
    b[i].size = 10; 
    b[i].c = YELLOW;
    b[i].fire_angle = 0;
    b[i].is_shot = 0; // if is_shot = 1 then the bullet is shot
    b[i].fire_cpt = 0;
    b[i].x_init = ba->x;
  }
 
  int score = 0;
  int fire_sc = 0; 

  while(1)
  { 
    for (int i = 0; i < NUMBER_OF_ENNEMY; i++) {
      score += t[i].explosion_cpt;
    } 

    for (int j = 0; j < NUMBER_OF_BULLET; j++) {
      fire_sc += b[j].fire_cpt;
    }  
    draw_basic(angle, &(ba[1]), score, fire_sc);
    fire_sc = 0;
    score = 0;                  
    // draw targets
    // srand(time( NULL ));
      // printf("%d %d\n", time(NULL), rand());
    for (int i=0; i<NUMBER_OF_ENNEMY;i++) {
      // if t->direction = 0 or 1, draw target
      // else => random number to display 

      if (t[i].direction != -1) {
       //printf("adress main 1 : %u", &(t[i]));
	     draw_target(&(t[i]), i);
       move_target(&(t[i]), i);
       if (rand() % 5 == RANDOM_NUMBER) {
         // the target throw a bullet 
         targetShoot(&(t[i]));
       }
      } else if (RANDOM_NUMBER == (rand() % 4))  {
          t[i].direction = 0; 
      }
    }
    
    // draw bullets 
    if(is_shooting)
    {
      for (int i = 0; i < NUMBER_OF_BULLET; i++){
	      if (b[i].is_shot == 1) {
          //gf("bullet %d", i);
          draw_bullet(&(b[i]));
	        move_bullet(&(b[i]));
        }
      }

     	for (int j = 0; j<NUMBER_OF_BULLET;j++) {
        // for each target we need to check the distance with all bullets, then explode them if they are close 
		    for (int k = 0; k < NUMBER_OF_ENNEMY; k++) {
          if (b[j].is_shot == 1) {
			      check_distance((&b[j]), &(t[k]), k);
	    	}	
    	}
    }
    }
    
   // y_target = cos(x_target)*50 + 50;
    
    gfx_updateScreen();

    /**if(gfx_isKeyDown(SDLK_RIGHT))
     angle -= 1.0 * (3.14/180.0);
    if(gfx_isKeyDown(SDLK_LEFT))
     angle += 1.0 * (3.14/180.0); */
    if(gfx_isKeyDown(SDLK_SPACE))
    { 
      //gf("%ld    ", timeNow);
      if (delayCpt == 0) {
        is_shooting = 1;
        for (int i=0; i<NUMBER_OF_BULLET; i++) {
          if (b[i].is_shot == 0) {
            //gf("oui");
            init_shot(&(b[i]), angle, &(ba[1]));
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
    
    if (delayCpt >= 4) {
      // here we define how many bullet we can throw per second according to SDL_Delay(..) AND delayCpt >= ... 
      delayCpt = 0;
    } else {
      delayCpt += 1;
    }
    SDL_Delay(10);
  };
  return 0;
}
