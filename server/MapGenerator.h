#include <iostream>
#include "ctime"
#define MIN_HEIGHT 9
#define MIN_WIDTH 11

class MapGenerator{
private:
  int width;
  int halfWidth;
  int height;
  Cell** finalMap;
  Cell** halfMap;

  //DFS Algorithm functions
  void mapGeneratorDFS(Cell** map, int x, int y);
  bool cornersVisited(int i, int j);
  void shuffle(int a[], int n);
  void swap(int &a, int &b);

  // Other "cleaning functions"
  void createCorners();
  void mapMirroring();
public:
  MapGenerator(int h, int w);
  Cell** generateMap();
  void printMap();

  int getWidth();
  int getHeight();
};

MapGenerator::MapGenerator(int h, int w){
  if(h < MIN_HEIGHT) h = MIN_HEIGHT;
  if(w < MIN_WIDTH) w = MIN_WIDTH;
  if(w % 2 == 0) w += 1; //Must be even

  height = h;
  width = w;
  halfWidth = (w - 1) / 2;

  finalMap = new Cell*[h];
  halfMap = new Cell*[h];

  for (int x = 0; x < h; ++x) {
    for (int y = 0; y < w; ++y) {
      if(y == 0) finalMap[x] = new Cell[w];
      finalMap[x][y] = Cell(x, y, WALL);
    }

    // Split the map in two parts. The middle column will always be the same.
    for (int y = 0; y < halfWidth; ++y) {
      if(y == 0) halfMap[x] = new Cell[halfWidth];
      halfMap[x][y] = Cell(x, y, WALL);
    }
  }
}

Cell** MapGenerator::generateMap(){
  srand(time(0));
  mapGeneratorDFS(halfMap, 0, 0);
  createCorners();
  mapMirroring();
  return finalMap;
}

void MapGenerator::printMap(){
  for (int x = height-1; x > -1; --x) {
    for (int y = 0; y < width; ++y) {
      if(finalMap[x][y].getCellType() == WALL) std::cout << "#";
      else if(finalMap[x][y].getCellType() == FOOD) std::cout << "·";
      else if(finalMap[x][y].getCellType() == JAIL) std::cout << "X";
      else std::cout << " ";
    }
    std::cout << "\n";
  }
}

int MapGenerator::getWidth(){
  return width;
}

int MapGenerator::getHeight(){
  return height;
}

void MapGenerator::mapGeneratorDFS(Cell** halfMap, int x, int y){
  int direct[][2] = {{0,1}, {0,-1}, {-1,0}, {1,0}};
  int visitOrder[] = {0,1,2,3};

  // If one of this three cases are true, then we don't change the Cell state.
  if(x < 0 || y < 0 || x >= height || y >= halfWidth) return ;
  if(halfMap[x][y].getCellType() == FOOD) return ;
  if(cornersVisited(x, y)) return ;

  // If the three previous conditions are false, then the cell state will be CORRIDOR
  halfMap[x][y].setCellType(FOOD);

  // Shuffle the visitOrder Array
  shuffle(visitOrder, 4);

  for (int k = 0; k < 4; ++k){
    int ni = x + direct[visitOrder[k]][0];
    int nj = y + direct[visitOrder[k]][1];
    mapGeneratorDFS(halfMap, ni, nj);
  }
}

// This function is the responsible of avoiding a 2x2 matrix of corridors cells.
bool MapGenerator::cornersVisited(int i, int j){
  /*
  Array with all neighbors of a given position. This is sorted following:
       ___________________________
      | {1,-1}  | {1,0}  | {1,1}  |
      | {0,-1}  | {i,j}  | {0,1}  |
      | {-1,-1} | {-1,0} | {-1,1} |
      |___________________________|
  */
  int neightbors[][2] = {{1,0}, {1,1}, {0,1}, {-1,1}, {-1,0}, {-1,-1}, {0,-1}, {1,-1}};

  // Looping over the 4 corner neightbors.
  for (int k = 1; k < 8; k+=2){
      // {ni, nj} -> corner
      int ni = i + neightbors[k][0];
      int nj = j + neightbors[k][1];
      // {niprev, njprev} -> previous neighbor.
      int niprev = i + neightbors[((k-1) % 8)][0];
      int njprev = j + neightbors[((k-1) % 8)][1];
      // {ninext, njnext} -> next neighbor.
      int ninext = i + neightbors[((k+1) % 8)][0];
      int njnext = j + neightbors[((k+1) % 8)][1];

      //out of boundary tests
      if(ni < 0 || nj < 0 || ni >= height || nj >= halfWidth) continue;
      if(niprev < 0 || njprev < 0 || niprev >= height || njprev >= halfWidth) continue;
      if(ninext < 0 || njnext < 0 || ninext >= height || njnext >= halfWidth) continue;

      //If the corner and its next and previous neighbors are corridors, then return true
      if(halfMap[ni][nj].getCellType() == FOOD &&
         halfMap[niprev][njprev].getCellType() == FOOD &&
         halfMap[ninext][njnext].getCellType() == FOOD) return true;
  }
  return false;
}

void MapGenerator::shuffle(int a[], int n){
    for (int i = 0; i < n; ++i)
    {
        swap(a[i], a[rand() % n]);
    }
}

void MapGenerator::swap(int & a, int &b){
    int c = a;
    a = b;
    b = c;
}

void MapGenerator::createCorners(){
  for (int x = 0; x < height; ++x) {
    halfMap[x][halfWidth - 1].setCellType(WALL);
    halfMap[x][halfWidth - 2].setCellType(FOOD);
  }
  for (int y = 0; y < halfWidth-1; ++y) {
    halfMap[0][y].setCellType(WALL);
    halfMap[height-1][y].setCellType(WALL);
    halfMap[1][y].setCellType(FOOD);
    halfMap[height-2][y].setCellType(FOOD);
  }

  for (int x = 0; x < 4; ++x) {
    for (int y = 0; y < 4; ++y){
      CellType c = JAIL;
      if(x == 2 || y == 2) c = WALL;
      if(x == 3 || y == 3) c = FOOD;
      halfMap[(int) (height/2) + y][x].setCellType(c);
      halfMap[(int) (height/2) - y][x].setCellType(c);
    }
  }
}

void MapGenerator::mapMirroring(){
  for (int x = 0; x < height; ++x) {
    for (int y = 0; y < halfWidth; ++y) {
      finalMap[x][y+halfWidth+1].setCellType(halfMap[x][y].getCellType());
      finalMap[x][halfWidth-1-y].setCellType(halfMap[x][y].getCellType());
    }
  }
  for (int x = 1; x < height-1; ++x) {
    // All corridors except the two borders and the position (heigth/2 + 3).
    if(x != (height/2)-2) finalMap[x][halfWidth].setCellType(FOOD);
    if(abs((height/2) - x) < 2) finalMap[x][halfWidth].setCellType(JAIL);
    if((x - (height/2)) == 2) finalMap[x][halfWidth].setCellType(JAIL);
  }
}
