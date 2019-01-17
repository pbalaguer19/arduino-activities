#include <iostream>
#include "ctime"

#define DURATION 200

#define MOVE 1
#define QUIET 2

#define Y 10
#define PLAYER_RADIUS 8
#define FOOD_RADIUS 3
#define GHOST_RADIUS 5

class PacManTextures{
private:
  Cell** map;
  MapGenerator* mapGenerator;
  int playerXPos;
  int playerYPos;
  particle player;

  int *ghostsXPos;
  int *ghostsYPos;
  particle *ghosts;
  GLenum *ghostLights;

  int COLUMNS;
  int ROWS;
  float WIDTH;
  float HEIGHT;
  float PIXELS_PER_COLUMN;
  float PIXELS_PER_ROW;

  float HEIGHT_DISPLACEMENT;
  float WIDTH_DISPLACEMENT;

  int GHOSTS;

  void definePlayerPosition(){
    ghostsXPos = new int[GHOSTS];
    ghostsYPos = new int[GHOSTS];
    ghosts = new particle[GHOSTS];

    for(playerXPos = 1; playerXPos < COLUMNS; playerXPos++){
      for(playerYPos = 1; playerYPos < ROWS; playerYPos++){
        if(map[playerYPos][playerXPos].getCellType() == FOOD){
          map[playerYPos][playerXPos].setCellType(PLAYER);
          return;
        }
      }
    }
  }

  void defineGhostsPositions(){
    int c = -2;
    int r = -1;

    for(int i = 0; i < GHOSTS; i++){
      ghostsXPos[i] = COLUMNS / 2 + c;
      ghostsYPos[i] = ROWS / 2 + r;
      r += 1;
      if(r >= 2){
        r = -1;
        c++;
      }

      map[ghostsYPos[i]][ghostsXPos[i]].setCellType(GHOST);
      ghosts[i].set_position(getPosition(ghostsXPos[i], PIXELS_PER_COLUMN), getPosition(ghostsYPos[i], PIXELS_PER_ROW));
    }
  }

  float getPosition(int n, float pixels) {
    return (n * pixels) ;
  }

  void setPlayerPosition(int newX, int newY, ParticleDirection direction){
    if(isWall(newX, newY)) return;

    int oldXPos = playerXPos;
    int oldYPos = playerYPos;

    playerXPos = newX;
    playerYPos = newY;
    player.init_movement(getPosition(playerXPos, PIXELS_PER_COLUMN), getPosition(playerYPos, PIXELS_PER_ROW), DURATION, direction, GL_LIGHT1);

    map[newY][newX].setCellType(PLAYER);
    map[oldYPos][oldXPos].setCellType(CORRIDOR);
  }

  bool isWall(int x, int y) {
    Cell cell = map[y][x];
    if(cell.getCellType() == WALL || cell.getCellType() == JAIL)
      return true;
    else return false;
  }

  void setGhostsPositions(long t){
    int direct[][3] = {{0,1,0}, {0,-1, 1}, {-1,0,3}, {1,0,2}};
    GLenum lights[6] = {GL_LIGHT2, GL_LIGHT3, GL_LIGHT4, GL_LIGHT5, GL_LIGHT6, GL_LIGHT7};


    for(int i = 0; i < GHOSTS; i++){
      if(ghosts[i].isReady()){
        int pos;
        int ni, nj;
        ParticleDirection direction;

        do{
          pos = rand() % 4;
          ni = ghostsXPos[i] + direct[pos][0];
          nj = ghostsYPos[i] + direct[pos][1];
          direction = getParticleDirection(direct[pos][2]);
        }while(map[nj][ni].getCellType() == WALL);

        CellType cellType = map[ghostsYPos[i]][ghostsXPos[i]].getPreviousCellType();
        map[nj][ni].setCellType(GHOST);
        map[ghostsYPos[i]][ghostsXPos[i]].setCellType(cellType);
        ghostsXPos[i] = ni;
        ghostsYPos[i] = nj;
        ghosts[i].init_movement(getPosition(ni, PIXELS_PER_COLUMN), getPosition(nj, PIXELS_PER_ROW), DURATION, direction, lights[i]);
      }

      ghosts[i].integrate(t);
    }
  }

  void drawSphere(int radius, int x, int z, float r, float g, float b){
    float baseX = WIDTH_DISPLACEMENT + (PIXELS_PER_COLUMN / 2);
    float baseY = radius;
    float baseZ = HEIGHT_DISPLACEMENT - (PIXELS_PER_ROW / 2);

    glPushMatrix();
    glColor3f(r, g, b);
    GLUquadric *quad;
    quad = gluNewQuadric();
    glTranslatef(baseX + x*PIXELS_PER_COLUMN, baseY, baseZ - z*PIXELS_PER_ROW);
    gluSphere(quad,radius,100,20);
    glEnd();
    glPopMatrix();
  }

