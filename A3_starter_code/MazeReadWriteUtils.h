#ifndef MAZEREADWRITEUTILS_H
#define MAZEREADWRITEUTILS_H


#include <iostream>
#include <vector>
#include "mcpp/mcpp.h"
#include <queue>
#include <deque>


bool ReadBasePoint(mcpp::Coordinate& start){
    std::string input="";
    bool success = true;

    mcpp::MinecraftConnection mc;

    std::cout << "In Minecraft, navigate to where you need the maze" << std::endl;
    std::cout << "to be built and type - done: " << std::endl;
    
    std::cin >> input;
    if(std::cin.fail()){
        success = false;
    }

    if(success){
        start = mc.getPlayerPosition() + mcpp::Coordinate(1,0,1);
    }else{
        std::cout << "Basepoint is not correct." << std::endl;
        std::cin.clear();
        std::cin.ignore(1000, '\n');
    }
    
    return success;
   
}

bool readLengthWidth(unsigned int& xlength, unsigned int& zlength){

    bool success = true;

    std::cout << "Enter the length and width of maze: " << std::endl;
    std::cin >> xlength;
    std::cin >> zlength;

    if (xlength == 0 || zlength == 0){
        success = false;
        std::cout <<
            "Length and width arguments are not correct!" << std::endl;
        std::cin.clear();
        std::cin.ignore(1000, '\n');
    }

    if (xlength%2 == 0 || zlength%2 == 0){
        success = false;
        std::cout <<
            "Length and width should to be odd numbers!" << std::endl;
    }

    return success;

}

bool readMaze(std::vector< std::vector<char> >& maze, mcpp::Coordinate& basePoint){

    unsigned int xlength = 0;
    unsigned int zlength = 0;

    bool success = ReadBasePoint(basePoint);

    if(success){
        success = readLengthWidth(xlength, zlength);
    }

    if(success){

        std::cout << 
        "Enter the maze structure: " 
        << std::endl;

        unsigned int charsRead = 0;
        char readC;

        maze.resize(zlength);
        for (unsigned int z = 0; z < zlength; z++){
            maze[z].resize(xlength);
            for (unsigned int x = 0; x < xlength; x++){
                if(std::cin.good()){
                    std::cin >> readC;
                    if(readC == 'x' || readC == '.'){
                        maze[z][x] = readC;
                        ++charsRead;
                    }
                }
            }
            
        }
        if (charsRead != (xlength * zlength)) {
            success = false;
            std::string errorString = "Structure not correct - only read ";
            errorString += std::to_string(charsRead);
            errorString += " characters instead of ";
            std::cout << errorString;
            std::cout << zlength * zlength << std::endl;
            std::cin.clear();
            std::cin.ignore(1000, '\n');
        }
    }

    return success;
    
}

bool validateMaze(const std::vector<std::vector<char>>& M,
                  int& entX, int& entZ,
                  bool& hasLoops, bool& hasIsolated)
{
    int Z = M.size(), X = M[0].size();
    std::vector<std::pair<int,int>> opens;

    // 1) collect '.' on the boundary
    auto checkCell = [&](int z,int x){
      if (M[z][x]=='.') opens.emplace_back(z,x);
    };
    for (int x=0; x<X; ++x) { checkCell(0,x); checkCell(Z-1,x); }
    for (int z=1; z<Z-1; ++z) { checkCell(z,0); checkCell(z,X-1); }
    if (opens.size()!=1) return false;
    std::tie(entZ,entX) = opens[0];

    // 2) flood-fill connectivity
    int total=0, reached=0;
    for (auto& row:M) for (char c:row) if (c=='.') ++total;
    std::vector<std::vector<bool>> seen(Z, std::vector<bool>(X,false));
    std::queue<std::pair<int,int>> q;
    seen[entZ][entX]=true;
    q.push({entZ,entX});
    while(!q.empty()){
      auto [z,x] = q.front(); q.pop(); ++reached;
      for (auto d: std::vector<std::pair<int,int>>{{1,0},{-1,0},{0,1},{0,-1}}) {
        int nz=z+d.first, nx=x+d.second;
        if (nz>=0&&nz<Z&&nx>=0&&nx<X
            && !seen[nz][nx] && M[nz][nx]=='.') {
          seen[nz][nx] = true;
          q.push({nz,nx});
        }
      }
    }
    hasIsolated = (reached < total);

    // 3) cycle‐detection via edge vs vertex count
    int edges=0, verts=0;
    for(int z=0; z<Z; ++z) for(int x=0; x<X; ++x){
      if (M[z][x]=='.'){
        ++verts;
        if (z+1<Z && M[z+1][x]=='.') ++edges;
        if (x+1<X && M[z][x+1]=='.') ++edges;
      }
    }
    hasLoops = (edges > verts-1);

    return !(hasIsolated||hasLoops);
}

// Stub for future auto‐fix logic
void autoFixMaze(std::vector<std::vector<char>>& /*M*/) {
    std::cout << "Auto‐fix not implemented yet.\n";
}

void printMaze(std::vector< std::vector<char> >& maze){
    std::cout << "**Printing Maze Structure**" << std::endl;
    for (unsigned int z = 0; z < maze.size(); z++){
        for (unsigned int x = 0; x < maze[0].size(); x++){
            if(maze[z][x] == 'x'){
                std::cout << 'x';
            }else{
                std::cout << '.';
            }
            
        }
        std::cout << std::endl;
            
    }
    std::cout << "**End Printing Maze**" << std::endl;
    std::cout << std::endl;

}



#endif