  ParticleDirection getParticleDirection(int val){
    if(val == 0) return UPDIR;
    if(val == 1) return DOWNDIR;
    if(val == 2) return RIGHTDIR;
    return LEFTDIR;
  }

public:
  PacManTextures(int c, int r, float w, float h, int ghostsNumber){
    mapGenerator = new MapGenerator(r, c);
    map = mapGenerator->generateMap();
    mapGenerator->printMap();

    ROWS = mapGenerator->getHeight();
    COLUMNS = mapGenerator->getWidth();
    WIDTH = w;
    HEIGHT = h;

    GHOSTS = ghostsNumber;
    if(ghostsNumber < 1) GHOSTS = 1;
    if(ghostsNumber > 16) GHOSTS = 15;

    PIXELS_PER_COLUMN = WIDTH / COLUMNS;
    PIXELS_PER_ROW = HEIGHT / ROWS;

    HEIGHT_DISPLACEMENT = (HEIGHT / 2);
    WIDTH_DISPLACEMENT = - (WIDTH / 2);

    definePlayerPosition();
    defineGhostsPositions();

    player.set_position(getPosition(playerXPos, PIXELS_PER_COLUMN), getPosition(playerYPos, PIXELS_PER_ROW));
  }

  void drawCorridor(int i, int j){
    GLfloat material[4] = {1.0, 1.0, 1.0, 1.0};

    Cell cell = map[j][i];
    if(cell.getCellType() == WALL) {
      float x = (i * PIXELS_PER_COLUMN) + WIDTH_DISPLACEMENT;
      float y = 0.0;
      float z = HEIGHT_DISPLACEMENT - (j * PIXELS_PER_ROW);

      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D,1);
      glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,material);
      glBegin(GL_QUADS);
      glNormal3f(0,0,1);
      glTexCoord2f(0,0);glVertex3i(x,y,z);
      glTexCoord2f(1,0);glVertex3i(x,y+Y,z);
      glTexCoord2f(1, 1);glVertex3i(x+PIXELS_PER_COLUMN,y+Y,z);
      glTexCoord2f(0,1);glVertex3i(x+PIXELS_PER_COLUMN,y,z);
      glEnd();

      glBegin(GL_QUADS);
      glNormal3f(1,0,0);
      glTexCoord2f(0,0);glVertex3i(x+PIXELS_PER_COLUMN,y,z);
      glTexCoord2f(1,0);glVertex3i(x+PIXELS_PER_COLUMN,y+Y,z);
      glTexCoord2f(1, 1);glVertex3i(x+PIXELS_PER_COLUMN,y+Y,z-PIXELS_PER_ROW);
      glTexCoord2f(0,1);glVertex3i(x+PIXELS_PER_COLUMN,y,z-PIXELS_PER_ROW);
      glEnd();

      glBegin(GL_QUADS);
      glNormal3f(-1,0,0);
      glTexCoord2f(0,0);glVertex3i(x,y,z);
      glTexCoord2f(1,0);glVertex3i(x,y+Y,z);
      glTexCoord2f(1, 1);glVertex3i(x,y+Y,z-PIXELS_PER_ROW);
      glTexCoord2f(0,1);glVertex3i(x,y,z-PIXELS_PER_ROW);
      glEnd();

      glBegin(GL_QUADS);
      glNormal3f(0,0,-1);
      glTexCoord2f(0,0);glVertex3i(x,y,z-PIXELS_PER_ROW);
      glTexCoord2f(1,0);glVertex3i(x,y+Y,z-PIXELS_PER_ROW);
      glTexCoord2f(1, 1);glVertex3i(x+PIXELS_PER_COLUMN,y+Y,z-PIXELS_PER_ROW);
      glTexCoord2f(0,1);glVertex3i(x+PIXELS_PER_COLUMN,y,z-PIXELS_PER_ROW);
      glEnd();

      glBegin(GL_QUADS);
      glNormal3f(0,1,0);
      glTexCoord2f(0,0);glVertex3i(x,y+Y,z);
      glTexCoord2f(1,0);glVertex3i(x,y+Y,z-PIXELS_PER_ROW);
      glTexCoord2f(1, 1);glVertex3i(x+PIXELS_PER_COLUMN,y+Y,z-PIXELS_PER_ROW);
      glTexCoord2f(0,1);glVertex3i(x+PIXELS_PER_COLUMN,y+Y,z);
      glEnd();

      glDisable(GL_TEXTURE_2D);

    }
  }

  void drawFood(int i, int j) {
    float x, y;
    Cell cell = map[j][i];
    if(cell.getCellType() == FOOD){
      drawSphere(FOOD_RADIUS, i, j, 1.0, 0, 0.0);
    }
  }

  void drawGhosts() {
    float baseX = WIDTH_DISPLACEMENT + (PIXELS_PER_COLUMN / 2);
    float baseZ = HEIGHT_DISPLACEMENT - (PIXELS_PER_ROW / 2);

    for(int i = 0; i < GHOSTS; i++){
     ghosts[i].draw(GHOST_RADIUS, 0.5, 0.2, 1.0, baseX, baseZ);
    }
  }

  void drawPlayer() {
    float baseX = WIDTH_DISPLACEMENT + (PIXELS_PER_COLUMN / 2);
    float baseZ = HEIGHT_DISPLACEMENT - (PIXELS_PER_ROW / 2);

    player.draw(PLAYER_RADIUS, 1.0, 1.0, 0, baseX, baseZ);
  }

  void playerUP(){
    setPlayerPosition(playerXPos, playerYPos+1, UPDIR);
  }

  void playerDOWN(){
    setPlayerPosition(playerXPos, playerYPos-1, DOWNDIR);
  }

  void playerLEFT(){
    setPlayerPosition(playerXPos-1, playerYPos, LEFTDIR);
  }

  void playerRIGHT(){
    setPlayerPosition(playerXPos+1, playerYPos, RIGHTDIR);
  }

  void integrate(long t){
    player.integrate(t);
    setGhostsPositions(t);
  }
};
